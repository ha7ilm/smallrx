#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

int main(int argc, char *argv[])
{
    //constraints: you should run it like: rx <center> <rx_freq> <USB|LSB|AM|FM>
    int i;
    unsigned char u;
    float complex buffer[10];
    float dshift = ((atoi(argv[2])-atoi(argv[1]))/2400000)*2*M_PI, shift = 0;
    while(1)
    {
        for(i=0;i<10;i++) 
        {
            buffer[i] = ((float)getchar()-127)/255 + I * ((float)getchar()-127)/255;
            shift += dshift;
            while(shift>2*M_PI) shift-=2*M_PI;
            buffer[i] *= sinf(shift) + cosf(shift) * I;
        }
        fwrite(buffer, sizeof(float complex), 10, stdout);
    }


}
