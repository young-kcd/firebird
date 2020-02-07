#  The contents of this file are subject to the Initial
#  Developer's Public License Version 1.0 (the "License");
#  you may not use this file except in compliance with the
#  License. You may obtain a copy of the License at
#  https://www.firebirdsql.org/en/initial-developer-s-public-license-version-1-0/
#
#  Software distributed under the License is distributed AS IS,
#  WITHOUT WARRANTY OF ANY KIND, either express or implied.
#  See the License for the specific language governing rights
#  and limitations under the License.
#
#  The Original Code was created by Alexander Peshkoff
#  for the Firebird Open Source RDBMS project.
#
#  Copyright (c) 2020 Alexander Peshkoff <peshkoff@mail.ru>
#  and all contributors signed below.
#
#  All Rights Reserved.
#  Contributor(s): ______________________________________.

ROOT=../..
#ROOT=$(shell cd ../..; pwd)

PLUGINS=$(ROOT)/plugins
BIN=$(ROOT)/bin
LIB=$(ROOT)/lib

LIB_PREFIX=lib
SHRLIB_EXT=so

makePluginName=$(PLUGINS)/$(LIB_PREFIX)$(1).$(SHRLIB_EXT)

TOMCRYPT_COMPILE=-DLTC_PTHREAD -DUSE_LTM -DLTM_DESC
OwnInclude=$(shell [ -d tomcrypt.include ] && echo Yes || echo No)
ifeq ($(OwnInclude), Yes)
TOMCRYPT_COMPILE += -Itomcrypt.include
TOMCRYPT_LINK=-L$(LIB)/.tm
endif
BLD_SIMPLE_KEY_AUTH=libExtAuth.so
SIMPLE_KEY_AUTH=$(call makePluginName,ExtAuth)
BLD_KEYGEN=extauth_keygen
KEYGEN=$(BIN)/$(BLD_KEYGEN)

KEYGEN_objects=keygen.o
TCWRAP_objects=TcWrapper.o
KEY_AUTH_objects=ExtAuth.o

CXXFLAGS=-std=c++11 -pthread -I$(ROOT)/include -fPIC $(TOMCRYPT_COMPILE)
LDFLAGS=-pthread -L$(LIB) -Wl,-rpath,'$$ORIGIN/../lib' $(TOMCRYPT_LINK)

LINK=c++
LINK_LIBS=-lfbclient -ltomcrypt

.PHONY:		all keygen plugin install

all:		keygen plugin

keygen:		$(BLD_KEYGEN)

$(BLD_KEYGEN):	$(KEYGEN_objects) $(TCWRAP_objects)
	$(LINK) $(LDFLAGS) $^ -o $@ $(LINK_LIBS)

plugin:		$(BLD_SIMPLE_KEY_AUTH)

$(BLD_SIMPLE_KEY_AUTH):	$(KEY_AUTH_objects) $(TCWRAP_objects)
	$(LINK) -shared $(LDFLAGS) $^ -o $@ $(LINK_LIBS)

clean:
	rm -f *.o* $(BLD_KEYGEN) $(BLD_SIMPLE_KEY_AUTH)

install:	$(SIMPLE_KEY_AUTH) $(KEYGEN)

$(SIMPLE_KEY_AUTH): $(BLD_SIMPLE_KEY_AUTH)
	cp $^ $@

$(KEYGEN): $(BLD_KEYGEN)
	cp $^ $@

