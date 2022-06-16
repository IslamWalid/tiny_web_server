CC = gcc
CFLAGS = -O2 -Wall -I .
LDFLAGS = -lpthread

all: tiny cgi

tiny.o: tiny.c 
	$(CC) $(CFLAGS) -c tiny.c 

sio.o: sio.c
	$(CC) $(CFLAGS) -c sio.c 

socket_interface.o: socket_interface.c
	$(CC) $(CFLAGS) -c socket_interface.c 

tiny: tiny.o sio.o socket_interface.o
	$(CC) $(CFLAGS) tiny.o sio.o socket_interface.o -o tiny $(LDFLAGS)

cgi:
	(cd cgi-bin; make)

clean:
	rm -f *.o tiny *~
	(cd cgi-bin; make clean)

