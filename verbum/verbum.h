/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#ifndef __VERBUM_H__
#define __VERBUM_H__

#include "hash_s.h"
#include "crypter.h"

#define verbum_decrypt(buffer, amount, pass, passLen) \
    encryptBlock(buffer, amount, pass, passLen)

#define verbum_hash(salt, pass, hash) hash_s(salt, pass, hash)

#endif /* __VERBUM_H__ */
