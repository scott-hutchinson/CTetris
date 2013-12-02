CFLAGS=-Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -g
CC=gcc
SOURCES=main.c tetris.c buffer.c input.c terminal.c block.c coordinate.c renderer.c pixel.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=tetris

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
