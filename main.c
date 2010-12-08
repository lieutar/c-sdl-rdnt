#include <SDL.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>

const double PI  = 3.141592653589793238462643383279502884197;
double Vol       = 2.0;
double Frequency = 180;
double Freq2     = 5.5;
int    Delay     = -1;
Uint16 Sec       = 22141;
double PlayTime  = 1000 * 60 * 90 * 4;
int    Pow       = 10;
int    Debug     = 0;
char   *Mus      = NULL;

SDL_AudioSpec Desired;
SDL_AudioSpec Obtained;

double wave(double t)
{
  return 1.0 + cos(PI * t * 2);
}

void callback_stereo(void *unused, Uint8 *stream, int len)
{
  int i;
  static unsigned int step = 0;
  Uint16 *frames = (Uint16 *) stream;
  if(Debug) printf("----%d----\n", len);
  for (i = 0; i < len / 2; i += 2) {
    double l, r;
    step++;
    l = wave(((double)step) * (Frequency / Obtained.freq));
    r = wave(((double)step) * ((Frequency + Freq2) / Obtained.freq));
    frames[i]   = Vol * l * 3000;
    frames[i+1] = Vol * r * 3000;
    if(Debug) printf("%f * 3000 = %d\n", (float)l, frames[i]);
  }
}

void callback(void *unused, Uint8 *stream, int len)
{
  int i;
  static unsigned int step = 0;
  Uint16 *frames = (Uint16 *) stream;
  int framesize = len / 2;
  if(Debug) printf("----%d----\n", len);
  for (i = 0; i < framesize; i++) {
    double d, v;
    step++;
    d = step * (Frequency / Obtained.freq);
    v = sin(PI * 2 * (0.5 + ((double)step / (0.37 / 1.0)) * 
                      (Freq2 * (0.37 / 1.0 / 2)) /
                      Obtained.freq));
    frames[i] = Vol * pow(v, Pow) * (1 + wave(d)) * 3000;
  }
}

int main(int argc, char *argv[])
{
  int c;
  

  Desired.freq     = 22050; /* Sampling rate: 22050Hz */
  Desired.format   = AUDIO_S16LSB; /* 16-bit signed audio */
  Desired.channels = 0; /* Mono */
  /* Buffer size: 8K = 0.37 sec. */
  /*Desired.samples  = 8192;*/
  /*printf("%d\n",(int)Sec);*/
  Desired.samples  = 8192; 
  Desired.callback = callback;
  Desired.userdata = NULL;


  while((c = getopt(argc, argv, "dbH:v:w:t:p:m:")) > -1){

    switch(c){

    case 'H':
      Frequency = atof(optarg);
      break;

    case 'v':
      Vol       = atof(optarg);
      break;

    case 'w':
      Freq2     = atof(optarg);
      break;

    case 't':
      Delay     = atoi(optarg);
      break;

    case 'p':
      Pow       = atoi(optarg);
      break;

    case 'm':
      Mus       = optarg;
      break;

    case 'b':
      Desired.format   = AUDIO_S16LSB;
      Desired.callback = callback_stereo;
      Desired.channels = 2; /* Mono */
      break;

    case 'd':
      Debug = 1;
      break;

    }
  }

  {
    Mix_Music *m;

    SDL_Init( SDL_INIT_AUDIO );

    if(Mus){
      Mix_OpenAudio(
                    MIX_DEFAULT_FREQUENCY,
                    AUDIO_S16LSB,
                    2,
                    1024
                    );
      Mix_AllocateChannels(2);
      m = Mix_LoadMUS(Mus);
      if(Debug) printf("-m %s\n", Mus);
      if(m){
        Mix_PlayMusic(m, -1);
      }else{
        printf("%s\n",Mix_GetError());
      }
    }

    SDL_OpenAudio(&Desired, &Obtained);
    SDL_PauseAudio(0);

    if(Delay > 0) SDL_Delay(1000 * Delay);
    else do SDL_Delay(30000); while(1);
    if(m){
      Mix_HaltMusic();
      Mix_FreeMusic(m);
      Mix_CloseAudio();
    }
    SDL_Quit();
  }
  return 0;
}

