/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#include <stdlib.h>
#include <assert.h>

#include "memset_s.h"

/*
 * http://www.dwheeler.com/secure-programs/Secure-Programs-HOWTO/protect-secrets.html
 */
void * memset_s(void *ptr, int c, size_t amount)
{
    /*
     * Sanity checks
     */
    assert(ptr != NULL);
    assert(amount > 0);

    volatile char *tmp = ptr;
    while (amount--) {
        *tmp++ = c;
    }
    return ptr;
}
