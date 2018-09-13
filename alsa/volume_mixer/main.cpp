/*
 * Copyright (c) 2015 Josh Williams <theprime@codingprime.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the COPYING file for details.
 */
#include <unistd.h>

#include "alsa.h"
#include "log.h"

using namespace VOsd;

struct Options {
	int HwId;
	std::string Mixer;
	std::string Action;
};

void commandDefaults(Options& pOptions) {
	pOptions.HwId = 0;
	pOptions.Mixer = "unknown";
	pOptions.Action = "unknown";
}

bool parseCommandLine(int pCount, char* pArgs[], Options& pOptions) {
	int c, vIndex;

	opterr = 0;

	while ((c = getopt(pCount, pArgs, "c:m:")) != -1) {
		switch (c) {
			case 'c':
				pOptions.HwId = atoi(optarg);
				break;
			case 'm':
				pOptions.Mixer = optarg;
				break;

		}
	}

	vIndex = optind;
	if (vIndex < pCount) {
		pOptions.Action = pArgs[vIndex];
	}

	return true;
}

int main(int argc, char *argv[]) {
	Options vOptions;
	commandDefaults(vOptions);
    long pVolume = 0;

	if (parseCommandLine(argc, argv, vOptions)) {
		Alsa* alsa = new Alsa();

		if (alsa->Init()) {
			AlsaCard* vCard = alsa->Card(vOptions.HwId);
			if (vCard != NULL) {
				AlsaMixer* vMixer = vCard->Mixer(vOptions.Mixer);
				if (vMixer != NULL) {
					Log().Debug() << "Found Mixer!";
					Log().Debug() << "  " << vCard->Name();
					Log().Debug() << "    " << vMixer->Name();
					long vCurrent;

					if (vOptions.Action.compare("up") == 0) {
						vMixer->VolumeChange(AlsaMixer::MixerLeft, 1);
						vMixer->VolumeChange(AlsaMixer::MixerRight, 1);
                        vMixer->getVolume(AlsaMixer::MixerLeft, &pVolume);
					} else if (vOptions.Action.compare("down") == 0) {
						vMixer->VolumeChange(AlsaMixer::MixerLeft, -1);
						vMixer->VolumeChange(AlsaMixer::MixerRight, -1);
					} else if (vOptions.Action.compare("mute") == 0) {
						vMixer->ToggleMute();
					}

				}
			}
		} else {
			Log().Debug() << "Unable to initialize.";
		}

		delete alsa;
		return 0;
	} else {
		Log().Error() << "Unable to parse command line.";
	}
}

