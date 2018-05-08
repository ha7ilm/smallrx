#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <limits.h>
#include <math.h>

int main(int argc, char *argv[])
{
    //constraints: you should run it like: rx <center> <rx_freq> <USB|LSB|AM|FM>
    const float samp_rate = 2400000;
    float dshift = ((atoi(argv[2])-atoi(argv[1]))/samp_rate)*2*M_PI, shift = 0;
    fprintf(stderr, "%f\n", dshift);
    while(1)
    {
        //load input
        complex float a = CMPLX(((float)getchar())/(UCHAR_MAX/2.0)-1.0, ((float)getchar())/(UCHAR_MAX/2.0)-1.0);
        //shift
        shift += dshift;
        while(shift>2*M_PI) shift-=2*M_PI;
        a *= sinf(shift) + cosf(shift) * I;
        //decimate
        //fmdemod
        //amdemod
        //output
        float i = creal(a), q = cimag(a);
        fwrite(&i, sizeof(float), 1, stdout);
        fwrite(&q, sizeof(float), 1, stdout);
        if(feof(stdin)) break;
    }
}
