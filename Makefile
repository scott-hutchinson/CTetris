CFLAGS=-Wall
CC=gcc
SOURCES=main.c tetris.c buffer.c input.c terminal.c block.c coordinate.c renderer.c pixel.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=tetris

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
