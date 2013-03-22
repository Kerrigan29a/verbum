/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#ifndef __RC4_H__
#define __RC4_H__

typedef struct _RC4_ctx {
    unsigned char S[256];
    unsigned int i;
    unsigned int j;
} RC4_ctx;

int RC4_init(RC4_ctx * const ctx, unsigned char *key, size_t keyLen);

unsigned char RC4_output(RC4_ctx * const ctx);

#endif /* __RC4_H__ */
