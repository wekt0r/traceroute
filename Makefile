CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: main.o receiver.o sender.o printer.o
	$(CC) -o traceroute $^ $(CFLAGS)

%.o: %.c %.h constants.h
	$(CC) -c -o $@ $< $(CFLAGS)

main.o: main.c constants.h
	$(CC) -c -o main.o main.c $(CFLAGS)

clean:
	rm -rf *.o
distclean:
	rm -rf *.o traceroute
