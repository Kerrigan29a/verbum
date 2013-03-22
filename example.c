/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>

#include "verbum/verbum.h"
#include "foo.h"
#include "bar.h"

static void signalHandler(int signal) {
    if (signal == SIGILL) {
        puts("[-] Invalid instruction");
    } else if (signal == SIGSEGV) {
        puts("[-] Invalid memory reference");
    } else {
        printf("[-] Signal %d\n", signal);
    }
    exit(EXIT_FAILURE);
}

int decryptPayload(char * const salt, char * const passHash,
    char * const password, unsigned char * const payload, size_t amount)
{
#if defined(__x86_64__) || defined(__x86_64)
    char *alignedPtr = (char *) ((long long) payload & ~ (getpagesize() - 1));
#else
    char *alignedPtr = (char *) ((int) payload & ~ (getpagesize() - 1));
#endif

    char calculatedHash[32 + 1];

    /* Sanity checks */
    assert(salt != NULL);
    assert(passHash != NULL);
    assert(password != NULL);
    assert(payload != NULL);
    assert(amount > 0);

    /* Check the password */
    verbum_hash(salt, password, calculatedHash);
    printf("[+] calculatedHash = %s\n", calculatedHash);
    if (strncasecmp(calculatedHash, passHash, 32) != 0) {
        printf("[-] Incorrect password\n");
        return -1;
    }

    /* Change permissions */
    if (mprotect(alignedPtr, amount, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        perror("[-] Unable to change permissions");
        return -1;
    }
    printf("[+] Added write permission on code section\n");

    /* Decrypt payload*/
    verbum_decrypt(payload, amount, password, strlen(password));
    printf("[+] Data decrypted\n");

    /* Restore permissions */
    if (mprotect(alignedPtr, amount, PROT_READ | PROT_EXEC) == -1) {
        perror("[-] Unable to restore permissions");
        return -1;
    }
    printf("[+] Restored permission on code section\n");

    return 0;
}

int main (int argc, char * argv[])
{
    /* Sanity check */
    if(argc != 2) {
        fprintf(stderr, "[-] Unable to understand arguments\n");
        printf("Usage: %s <pass>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("[*] Pass = %s\n", argv[1]);

    /* Setting signal handler */
    if (signal(SIGILL, signalHandler) == SIG_ERR) {
        perror("[-] An error occurred while setting SIGILL signal handler.");
        return EXIT_FAILURE;
    }
    if (signal(SIGSEGV, signalHandler) == SIG_ERR) {
        perror("[-] An error occurred while setting SIGSEGV signal handler.");
        return EXIT_FAILURE;
    }

    /* Decrypt each seaction with his password */
#define BLOCK(name, salt, hash, amount) \
    puts("\n[ " #name " ]"); \
    puts("[*] Salt = " salt); \
    puts("[*] Hash = " hash); \
    puts("[*] Amount = " #amount); \
    if (decryptPayload(salt, hash, argv[1], (unsigned char *) name, amount) == 0) { \
        name(); \
    } else { \
        return EXIT_FAILURE; \
    }
#include "verbum.def"
#undef BLOCK
    return EXIT_SUCCESS;
}
