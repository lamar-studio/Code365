/*
 * Copyright (c) 2015 Josh Williams <theprime@codingprime.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the COPYING file for details.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <sstream>

#include <alsa/asoundlib.h>

#include "alsa.h"
#include "log.h"

namespace VOsd {

	Alsa::Alsa(void) {
		mInitialized = true;
	}

	Alsa::~Alsa(void) {
		if (mInitialized) {
			Log().Debug() << "Cleaning up cards...";
			for (std::vector<AlsaCard*>::const_iterator vIt = mCards.begin(); vIt != mCards.end(); ++vIt) {
				Log().Debug() << "Destroying card: " << (*vIt)->CardId();
				delete *vIt;
			}
			// Destroy stuffs
		}
	}

	bool Alsa::Init(void) {
		int err, dev;
		char name[32];
		int card = -1;
		AlsaCard *vCard;

		if (snd_card_next(&card) < 0 || card < 0) {
			Log().Debug() << "No cards found...";
			return false;
		}

		while (card >= 0) {
			vCard = new AlsaCard(card);
			if (!vCard->Init()) {
				Log().Error() << vCard->Error();
				delete vCard;
				return false;
			}
			mCards.push_back(vCard);
			if (snd_card_next(&card) < 0) {
				break;
			}
		}
		return true;
	}

	int Alsa::CardCount(void) const {
		return mCards.size();
	}

	AlsaCard* Alsa::Card(int pCardNum) {
		if (pCardNum >= mCards.size()) {
			Log().Error() << "Card number out of range: " << pCardNum;
			return NULL;
		}

		return mCards[pCardNum];
	}
} // End Namespace

