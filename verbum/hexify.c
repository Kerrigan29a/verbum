/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#include <assert.h>
#include <stdlib.h>

void hexify(const unsigned char * const bin, size_t binLen, char * const hex)
{
    register size_t i;
    unsigned int j;

    /*
     * Sanity checks
     */
    assert(bin != NULL);
    assert(binLen > 0);
    assert(hex != NULL);

    for (i = 0; i < binLen; ++i) {
        j = (bin[i] >> 4) & 0x0f;
        hex[i * 2] = j <= 9 ? (j + '0') : (j + 'a' - 10);
        j = bin[i] & 0x0f;
        hex[i * 2 + 1] = j <= 9 ? (j + '0') : (j + 'a' - 10);
    }
    hex[binLen * 2] = '\0';
}
