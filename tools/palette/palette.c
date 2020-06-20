/*
 * palette.c - generate a NRZI sequence for WS2812B from
 * gimp exported palettes.
 * 
 * (1) export from gimp a palette as css format: palette.css
 * (2) preformat with:
 *     "cat palette.css | grep -o "rgb([0-9]*, [0-9]*, [0-9]*)" | sed "s/)/),/g" > palette.inc"
 * (3) gcc -o palette palette.c
 * (4) ./palette > palette.h
 * 
 * Stephan Baerwolf (matrixstorm@gmx.de), Schwansee 2020
 * (please contact me at least before commercial use)
 */

#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>

#define luminancescaling_numerator      (1)
#define luminancescaling_denominator    (1)
#define bytespercolor                   (3)


//permute into WS2812B and do lumincance scaling
#define rgb(r,g,b) {(g*luminancescaling_numerator)/luminancescaling_denominator, (r*luminancescaling_numerator)/luminancescaling_denominator, (b*luminancescaling_numerator)/luminancescaling_denominator}



static uint8_t palette[256][(bytespercolor)] = {
    // the 256's color is replaced as WS2812B "reset" signal (6 consecutive pixels reset the bus)
#   include "palette.inc"
};



static uint8_t __NRZIsequence[3*(bytespercolor)][256];

int main(int argc, char **argv) {
    int x,y,z;

    //build up __NRZIsequence (this also encodes the color 255 which is overwritten later)
    for (x=0;x<256;x++) {
        //palette[x][0..2] is the data value to translate into nrz
        for (z=0;z<(bytespercolor);z++) {
            //walk through color-channels
            uint8_t i, tmp;
            uint32_t nrzibits = 0;

            //convert one channel to nrz
            tmp=palette[x][z];
            for (i=0;i<8;i++) {
                nrzibits<<=3;
                if ((tmp & 1) != 0) nrzibits|=0b011; //1
                else                nrzibits|=0b001; //0
                tmp>>=1;
            }

            //save the nrz to the sequence
            for (y=0;y<3;y++) {
                __NRZIsequence[y+(z*3)][x]=nrzibits & 0xff;
                nrzibits>>=8;
            }
        }
    }


    //insert the reset-sequence into colorcode 255
    for (y=0;y<(3*(bytespercolor));y++) {
        __NRZIsequence[y][255]=0;
    }


    //output the array as C structure
    printf("static const PROGMEM __attribute__ ((used,aligned(256))) uint8_t __NRZIsequence[%u][256] = {\n",(3*(bytespercolor)));
    for (y=0;y<(3*(bytespercolor));y++) {
        printf("\t{");
        for (x=0;x<256;x++) {
            printf("0x%02x%s",__NRZIsequence[y][x],(x<255)?", ":"");
        }
        printf("}%s\n",(y<((3*(bytespercolor))-1))?",":""); 
    }
    printf("};\n");


    return EXIT_SUCCESS;
}
