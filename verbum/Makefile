# Copyright (c) 2013 Javier Escalada Gómez
# All rights reserved



################################################################################
# Program paths
################################################################################
CC = clang
AR = ar
RANLIB = ranlib

################################################################################
# Compilation options
################################################################################
CFLAGS = -std=c99
ifdef DEBUG
	CPPFLAGS = -DDEBUG
	CFLAGS += -Wall -Wreturn-type -O0 -g
else
	CPPFLAGS = -DNDEBUG
	CFLAGS +=  -O2
endif

################################################################################
# Project options
################################################################################
TOOLS_PATH = tools
CRYPT = $(TOOLS_PATH)/crypt
CRYPT_SOURCES = $(CRYPT).c
CRYPT_OBJS = $(CRYPT_SOURCES:.c=.o)
HASH = $(TOOLS_PATH)/hash
HASH_SOURCES = $(HASH).c
HASH_OBJS = $(HASH_SOURCES:.c=.o)


LIB_SOURCES = crypter.c hash_s.c hexify.c md5.c memset_s.c rc4.c
LIB_OBJS = $(LIB_SOURCES:.c=.o)
LIB = libverbum.a

################################################################################
# Internal options
################################################################################
DEPS = $(LIB_SOURCES:.c=.d) $(CRYPT_SOURCES:.c=.d) $(HASH_SOURCES:.c=.d)

################################################################################

.PHONY: all clean

all: $(LIB) $(CRYPT) $(HASH)

$(LIB): $(LIB_OBJS)
	ar rcs $@ $^
#	ranlib $@

$(CRYPT): $(CRYPT_OBJS) $(LIB)

$(HASH): $(HASH_OBJS) $(LIB)

# automatically generate dependency rules
%.d : %.c
# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency
	$(CC) $(CPPFLAGS) -MF "$@" -MG -MM -MP -MT "$@" -MT "$(<:.c=.o)" "$<"

-include $(DEPS)

clean:
	-$(RM) *.o
	-$(RM) $(TOOLS_PATH)/*.o
	-$(RM) $(LIB)
	-$(RM) $(CRYPT)
	-$(RM) $(HASH)
	-$(RM) $(DEPS)
