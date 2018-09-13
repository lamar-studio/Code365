/*
 * Copyright (c) 2015 Josh Williams <theprime@codingprime.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the COPYING file for details.
 */
#include "alsa_mixer.h"
#include <cmath>
#include "log.h"

#define MAX_LINEAR_DB_SCALE	24

static inline bool use_linear_dB_scale(long dBmin, long dBmax)
{
	return dBmax - dBmin <= MAX_LINEAR_DB_SCALE * 100;
}

static long lrint_dir(double x, int dir)
{
	if (dir > 0)
		return lrint(ceil(x));
	else if (dir < 0)
		return lrint(floor(x));
	else
		return lrint(x);
}

namespace VOsd {
	AlsaMixer::AlsaMixer(snd_mixer_t* pMixer, snd_mixer_elem_t* pElem)
		: mMixer(pMixer), mElem(pElem) {
		snd_mixer_selem_id_malloc(&mSid);
		snd_mixer_selem_get_id(pElem, mSid);
	}

	AlsaMixer::~AlsaMixer(void) {
		if (mSid != NULL) {
			snd_mixer_selem_id_free(mSid);
		}
	}

	const std::string& AlsaMixer::Name(void) {
		if (mName.length() == 0) {
			mName.assign(snd_mixer_selem_id_get_name(mSid));
		}
		return mName;
	}

	bool AlsaMixer::setVolume(MixerChannel pWhich, long pVolume) {
		long vMin, vMax, vValue;
		double vVolume, vMinNormalized;
		int vErr;

		if (!MinMaxDB(&vMin, &vMax) || vMin >= vMax) {
            if (!MinMaxVolume(&vMin, &vMax) || vMin == vMax)
                return false;

            pVolume= lrint_dir(pVolume*(vMax - vMin), 0) + vMin;
            return setVolumeRaw(pWhich, pVolume);
		}


        if (use_linear_dB_scale(vMin, vMax)) {
            pVolume= lrint_dir(pVolume*(vMax - vMin), 0) + vMin;
            return setVolumeDB(pWhich, pVolume);
        }

		if (vMin != SND_CTL_TLV_DB_GAIN_MUTE) {
			vMinNormalized = exp(((vMin - vMax) / 6000.0) * log(10));
			pVolume = (pVolume - vMinNormalized) / (1 - vMinNormalized);
		}

        pVolume = lrint_dir(6000.0 * log10(pVolume), 0) + vMax;
        return setVolumeDB(pWhich, pVolume);
	}

	bool AlsaMixer::getVolume(MixerChannel pWhich, long* pVolume) {
		long vMin, vMax, vValue;
		double vVolume, vMinNormalized;
		int vErr;

		if (!MinMaxDB(&vMin, &vMax) || vMin >= vMax) {
            if (!MinMaxVolume(&vMin, &vMax) || vMin == vMax)
                return false;
            if(!VolumeRaw(pWhich, pVolume))
                return false;
            *pVolume = (*pVolume - vMin) / (vMax - vMin);
            return true;
		}

		if (!VolumeDB(pWhich, &vValue)) {
			return false;
		}

        if (use_linear_dB_scale(vMin, vMax)) {
            *pVolume = (*pVolume - vMin) / (vMax - vMin);
            return true;
        }

		vVolume = exp(((vValue - vMax) / 6000.0) * log(10));
		if (vMin != SND_CTL_TLV_DB_GAIN_MUTE) {
			vMinNormalized = exp(((vMin - vMax) / 6000.0) * log(10));
			vVolume = (vVolume - vMinNormalized) / (1 - vMinNormalized);
		}

		*pVolume = round(vVolume * 100);
		VOsd::Log().Debug() << "get Normalized Volume: " << *pVolume;

		return true;
	}


	bool AlsaMixer::VolumeChange(MixerChannel pWhich, long pAmount) {
		long vMin, vMax, vVolume, vStep;
		int vErr;
		double vMinNormalized, vVolumeNormalized;
		snd_mixer_selem_channel_id_t vChannel;

		if (!VolumeDB(pWhich, &vVolume)) {
			VOsd::Log().Error() << "Couldn't retrieve current volume.";
			return false;
		}

		VOsd::Log().Debug() << "***************************************************************";
		VOsd::Log().Debug() << "Current Volume: " << vVolume;

		if (!MinMaxDB(&vMin, &vMax) || vMin >= vMax) {
			VOsd::Log().Error() << "Couldn't retrieve min/max";
			return false;
		}

		if (pWhich == MixerLeft) {
			vChannel = SND_MIXER_SCHN_FRONT_LEFT;
		} else if (pWhich == MixerRight) {
			vChannel = SND_MIXER_SCHN_FRONT_RIGHT;
		}

		vStep = (vMax - vMin) / 100.0;
		VOsd::Log().Debug() << "Step: " << vStep;

		vVolume += (vStep * pAmount);
		if (vVolume < vMin) {
			vVolume = vMin;
		} else if (vVolume > vMax) {
			vVolume = vMax;
		}

		VOsd::Log().Debug() << "New Volume: " << vVolume;
		VOsd::Log().Debug() << "Channel: " << vChannel;

		int vDir = pAmount > 0 ? 1 : -1;

		snd_mixer_selem_set_playback_dB(mElem, vChannel, vVolume, vDir);

		VolumeDB(MixerLeft, &vVolume);
		VOsd::Log().Debug() << "Adjusted Volume Left:  " << vVolume;
		VolumeDB(MixerRight, &vVolume);
		VOsd::Log().Debug() << "Adjusted Volume Right: " << vVolume;

		if (!VolumeDB(pWhich, &vVolume)) {
			VOsd::Log().Error() << "Couldn't retrieve current volume.";
			return false;
		}


		return true;

/* 		vVolume += pAmount; */
/* 		if (vVolume <= 0) { */
/* 			snd_mixer_selem_set_playback_dB(mElem, vChannel, vMin, -1); */
/* 			return true; */
/* 		} else if (vVolume >= 100) { */
/* 			snd_mixer_selem_set_playback_dB(mElem, vChannel, vMax, -1); */
/* 			return true; */
/* 		} else { */
/* 			long vStep = */
/* 			vVolumeNormalized = vVolume / 100.0; */
/* 			VOsd::Log().Debug() << "vVolumeNormalized: " << vVolumeNormalized; */
/* 			if (vMin != SND_CTL_TLV_DB_GAIN_MUTE) { */
/* 				vMinNormalized = exp(((vMin - vMax) / 6000.0) * log(10)); */
/* 				vVolumeNormalized = vVolumeNormalized * (1 - vMinNormalized) + vMinNormalized; */
/* 			} */
/* 			VOsd::Log().Debug() << "vVolumeNormalized: " << vVolumeNormalized; */
/* 			vVolumeNormalized = lrint(round(6000.0 * log10(vVolumeNormalized))) + vMax; */
/* 			VOsd::Log().Debug() << "Setting volume to: " << vVolumeNormalized; */
/* 			VOsd::Log().Debug() << "vMin: " << vMin; */
/* 			VOsd::Log().Debug() << "vMax: " << vMax; */
/* 			VOsd::Log().Debug() << "vMinNormalized: " << vMinNormalized; */
/* 			snd_mixer_selem_set_playback_dB(mElem, vChannel, vVolumeNormalized, 0); */
/* 			if (Volume(pWhich, &vVolume)) { */
/* 				VOsd::Log().Debug() << "Adjusted Volume: " << vVolume; */
/* 			} */
/* 			return true; */
/* 		} */
	}

	bool AlsaMixer::ToggleMute(void) {
	}

	bool AlsaMixer::MinMaxDB(long* pMin, long* pMax) {
		return MinMax(&snd_mixer_selem_get_playback_dB_range, pMin, pMax);
	}

	bool AlsaMixer::MinMaxVolume(long* pMin, long* pMax) {
		return MinMax(&snd_mixer_selem_get_playback_volume_range, pMin, pMax);
	}

	bool AlsaMixer::MinMax(RangeFunc pFunc, long* pMin, long* pMax) {
		int vErr;

		if ((vErr = (*pFunc)(mElem, pMin, pMax)) < 0) {
			VOsd::Log().Error() << "Unable to retrieve range: " << snd_strerror(vErr);
			return false;
		}

		return true;
	}

	bool AlsaMixer::VolumeDB(MixerChannel pWhich, long* pVolume) {
		return VolumeValue(&snd_mixer_selem_get_playback_dB, pWhich, pVolume);
	}

	bool AlsaMixer::VolumeRaw(MixerChannel pWhich, long* pVolume) {
		return VolumeValue(&snd_mixer_selem_get_playback_volume, pWhich, pVolume);
	}

	bool AlsaMixer::VolumeValue(VolumeFunc pFunc, MixerChannel pWhich, long* pVolume) {
		int vErr;
		snd_mixer_selem_channel_id_t vChannel;

		//VOsd::Log().Debug() << "pWhich: " << pWhich;
		if (pWhich == MixerLeft) {
			vChannel = SND_MIXER_SCHN_FRONT_LEFT;
		} else if (pWhich == MixerRight) {
			vChannel = SND_MIXER_SCHN_FRONT_RIGHT;
		} else {
			VOsd::Log().Error() << "Invalid channel specified: " << pWhich;
			return false;
		}

		if ((vErr = (*pFunc)(mElem, vChannel, pVolume)) < 0) {
			VOsd::Log().Error() << "Unable to retrieve volume: " << snd_strerror(vErr);
			return false;
		}

		return true;
	}


	bool AlsaMixer::setVolumeDB(MixerChannel pWhich, long pVolume) {
		return setVolumeValue(pWhich, pVolume, true);
	}

	bool AlsaMixer::setVolumeRaw(MixerChannel pWhich, long pVolume) {
		return setVolumeValue(pWhich, pVolume, false);
	}

    bool AlsaMixer::setVolumeValue(MixerChannel pWhich, long pVolume, bool isDB) {
        int vErr;
        snd_mixer_selem_channel_id_t vChannel;

        //VOsd::Log().Debug() << "pWhich: " << pWhich;
        if (pWhich == MixerLeft) {
            vChannel = SND_MIXER_SCHN_FRONT_LEFT;
        } else if (pWhich == MixerRight) {
            vChannel = SND_MIXER_SCHN_FRONT_RIGHT;
        } else {
            VOsd::Log().Error() << "Invalid channel specified: " << pWhich;
            return false;
        }

        if (isDB)
            vErr = snd_mixer_selem_set_playback_dB(mElem, vChannel, pVolume, 0);
        else
            vErr = snd_mixer_selem_set_playback_volume(mElem, vChannel, pVolume);

        if (vErr < 0) {
            VOsd::Log().Error() << "Unable to retrieve volume: " << snd_strerror(vErr);
            return false;
        }

        return true;
    }

} // End Namespace

