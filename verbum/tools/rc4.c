/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#include <stdio.h>

#include "../rc4.h"

int main(int argc, char* argv[]) {
    RC4_ctx ctx;
    int c;

    if(argc < 2) {
        fprintf(stderr, "%s <pass>\n", argv[0]);
        return 1;
    }

    RC4_init(&ctx, argv[1], strlen(argv[1]));

    while ((c = getchar()) != EOF) {
        printf("%c", c ^ RC4_output(&ctx));
        /* printf("%02X", c ^ RC4_output(&ctx)); */
    }

    return 0;
}
