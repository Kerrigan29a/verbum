/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> /* For getopt */

#include "../hash_s.h"
#include "../memset_s.h"
#include "../hexify.h"
#include "../config.h"


void help(char * progName)
{
    printf(
        "Usage:\n"
        "    %1$s [-e] -f <file>\n"
        "    %1$s [-e] -p <pass>\n"
        "    %1$s -h\n"
        "\n"
        "Params:\n"
        "    -e               Generate the output in a esay way to parse: (salt:hash)\n"
        "    -h               Show help\n"
        "    -p <password>    Password\n",
        progName
    );
}

int main(int argc, char *argv[])
{
    char hashBuffer[32 + 1];
    char saltBuffer[SALT_LEN + 1];
    char saltHexBuffer[SALT_LEN * 2 +1];
    FILE *f = NULL;
    int ch;
    int result = EXIT_FAILURE;
    char *password = NULL;
    bool easyMode = false;

    while ((ch = getopt(argc, argv, "ehp:")) != -1) {
        switch(ch) {
         case 'e':
            easyMode = true;
            break;
         case 'h':
            help(argv[0]); 
            result = EXIT_SUCCESS;
            goto free_params;
        case 'p':
            password = strdup(optarg);
            break;
        case '?':
        default:
            result = EXIT_FAILURE;
            goto free_params;
        }
    }

    /* Sanity checks */
    if (password == NULL) {
        fprintf(stderr, "[-] Must provide the password\n");
        result = EXIT_FAILURE;
        goto free_params;
    }

    if(!easyMode) {
        printf("[*] Password = %s\n", password);
    }

    /* Generate random salt */
    f = fopen(RANDOM_DEV, "r");
    if(f == NULL) {
        perror("[-] Unable to open " RANDOM_DEV "\n");
        result = EXIT_FAILURE;
        goto free_params;
    }
    fread(saltBuffer, sizeof(char), SALT_LEN, f);
    saltBuffer[SALT_LEN] = '\0';
    hexify((unsigned char *) saltBuffer, SALT_LEN, saltHexBuffer);
    saltHexBuffer[SALT_LEN * 2] = '\0';

    printf((easyMode)? "%s:" : "[+] Salt = %s\n",
        saltHexBuffer);

    fclose(f);
    f = NULL;

    if(hash_s(saltHexBuffer, password, hashBuffer) == -1) {
        perror("[-] Unable to generate the hash\n");
        result = EXIT_FAILURE;
        goto free_params;
    }
    
    printf((easyMode)? "%s\n" : "[+] Hash = %s\n",
        hashBuffer);

    result = EXIT_SUCCESS;

free_params:
    if (password != NULL) {
        memset_s(password, 0, strlen(password));
        free(password);
    }

    return result;
}
