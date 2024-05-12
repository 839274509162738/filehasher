CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99
LDFLAGS = -lcrypto

hash.exe: hash.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

hash.o: hash.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f hash.exe hash.o
