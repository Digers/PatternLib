CC ?= gcc

OUTPUT = libPatternLib.o

LIBFILES = clockController.c color.c DayPattern.c interface.c NightPattern.c TestPattern.c

all:
	${CC} -fPIC -shared ${LIBFILES} -o libPatternLib.o

clean:
	rm -rf ${OUTPUT}

