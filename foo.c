/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#include <stdio.h>
#include <string.h>

char *prefix = "[+] It's ";
char *suffix = " - MAGIC";
char txt[512];

int foo()
{
    char *name = "FOO";
    char *actual = txt;

    memset(txt, 0, 512);

    strcpy(actual, prefix);
    actual += strlen(prefix);
    strcpy(actual, name);
    actual += strlen(name);
    strcpy(actual, suffix);

    puts(txt);
    return 0;
}
