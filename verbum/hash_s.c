/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "md5.h"
#include "hexify.h"
#include "memset_s.h"
#include "config.h"


//int old_hash_s(char * const salt, char * const pass, char * const hash)
//{
//    register unsigned i;
//    unsigned char * buffer;
//    size_t chunkLen = 0;
//    MD5_CTX ctx;
//
//    /*
//     * Sanity checks
//     */
//    assert(salt != NULL);
//    assert(pass != NULL);
//    assert(hash != NULL);
//
//    /*
//     * Alloc mem and compose the buffer
//     */
//    chunkLen = strlen(salt) + strlen(pass);
//
//    buffer = (unsigned char *) malloc(
//        sizeof(unsigned char) * (chunkLen < 16 ) ? 16 + 1 : chunkLen + 1);
//    if (buffer == NULL) {
//        return -1;
//    }
//
//    strcpy((char *) buffer, salt);
//    strcpy((char *) buffer + strlen(salt), pass);
//    buffer[chunkLen] = '\0';
//
//    /* MD5 of the chunk (size = chunkLen) */
//    MD5_Init(&ctx);
//    MD5_Update(&ctx, buffer, chunkLen);
//    MD5_Final(buffer, &ctx);
//    buffer[16] = '\0';
//
//    /* Md5 of Md5 (size = 16) */
//    for (i = 0; i < (HASH_ITERATIONS - 1); ++i) {
//        MD5_Init(&ctx);
//        MD5_Update(&ctx, buffer, 16);
//        MD5_Final(buffer, &ctx);
//        buffer[16] = '\0';
//    }
//
//    hexify(buffer, 16, hash);
//
//    /*
//     * Free and burn mem
//     */
//    memset_s(buffer, 0, (chunkLen < 16 ) ? 16 + 1 : chunkLen + 1);
//    free(buffer);
//    buffer = NULL;
//
//    return 0;
//}

int hash_s(char * const salt, char * const pass, char * const hash)
{
    register unsigned i;
    unsigned char * buffer;
    size_t bufferLen = 0;

    /*
     * Sanity checks
     */
    assert(salt != NULL);
    assert(pass != NULL);
    assert(hash != NULL);

    /*
     * Alloc mem and compose the buffer
     */
    bufferLen = strlen(salt) + strlen(pass) + 16 /* Len of MD5 */;

    buffer = (unsigned char *) malloc( sizeof(unsigned char) * bufferLen);
    if (buffer == NULL) {
        return -1;
    }
    memset_s(buffer, 0, bufferLen);

    /*
     * https://en.wikipedia.org/wiki/Key_stretching
     *
     * key = ""
     * for 1 to HASH_ITERATIONS do
     *     key = hash(key + password + salt)
     */
    char * actual = (char *) buffer;
    assert(len(buffer) == 0);
    for (i = 0; i < HASH_ITERATIONS; ++i) {
        MD5_CTX ctx;

        strcpy(actual, pass);
        actual += strlen(pass);
        strcpy(actual, salt);
        actual += strlen(salt);
        *actual = '\0';

        MD5_Init(&ctx);
        MD5_Update(&ctx, buffer, actual - (char *) buffer);
        MD5_Final(buffer, &ctx);

        actual = (char *) buffer + 16;
    }

    hexify(buffer, 16, hash);

    /*
     * Free and burn mem
     */
    memset_s(buffer, 0, bufferLen);
    free(buffer);
    buffer = NULL;

    return 0;
}
