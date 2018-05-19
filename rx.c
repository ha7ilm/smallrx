#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <complex.h>
#include <limits.h>
#include <math.h>

float hamming(float x) { return (0.54-0.46*(2*M_PI*(0.5+(x/2)))); }

int main(int argc, char *argv[]) {
    //constraints: you should run it like: rx <center> <rx_freq> <l[sb]|u[sb]|a[m]|f[m]>
    if(argc<4) return 1;
    char mod = argv[3][0];
    const float samp_rate = 240000, decimate_transition_bw = 800, ssb_bw = 3000, amfm_bw = 12000;
    const int decimate_taps_length = (int)(4.0/(decimate_transition_bw/samp_rate)) | 1;
    float dshift = ((atoi(argv[2])-atoi(argv[1]))/samp_rate)*2*M_PI, shift = 0;
    fprintf(stderr, "dshift = %f, decimate_taps_length = %d\n", dshift, decimate_taps_length);
    complex float* decimate_taps = malloc(sizeof(complex float)*decimate_taps_length);
    complex float* decimate_buffer = calloc(sizeof(complex float),decimate_taps_length);
    int decimate_taps_middle=decimate_taps_length/2;
    const int output_rate = 48000;
    const int decimate_factor = samp_rate / output_rate;
    const complex float decimate_dshift = (mod=='u'?1:-1) * ((ssb_bw/2)/samp_rate)*2*M_PI;
    const float decimate_cutoff_rate = (mod=='u'||mod=='l') ? (ssb_bw/2)/samp_rate  : (amfm_bw/2)/samp_rate;
    decimate_taps[decimate_taps_middle]=2*M_PI*decimate_cutoff_rate*hamming(0);
    for(int i=1; i<=decimate_taps_middle; i++) 
        decimate_taps[decimate_taps_middle-i] = decimate_taps[decimate_taps_middle+i] =
            (sin(2*M_PI*decimate_cutoff_rate*i)/i) * hamming((float)i/decimate_taps_middle);
    if(1 && (mod=='u'||mod=='l')) for(int i=0; i<decimate_taps_length; i++) {
        decimate_taps[i] *= (sinf(shift) + cosf(shift) * I);
        shift += decimate_dshift;
        while(shift>2*M_PI) shift-=2*M_PI;
    }
    float decimate_taps_sum = 0;
    for(int i=0; i<decimate_taps_length; i++) decimate_taps_sum += cabsf(decimate_taps[i]);
    for(int i=0; i<decimate_taps_length; i++) decimate_taps[i] /= decimate_taps_sum;
    FILE* filterfile = fopen("test.m", "w");
    fprintf(filterfile, "freqz([");
    for(int i=0; i<decimate_taps_length; i++) fprintf(filterfile, " %f+(%f*i)\n", creal(decimate_taps[i]), cimag(decimate_taps[i]));
    fprintf(filterfile, "]);input(\"\");");
    int decimate_counter = 0;
    float last_phi = 0;
    while(1) {
        //load input
        complex float a = CMPLX(((float)getchar())/(UCHAR_MAX/2.0)-1.0, ((float)getchar())/(UCHAR_MAX/2.0)-1.0);
        //shift
        shift += dshift;
        while(shift>2*M_PI) shift-=2*M_PI;
        a *= sinf(shift) + cosf(shift) * I;
        //decimate
        decimate_buffer[decimate_taps_length-decimate_factor+decimate_counter] = a;
        if(++decimate_counter >= decimate_factor) {
            decimate_counter = 0;
            complex float d = CMPLX(0,0);
            for(int i=0; i<=decimate_taps_length; i++) { d += decimate_buffer[i] * decimate_taps[i]; }

            //fwrite(&d, sizeof(complex float), 1, stdout);
            //if(feof(stdin)) break;
            //continue;

            short o;
            //fmdemod
            if(mod=='f') {
                float phi = cargf(d);
                float dphi = phi-last_phi;
                last_phi = phi;
                while(dphi<-M_PI) dphi += 2*M_PI;
                while(dphi>M_PI) dphi -= 2*M_PI;
                o = (SHRT_MAX-1)*(dphi/M_PI);
            }
            //amdemod
            else if(mod=='a') o = cabsf(d);
            //ssbdemod
            else o = crealf(d);
            //output
            fwrite(&o, sizeof(short), 1, stdout);
            if(feof(stdin)) break;
            //memmove
            memmove(decimate_buffer, decimate_buffer+decimate_factor, (decimate_taps_length-decimate_factor)*sizeof(complex float));
        }
         
    }
}
