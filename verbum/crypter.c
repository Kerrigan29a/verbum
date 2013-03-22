/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#include <stdlib.h>
#include <assert.h>

#include "rc4.h"

void encryptBlock(unsigned char * const buffer, size_t amount,
    unsigned char * const pass, size_t passLen)
{
    RC4_ctx ctx;
    register size_t i;

    /*
     * Sanity checks
     */
    assert(buffer != NULL);
    assert(amount > 0);
    assert(pass != NULL);
    assert(passLen > 0);

    /*
     * Init RC4 key
     */
    RC4_init(&ctx, pass, passLen);

    /*
     * Loop to encrypt
     */
    for(i = 0; i < amount; ++i) {
        unsigned char c = buffer[i];
        buffer[i]= c ^ RC4_output(&ctx);
    }

}
