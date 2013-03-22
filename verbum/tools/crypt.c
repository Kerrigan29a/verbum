/*
 * Copyright (c) 2013 Javier Escalada Gómez
 * All rights reserved
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> /* For getopt */

#include "../crypter.h"
#include "../memset_s.h"

void help(char * progName)
{
    printf(
        "Usage:\n"
        "    %1$s [-b <base offset>] -s <start offset> (-t <stop offset> | -a <amount>) -f <file> -p <pass>\n"
        "    %1$s -h\n"
        "\n"
        "Params:\n"
        "    -a <amount>          Amount of bytes from <start offset> (including it)\n"
        "    -b <base offset>     Address to substract to <start offset> and <stop offset>\n"
        "    -h                   Show help\n"
        "    -f <file>            File to overwrite\n"
        "    -p <password>        Password\n"
        "    -s <start offset>    Start offset in the file. Could be relative to <base offset> or absolute\n"
        "    -t <stop offset>     Stop offset in the file. Could be relative to <base offset> or absolute\n",
        progName
    );
}

int main(int argc, char *argv[])
{
    FILE *f = NULL;
    unsigned long int start = 0;
    unsigned long int stop = 0;
    unsigned long int base = 0;
    unsigned char *buffer = NULL;
    char *fileName = NULL;
    char *password = NULL;
    size_t resultAmount = 0;
    size_t amount = 0;
    int result = EXIT_FAILURE;
    int ch;

    while ((ch = getopt(argc, argv, "a:b:hf:p:s:t:")) != -1) {
        switch(ch) {
        case 'a':
            amount = strtoul(optarg, NULL, 0);
            if (errno == ERANGE) {
                perror("[-] Unable to read amount");
                return EXIT_FAILURE;
            }
            printf("[*] Amount = %zd\n", amount);
            break;
        case 'b':
            base = strtoul(optarg, NULL, 0);
            if (errno == ERANGE) {
                perror("[-] Unable to read base offset");
                return EXIT_FAILURE;
            }
            printf("[*] Base offset = 0x%lX\n", base);
            break;
        case 'f':
            fileName = strdup(optarg);
            printf("[*] File = %s\n", fileName);
            break;
        case 'p':
            password = strdup(optarg);
            printf("[*] Password = %s\n", password);
            break;
        case 's':
            start = strtoul(optarg, NULL, 0);
            if (errno == ERANGE) {
                perror("[-] Unable to read start offset");
                return EXIT_FAILURE;
            }
            printf("[*] Start offset = 0x%lX\n", start);
            break;
        case 't':
            stop = strtoul(optarg, NULL, 0);
            if (errno == ERANGE) {
                perror("[-] Unable to read stop offset");
                return EXIT_FAILURE;
            }
            printf("[*] Stop offset = 0x%lX\n", stop);
            break;
        case 'h':
            help(argv[0]); 
            result = EXIT_SUCCESS;
            goto free_params;
        case '?':
        default:
            result = EXIT_FAILURE;
            goto free_params;
        }
    }

    /* Sanity checks */
    if (!start) {
        fprintf(stderr, "[-] Must provide start offset\n");
        result = EXIT_FAILURE;
        goto free_params;
    }
    if (fileName == NULL) {
        fprintf(stderr, "[-] Must provide file name\n");
        result = EXIT_FAILURE;
        goto free_params;
    }
    if (password == NULL) {
        fprintf(stderr, "[-] Must provide password\n");
        result = EXIT_FAILURE;
        goto free_params;
    }
    if (!stop && !amount) {
        fprintf(stderr, "[-] Must provide stop offset or amount\n");
        result = EXIT_FAILURE;
        goto free_params;
    }
    if (stop && (start >= stop)) {
        fprintf(stderr, "[-] Start offset must be lower than stop address\n");
        result = EXIT_FAILURE;
        goto free_params;
    }
    if (base && (base > start)) {
        fprintf(stderr, "[-] Base offset must be equal or lower than start address\n");
        result = EXIT_FAILURE;
        goto free_params;
    }

    /* Other checks*/
    if (stop && amount) {
        printf("[*] Provided stop offset and amont. Ignoring stop offset\n");
    }
    if (base){
        unsigned long int tmp = start;
        start = tmp - base;
        printf("[+] Relative start offset = 0x%lX\n", start);
        if(stop) {
            tmp = stop;
            stop = tmp - base;
            printf("[+] Relative stop offset = 0x%lX\n", stop);
        }
    }
    if (!amount) {
        amount = stop - start;
        printf("[+] Calculated amount = %zd\n", amount);
    }

    /* Alloc buffer */
    buffer = (unsigned char *) malloc(sizeof(unsigned char) * amount);
    if (buffer == NULL) {
        perror("[-] Unable allocate memmory");
        result = EXIT_FAILURE;
        goto free_params;
    }
    printf("[+] Buffer allocated\n");

    /* Open input file */
    f = fopen(fileName, "r+b");
    if (!f) {
        perror("[-] Unable to open the file");
        result = EXIT_FAILURE;
        goto free_params_and_buffer;
    }
    printf("[+] File opened\n");

    /* Positionate in the offset to read */
    if (fseek(f, start, SEEK_SET) == -1) {
        perror("[-] Unable to position in the file");
        result = EXIT_FAILURE;
        goto free_all_and_close_file;
    }
    printf("[+] In position to read\n");

    /* Read */
    resultAmount = fread(buffer, sizeof(char), amount, f);
    if(resultAmount != amount) {
        printf("[*] Amount readed = %zd\n", resultAmount);
        perror("[-] Unable to read all the data");
        result = EXIT_FAILURE;
        goto free_all_and_close_file;
    }
    printf("[+] Data read\n");

    decryptBlock(buffer, amount, password, strlen(password));
    printf("[+] Data encrypted\n");

    /* Positionate in the offset to write */
    if (fseek(f, start, SEEK_SET) == -1) {
        perror("[-] Unable to position in the file");
        result = EXIT_FAILURE;
        goto free_all_and_close_file;
    }
    printf("[+] In position to write\n");

    /* Write */
    resultAmount = fwrite(buffer, sizeof(char), amount, f);
    if(resultAmount != amount) {
        printf("[*] Amount writed = %zd\n", resultAmount);
        perror("[-] Unable to write all the data");
        result = EXIT_FAILURE;
        goto free_all_and_close_file;
    }
    printf("[+] Data wrote\n");

    result = EXIT_SUCCESS;

    /* Close file */
free_all_and_close_file:
    fclose(f);

    /* Free buffer */
free_params_and_buffer:
    free(buffer);

    /* Free params */
free_params:
    if (fileName != NULL) {
        free(fileName);
    }
    if (password != NULL) {
        memset_s(password, 0, strlen(password));
        free(password);
    }

    return result;
}
