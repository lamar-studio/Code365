/*
 * Copyright (c) 2015 Josh Williams <theprime@codingprime.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the COPYING file for details.
 */
#ifndef __VOSD_ALSA_H__
#define __VOSD_ALSA_H__

#include <string>
#include <vector>
#include "alsa_card.h"

namespace VOsd {

	class Alsa {
	public:
		Alsa(void);
		~Alsa(void);

		bool                    Init(void);
		int						CardCount(void) const;
		AlsaCard*               Card(int pCardNum);
	private:

		bool                    mInitialized;
		std::vector<AlsaCard *> mCards;
	};
} // End Namespace

#endif /* !__VOSD_ALSA_H__ */

