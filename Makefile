CC ?= gcc
DESTDIR ?= ./product
includedir ?= /usr/include
libdir ?= /usr/lib

OUTPUT = libPatternLib.so

LIBFILES = clockController.c color.c DayPattern.c interface.c NightPattern.c TestPattern.c
INCLUDEFILES = interface.h

all: ${OUTPUT}
	env > environment.txt

${OUTPUT}: ${LIBFILES}
	${CC} -fPIC -shared ${LIBFILES} -o $@


install: ${OUTPUT}
	mkdir -p ${DESTDIR}/${includedir}/patternLib
	mkdir -p ${DESTDIR}/${libdir}
	install ${INCLUDEFILES} ${DESTDIR}/${includedir}/patternLib
	install ${OUTPUT} ${DESTDIR}/${libdir}

clean: ${OUTPUT}
	rm -rf $?

