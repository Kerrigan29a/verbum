/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#ifndef __CRYPTER_H__
#define __CRYPTER_H__

void encryptBlock(unsigned char * const buffer, size_t amount,
    char * const pass, size_t passLen);

#define decryptBlock(buffer, amount, pass, passLen) \
    encryptBlock(buffer, amount, pass, passLen)

#endif /* __CRYPTER_H__ */

