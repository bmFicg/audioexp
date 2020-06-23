// -----------------------------------------------------
// twitter: @t_nabroski
// https://github.com/bmFicg/audioexp/edit/fdn/fdn/main.c
// License Creative Commons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
// https://creativecommons.org/licenses/by-nc/4.0/
// -----------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


static inline double mat_mult(const double s[4], const double M[4][4], uint8_t i)
{
    double r = s[0] * M[i][0] + s[1] * M[i][1]+ s[2] * M[i][2]+ s[3] * M[i][3];
    return r;
}
// drums
static inline double drm(double t)
{
    double kk,sn;
    double tt = t * 3., ft = fmod(tt, 1.);
    uint_fast16_t ut = (uint_fast16_t)tt % 12u;
    // kick
    kk = .316 * sin(471. * ft * exp(-8.*ft)) + .707 * sin(311. *  ft * exp(-2.*ft));
    kk *= (double[]){1., 0., 0., 1., 0., 0.,1., 0., 1., 1., 0., 0.}[ut];
    kk *= .63 * atan2(1. - ft, 10. * ft);
    // snare
    double x = t * 5623.123456;
    sn = fmod(x * fmod(t + x, 1.), 1.);
    sn += .5 * sin(6.283185 * t * 170.) + sin(6.283185 * t * 300.) * exp(-10.*ft);
    sn *= (double[]){0., 0., 1., 0., 0., 1., 0., 1., 0., 0., 1., 1.}[ut];
    sn *=  .25 * atan2(1. - ft, 40. * ft);
    // out
    return kk +sn;
}


#define SR 48000. // sampelrate

int main()
{
    FILE *pipeout;
    pipeout = _popen("ffmpeg -y -hide_banner -f f64le -ar 48000 -ac 1 -i - -threads 4 -c:a pcm_s16le sound.wav", "wb");
    
    // Num of sampels - duration
    const uint_fast32_t N = (uint_fast32_t)SR * 8u;
    // buffer
    double *y;
    y = (double *)malloc(N * sizeof(*y));
    
    // feedback matrix https://ccrma.stanford.edu/~jos/pasp/Feedback_Delay_Networks_FDN.html
    const double A[4][4] = {{ 1e-6, -.685894, .685894, 1e-6},{ .685894, 1e-6, 1e-6, .685894},{ .685894, 1e-6, 1e-6,-.685894},{ 1e-6, -.685894,-.685894, 1e-6}};
    
    const uint_fast32_t dt[4] = {577, 601, 641, 661}; //{149, 211, 263, 293}
    double  z1[661] = { 0. },
            z2[661] = { 0. },
            z3[661] = { 0. },
            z4[661] = { 0. };

    uint_fast32_t s;
    for(s=0; s <N; s++)
    {
        double t = (double)s / SR;
       

        double tmp[4];
        tmp[0] = z1[s % dt[0]];
        tmp[1] = z2[s % dt[1]];
        tmp[2] = z3[s % dt[2]];
        tmp[3] = z4[s % dt[3]];

        y[s] = .707 * drm(t) + //dry
        .354 * (z1[s % dt[0]] +
                z2[s % dt[1]] +
                z3[s % dt[2]] +
                z4[s % dt[3]] );
        double x = .354 * drm(t);
        z1[s % dt[0]] = x + mat_mult(tmp, A, 0u);
        z2[s % dt[1]] = x + mat_mult(tmp, A, 1u);
        z3[s % dt[2]] = x + mat_mult(tmp, A, 2u);
        z4[s % dt[3]] = x + mat_mult(tmp, A, 3u);

        // write buffer
        fwrite(&y[s], sizeof(double), 1, pipeout);
    }
    // clean up
    _pclose(pipeout);
    free(y);
}
