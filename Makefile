# Copyright (c) 2013 Javier Escalada Gómez
# All rights reserved



################################################################################
# Program paths
################################################################################
CC = clang
STRIP = strip
MAKE = make

################################################################################
# Compilation options
################################################################################
CFLAGS = #-std=c99
ifdef DEBUG
	CPPFLAGS = -DDEBUG
	CFLAGS += -Wall -Wreturn-type -O0 -g
else
	CPPFLAGS = -DNDEBUG
	CFLAGS +=  -O2
endif

################################################################################
# Verbum options
################################################################################
VERBUM_PATH = verbum

VERBUM_LIB = $(VERBUM_PATH)/libverbum.a

VERBUM_HASHES_SCRIPT = $(VERBUM_PATH)/scripts/unix/hash_passwds.sh
VERBUM_CRYPT_SCRIPT = $(VERBUM_PATH)/scripts/unix/crypt_payloads.sh

################################################################################
# Project options
################################################################################
EXAMPLE = example

EXAMPLE_SOURCES = $(EXAMPLE).c
EXAMPLE_OBJS = $(EXAMPLE_SOURCES:.c=.o)

VERBUM_DEF = verbum.def
VERBUM_CONFIG = verbum.conf

PAYLOADS_SOURCES = foo.c bar.c
PAYLOADS_OBJS = $(PAYLOADS_SOURCES:.c=.o)

################################################################################
# Internal options
################################################################################
DEPS = $(EXAMPLE_SOURCES:.c=.d) $(PAYLOADS_SOURCES:.c=.d)

################################################################################

.PHONY: all check clean symbols

all: $(EXAMPLE)

$(VERBUM_HASHES_SCRIPT) $(VERBUM_CRYPT_SCRIPT): $(VERBUM_LIB)

$(EXAMPLE):: $(EXAMPLE_OBJS) $(PAYLOADS_OBJS) $(VERBUM_LIB)

$(EXAMPLE):: $(VERBUM_CONFIG)
ifndef NO_CRYPT
	$(VERBUM_CRYPT_SCRIPT) $< $@
else
	@true # This is to avoid the default behaviour of the target (link)
endif

$(EXAMPLE)::
ifndef NO_STRIP
	$(STRIP) $@
else
	@true # This is to avoid the default behaviour of the target (compile & link)
endif

$(EXAMPLE_OBJS): $(EXAMPLE_SOURCES)

$(EXAMPLE_SOURCES): $(VERBUM_DEF)

$(VERBUM_DEF): $(VERBUM_CONFIG)
	$(VERBUM_HASHES_SCRIPT) $< $@

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
	-$(RM) $(EXAMPLE)
	-$(RM) $(VERBUM_DEF)
	-$(RM) $(DEPS)

symbols:
	$(MAKE) clean
	$(MAKE) NO_STRIP=1 NO_CRYPT=1
	@clear
	@nm -g -n -a $(EXAMPLE) | grep -v '^ ' | nl
	@read -p "Press ENTER to continue"
	$(MAKE) clean
