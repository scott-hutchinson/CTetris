CFLAGS=-Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -std=gnu99 -g
CC=gcc
SOURCES=main.c tetris.c input.c block.c renderer.c buffer.c terminal.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=tetris

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
