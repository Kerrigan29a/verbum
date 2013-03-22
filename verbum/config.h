/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
 * Number of iterations to apply on Key Stretching
 * https://en.wikipedia.org/wiki/Key_stretching
 */
#define HASH_ITERATIONS 1000000

/* The device used to get random bytes */
#define RANDOM_DEV "/dev/random"

/* The length of the salt */
#define SALT_LEN 16

#endif /* __CONFIG_H__ */
