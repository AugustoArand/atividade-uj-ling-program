CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Isrc
TARGET = contatos
SRCDIR = src
UTILSDIR = $(SRCDIR)/utils
DATADIR = data
OBJS = $(SRCDIR)/main.o $(SRCDIR)/contato.o $(SRCDIR)/menu.o $(UTILSDIR)/string_utils.o $(UTILSDIR)/memory_utils.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(SRCDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/contato.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.c -o $(SRCDIR)/main.o

$(SRCDIR)/contato.o: $(SRCDIR)/contato.c $(SRCDIR)/contato.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/contato.c -o $(SRCDIR)/contato.o

$(SRCDIR)/menu.o: $(SRCDIR)/menu.c $(SRCDIR)/menu.h $(SRCDIR)/contato.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/menu.c -o $(SRCDIR)/menu.o

$(UTILSDIR)/string_utils.o: $(UTILSDIR)/string_utils.c $(UTILSDIR)/string_utils.h
	$(CC) $(CFLAGS) -c $(UTILSDIR)/string_utils.c -o $(UTILSDIR)/string_utils.o

$(UTILSDIR)/memory_utils.o: $(UTILSDIR)/memory_utils.c $(UTILSDIR)/memory_utils.h
	$(CC) $(CFLAGS) -c $(UTILSDIR)/memory_utils.c -o $(UTILSDIR)/memory_utils.o

clean:
	rm -f $(SRCDIR)/*.o $(UTILSDIR)/*.o $(TARGET) $(DATADIR)/*.bin $(DATADIR)/*.dat

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
