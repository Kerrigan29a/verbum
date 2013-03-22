/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "rc4.h"
 
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
 
static void _swap(unsigned char *s, unsigned int i, unsigned int j)
{
    /*
     * Sanity checks
     */
    assert(s != NULL);

    unsigned char temp = s[i];
    s[i] = s[j];
    s[j] = temp;
}
 
/* KSA */
int RC4_init(RC4_ctx * const ctx, unsigned char *key, size_t keyLen)
{
    /*
     * Sanity checks
     */
    assert(ctx != NULL);
    assert(key != NULL);
    assert(keyLen > 0);

    for (ctx->i = 0; ctx->i < 256; ctx->i++) {
        ctx->S[ctx->i] = ctx->i;
    }
 
    for (ctx->i = ctx->j = 0; ctx->i < 256; ctx->i++) {
        ctx->j = (ctx->j + key[ctx->i % keyLen] + ctx->S[ctx->i]) & 255;
        _swap(ctx->S, ctx->i, ctx->j);
    }
 
    ctx->i = ctx->j = 0;

    return 0;
}
 
/* PRGA */
unsigned char RC4_output(RC4_ctx * const ctx)
{
    /*
     * Sanity checks
     */
    assert(ctx != NULL);

    ctx->i = (ctx->i + 1) & 255;
    ctx->j = (ctx->j + ctx->S[ctx->i]) & 255;
 
    _swap(ctx->S, ctx->i, ctx->j);
 
    return ctx->S[(ctx->S[ctx->i] + ctx->S[ctx->j]) & 255];
}
