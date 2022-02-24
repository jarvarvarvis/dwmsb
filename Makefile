# dwmsb - dwm status bar

include config.mk

SRC = dwmsb.c
OBJ = ${SRC:.c=.o}

all: options dwmsb

options:
	@echo dwmsb build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC      = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk config.h

dwmsb: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dwmsb ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dwmsb ${DESTDIR}${PREFIX}/bin
	chmod +x ${DESTDIR}${PREFIX}/bin/dwmsb

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwmsb

.PHONY: all options clean install uninstall
