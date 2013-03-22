# Verbum

Beta version

# Disclaimer
Programs and information are given for educational purpose only. 
I could be not responsable of what you will make with.

## Introduction
This project is based on the main idea that appears in [an article about how Gauss malware encrypts his payload](http://www.securelist.com/en/blog/208193781/The_Mystery_of_the_Encrypted_Gauss_Payload) ([more](http://arstechnica.com/security/2013/03/the-worlds-most-mysterious-potentially-destructive-malware-is-not-stuxnet/)). With Verbum, you can encrypt some desired parts of an executable file and decrypt them in an explicit way when it's necessary without storing the password inside the executable itself (obviously you have to provide the password in any way to the process when necessary). Verbum it's a proof of concept and is not intended to replace standard packers and crypters.

The design of Verbum tries to uncouple as much as possible the code related to the ciphering capabilities from the rest of the code.

## Usage
To include Verbum in your project you have to do the following steps:
1. Compile Verbum
2. Include  the scripts in the configuration of the target project.
3. Include your code to decrypt the payload.
4. Create an empty Verbum configuration file. Due to the way in wich Verbum works ([x-macros](http://en.wikipedia.org/wiki/X_Macro)), its necessary to generate a line for each target block with "dummy" addresses, to fill them later.

```
$ cat verbum.conf
# Format:
#     symbol:start_addrs:stop_addrs:passwd
#
# Addresses must be absolute
# start_addrs must be greater than stop_addrs

# Foo module
foo:0x0000000100000000:0x0000000100000001:gauss
# Bar module
bar:0x0000000100000000:0x0000000100000001:gauss
```

5. Compile your project with debug symbols and without crypt.

```
make NO_STRIP=1 NO_CRYPT=1
```

6. Examine and extract the addresses of the blocks you want to protect.

```
$ nm -g -n  -a example
                 U ___bzero
                 U ___stack_chk_fail
                 U ___stack_chk_guard
                 U ___stderrp
                 U _exit
                 U _free
                 U _fwrite
                 U _getpagesize
                 U _malloc
                 U _memcpy
                 U _mprotect
                 U _perror
                 U _printf
                 U _puts
                 U _signal
                 U _strcpy
                 U _strlen
                 U _strncasecmp
                 U dyld_stub_binder
0000000100000000 T __mh_execute_header
0000000100001520 T start
0000000100001560 T _decryptPayload
0000000100001720 T _main
00000001000018a0 T _foo
0000000100001910 T _bar
0000000100001930 T _encryptBlock
00000001000019a0 T _hash_s
0000000100001ae0 T _hexify
0000000100001b50 T _MD5_Init
0000000100002390 T _MD5_Final
00000001000024d0 T _MD5_Update
0000000100002590 T _memset_s
00000001000025b0 T _RC4_init
0000000100002680 T _RC4_output
00000001000030c8 D _prefix
00000001000030d0 D _suffix
00000001000030e0 S _NXArgc
00000001000030e8 S _NXArgv
00000001000030f0 S _environ
00000001000030f8 S ___progname
0000000100003100 S _txt
```

7. Update the addresses in the configuration file

```
$ cat verbum.conf
# Format:
#     symbol:start_addrss:stop_addrss:passwd

# Foo module
foo:0x00000001000018a0:0x0000000100001910:gauss
# Bar module
bar:0x0000000100001910:0x0000000100001930:gauss
```

7. Clean all the automatically generated files.
8. Compile again the target project to generate the crypted executables.
9. You can strip the executable.
10. Test the final executable. The first time (when you fill the configuration with dummy addresses) could fail and don't decrypt correctly the payloads. This is caused due to some issues related with x-macros. To solve this, jump again to the step 5 and update again the addresses. In one or two iterations, the problem should be solved.

For more information, see the example project.

# Configuration
Inside Verbum folder you can find `config.h`:

```c
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
```

You can also change some parameter on Makefiles. By default is used Clang instead of GCC.

## Todo
- Windows port.
- Improve documentation.
- Use in big project to test stability.
- Search for a better and automatic way to generate the configuration file.

## Contribute
Feel free to help me to improve this project in any way.

## License
Copyright (c) 2013 Javier Escalada Gómez
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of copyright holders nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
