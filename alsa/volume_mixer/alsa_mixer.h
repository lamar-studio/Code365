/*
 * Copyright (c) 2015 Josh Williams <theprime@codingprime.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the COPYING file for details.
 */

#ifndef __VOSD_ALSA_ALSA_MIXER_H__
#define __VOSD_ALSA_ALSA_MIXER_H__

#include <alsa/asoundlib.h>
#include <string>

typedef int (*RangeFunc)(snd_mixer_elem_t*, long*, long*);
typedef int (*VolumeFunc)(snd_mixer_elem_t*, snd_mixer_selem_channel_id_t, long*);

namespace VOsd {
	class AlsaMixer {
	public:

		enum MixerChannel {
			MixerLeft,
			MixerRight
		};

		enum MixerMinMax {
			MixerMin,
			MixerMax
		};

        // enum MixerDirection {
            // MixerPlayback,
            // MixerCapture
        // }

		AlsaMixer(snd_mixer_t* pMixer, snd_mixer_elem_t* pElem);
		~AlsaMixer(void);

		const std::string&      Name(void);
		bool                    setVolume(MixerChannel pWhich, long pVolume);
		bool                    getVolume(MixerChannel pWhich, long* pVolume);
		bool                    VolumeChange(MixerChannel pWhich, long pVolume);
		bool                    ToggleMute(void);
	private:
		bool                    MinMaxDB(long* pMin, long* pMax);
		bool                    MinMaxVolume(long* pMin, long* pMax);
		bool                    VolumeDB(MixerChannel pWhich, long* pVolume);
		bool                    VolumeRaw(MixerChannel pWhich, long* pVolume);
		bool                    MinMax(RangeFunc pFunc, long* pMin, long* pMax);
		bool                    VolumeValue(VolumeFunc pFunc, MixerChannel pWhich, long* pValue);
        bool                    setVolumeDB(MixerChannel pWhich, long pVolume);
        bool                    setVolumeRaw(MixerChannel pWhich, long pVolume);
        bool                    setVolumeValue(MixerChannel pWhich, long pVolume, bool isDB);

		snd_mixer_t*            mMixer;
		snd_mixer_elem_t*		mElem;
		snd_mixer_selem_id_t*   mSid;
		std::string             mName;
	};
}

#endif // __VOSD_ALSA_ALSA_MIXER_H__

