#include <nmmintrin.h> // for SSE4.2
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// drums
__m128d drm(double t)
{
	double kk,sn;
	double tt = t * 3.,ft = fmod(tt, 1.);
	uint_fast8_t ut = (uint_fast8_t)tt % 12u; 
	// kick
	kk = .316 * sin(471. * ft * exp(-8.*ft)) +.707 * sin(311. *  ft * exp(-2.*ft));
	kk *= (double[]){1., 0., 0., 1., 0., 1., 0., 1., 1., 0., 0., 0.}[ut];
	kk *= .63 * atan2(1. - ft, 5. * ft);
	
	// snare
	double x = t * 5623.123456;
	sn = fmod(x * fmod(t + x, 1.), 1.);
	sn += .5 * sin(6.283185 * t * 170.) + sin(6.283185 * t * 300.) * exp(-10.*ft);
	sn *= (double[]){0., 0., 1., 0., 0., 0., 1., 0., 0., 1., 1., 0.}[ut];
	sn *= atan2(1. - ft, 40. * ft);
	
	// out
	return (__m128d) { kk + .223 * sn, kk + .316 * sn };
}


#define SR 48000. // sampelrate

int main()
{
	FILE *pipeout;
	pipeout = _popen("ffmpeg -y -hide_banner -f f32le -ar 48000 -ac 2 -i - -threads 4 -c:a pcm_s16le sound.wav", "wb");
	
	// Num of sampels - duration
	const uint_fast32_t N =  (uint_fast32_t)SR * 8u; 

	// buffer
	float *y;
	y = (float *)malloc(N * sizeof(*y));

	uint_fast32_t s;
	for(s=0; s <N; ++s)
	{
		
		double t = (double)s / SR;
		y[s] =	 drm(t)[0];// left
		y[s+1] = drm(t)[1];// right

		// write buffer
		fwrite(&y[s], sizeof(float), 2, pipeout);

	} 

	// clean up
	_pclose(pipeout);
}
