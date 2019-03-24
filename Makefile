CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: main.o receiver.o sender.o printer.o
	$(CC) -o traceroute $^ $(CFLAGS)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

# receiver.o: receiver.c receiver.h
# 	$(CC) -c -o $@ receiver.c $(CFLAGS)
#
# sender.o: sender.c sender.h
# 	$(CC) -c -o $@ sender.c $(CFLAGS)
#
# printer.o: printer.c printer.h
# 	$(CC) -c -o $@ printer.c $(CFLAGS)

main.o: main.c
	$(CC) -c -o main.o main.c $(CFLAGS)

clean:
	rm -rf *.o
distclean:
	rm -rf *.o traceroute
