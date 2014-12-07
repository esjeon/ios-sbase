# sbase version
VERSION = 0.0

# paths
PREFIX = /tmp/sbase
MANPREFIX = $(PREFIX)/share/man

SDK_PATH = ../sdk

CC = clang -target armv7-apple-darwin14.0.0
LD = $(CC) -mlinker-version=1 -B$(SDK_PATH)/usr/bin
AR = $(SDK_PATH)/usr/bin/ar
RANLIB = $(SDK_PATH)/usr/bin/ranlib

CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700
CFLAGS   = -std=c99 -Wall -pedantic
CFLAGS  += -I$(SDK_PATH)/usr/include

LDFLAGS  = 
LDFLAGS  += -L$(SDK_PATH)/usr/lib
LDFLAGS  += -L$(SDK_PATH)/usr/lib/system
