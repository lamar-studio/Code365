/*
 * Copyright (c) 2015 Josh Williams <theprime@codingprime.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the COPYING file for details.
 */
#ifndef __VOSD_ALSA_ALSA_CARD_H__
#define __VOSD_ALSA_ALSA_CARD_H__

#include <map>

#include "alsa_mixer.h"


namespace VOsd {

	typedef std::map<std::string, AlsaMixer*> MixerList;

	class AlsaCard {
	public:
		AlsaCard(int pCardNum);
		~AlsaCard(void);

		bool                    Init(void);
		int                     CardId(void) const;
		const std::string&      Name(void) const;
		const std::string&      Error(void) const;;
		void                    Dump(void) const;
		AlsaMixer*				Mixer(const std::string& pMixerName);
	private:
		std::string             mError;
		std::string             mName;
		int                     mCardNum;
		snd_mixer_t*            mMixerHandle;
		MixerList				mMixers;
	};

} // End Namespace

#endif // __VOSD_ALSA_ALSA_CARD_H__

