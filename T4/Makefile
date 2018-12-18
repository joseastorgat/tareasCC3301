CFLAGS= -g -Wall -pedantic -std=c99
LDFLAGS= -g -pthread

all: parking auto

auto.o parking.o libjsocket.o: util.h jsocket.h

auto: auto.o util.o libjsocket.o

parking: parking.o util.o libjsocket.o

clean:
	rm -f *.o
	rm -f parking auto
