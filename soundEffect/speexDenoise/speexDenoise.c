/*
 * this demo is test for speex denoise function.
 *
 * Para:
 *    samplerate=16K, framesize=320, frametime=20ms
 *
 * Usage:
 *    ./speexDenoise < in.pcm > out.pcm
 *
 * Build:
 *    -lspeexdsp
 *
 * Author:
 *    @LaMar
 */

#include <speex/speex_preprocess.h>
#include <stdio.h>

// this demo set the frametime = 20ms; samplerate = 16000
// framesize = (frametime*samplerate)/1000 (frametime_unit:ms)
#define FSIZE          (320)
#define SAMPLERATE     (16000)

int main()
{
   SpeexPreprocessState *st;
   short in[FSIZE] = {0};
   int i = 0;
   int count = 0;
   float f = 0.0;

   // Initialization
   st = speex_preprocess_state_init(FSIZE, SAMPLERATE); //framesize,samplerate

   // enable NS
   i=1;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &i);

   // disable AGC
   i=0;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC, &i);

   // The other effect for sound
   /*
   i=8000;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &i);
   i=0;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB, &i);
   f=.0;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
   f=.0;
   speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);
   */

   // Denoise process
   while (1)
   {
      fread(in, sizeof(short), FSIZE, stdin);
      if (feof(stdin)) {
          break;
      }

      speex_preprocess_run(st, in);
      fwrite(in, sizeof(short), FSIZE, stdout);
      count++;
   }
   printf("process cnt:%d", count);
   speex_preprocess_state_destroy(st);

   return 0;
}
