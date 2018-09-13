/*
 * Copyright (c) 2015 Josh Williams <theprime@codingprime.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the COPYING file for details.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <alsa/asoundlib.h>

#include "alsa_card.h"
#include "log.h"

namespace VOsd {
	AlsaCard::AlsaCard(int pCardNum) :
		mError(""), mMixerHandle(NULL) {
		mCardNum = pCardNum;
	}

	AlsaCard::~AlsaCard(void) {
		int err;
		if (mMixerHandle != NULL) {
			for (MixerList::iterator vIt = mMixers.begin(); vIt != mMixers.end(); ++vIt) {
				delete vIt->second;
			}

			err = snd_mixer_close(mMixerHandle);
			if (err < 0) {
				Log().Error() << "snd_mixer_close(" << err << "): " << snd_strerror(err);
			}
			mMixerHandle = NULL;
			snd_config_update_free_global();
		}
	}

	bool AlsaCard::Init(void) {
		int err;
		char* vName;
		snd_mixer_elem_t* elem;
		std::ostringstream vNameSS, vErrSS;
		vNameSS << "hw:" << mCardNum;
		std::string vCardName = vNameSS.str();
		if ((err = snd_card_get_name(CardId(), &vName)) < 0) {
			Log().Error() << "Unable to get card name: " << err;
		} else {
			mName = vName;
			free(vName);
		}

		if ((err = snd_mixer_open(&mMixerHandle, 0)) < 0) {
			vErrSS << "control hardware info(" << err << "): " << snd_strerror(err);
			mError = vErrSS.str();
			return false;
		}

		if ((err = snd_mixer_attach(mMixerHandle, vCardName.c_str())) < 0) {
			snd_mixer_close(mMixerHandle);
			mMixerHandle = NULL;
			vErrSS << "Mixer " << mCardNum << " attach error: " << snd_strerror(err);
			mError = vErrSS.str();
			return false;
		}

		if ((err = snd_mixer_selem_register(mMixerHandle, NULL, NULL)) < 0) {
			snd_mixer_close(mMixerHandle);
			mMixerHandle = NULL;
			vErrSS << "Mixer " << mCardNum << " register error: " << snd_strerror(err);
			mError = vErrSS.str();
			return false;
		}
		if ((err = snd_mixer_load(mMixerHandle)) < 0) {
			snd_mixer_close(mMixerHandle);
			vErrSS << "Mixer " << mCardNum << " error: " << snd_strerror(err);
			return false;
		}
		snd_config_update_free_global();

		AlsaMixer* vMixer;
		for (elem = snd_mixer_first_elem(mMixerHandle); elem; elem = snd_mixer_elem_next(elem)) {
			if (!snd_mixer_selem_is_active(elem)) {
				continue;
			}

			vMixer = new AlsaMixer(mMixerHandle, elem);
			mMixers[vMixer->Name()] = vMixer;
		}
        return true;
	}

	int AlsaCard::CardId(void) const {
		return mCardNum;
	}

	const std::string& AlsaCard::Name(void) const {
		return mName;
	}

	const std::string& AlsaCard::Error(void) const {
		return mError;
	}

	AlsaMixer* AlsaCard::Mixer(const std::string& pCardName) {
		MixerList::iterator vIt = mMixers.find(pCardName);
		if (vIt == mMixers.end()) {
			Log().Error() << "Mixer " << pCardName << " not found.";
			return NULL;
		} else {
			return vIt->second;
		}
	}

} // End Namespace

