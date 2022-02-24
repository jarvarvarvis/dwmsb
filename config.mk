# dwmsb version
VERSION = 0.1

# Customize below to fit your system

# Paths
PREFIX = /usr/local

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# Includes and Libs
INCS = -I${X11INC}
LIBS = -L${X11LIB} -lX11

# Flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L -DVERSION=\"${VERSION}\"
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

# Compiler and Linker
CC = cc
