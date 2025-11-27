CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Isrc/contact -Isrc/storage -Isrc/utils -Isrc/main
TARGET = contatos
SRCDIR = src
DATADIR = data

# Source files
MAIN_SRC = $(SRCDIR)/main/main.c
CONTACT_SRC = $(SRCDIR)/contact/contact.c
STORAGE_SRC = $(SRCDIR)/storage/storage.c
UTILS_SRC = $(SRCDIR)/utils/utils.c

# Object files
OBJS = $(SRCDIR)/main/main.o $(SRCDIR)/contact/contact.o $(SRCDIR)/storage/storage.o $(SRCDIR)/utils/utils.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(SRCDIR)/main/main.o: $(MAIN_SRC) $(SRCDIR)/contact/contact.h $(SRCDIR)/storage/storage.h $(SRCDIR)/utils/utils.h
	$(CC) $(CFLAGS) -c $(MAIN_SRC) -o $(SRCDIR)/main/main.o

$(SRCDIR)/contact/contact.o: $(CONTACT_SRC) $(SRCDIR)/contact/contact.h
	$(CC) $(CFLAGS) -c $(CONTACT_SRC) -o $(SRCDIR)/contact/contact.o

$(SRCDIR)/storage/storage.o: $(STORAGE_SRC) $(SRCDIR)/storage/storage.h $(SRCDIR)/contact/contact.h
	$(CC) $(CFLAGS) -c $(STORAGE_SRC) -o $(SRCDIR)/storage/storage.o

$(SRCDIR)/utils/utils.o: $(UTILS_SRC) $(SRCDIR)/utils/utils.h
	$(CC) $(CFLAGS) -c $(UTILS_SRC) -o $(SRCDIR)/utils/utils.o

clean:
	rm -f $(SRCDIR)/main/*.o $(SRCDIR)/contact/*.o $(SRCDIR)/storage/*.o $(SRCDIR)/utils/*.o
	rm -f $(TARGET) $(DATADIR)/*.bin $(DATADIR)/*.dat $(DATADIR)/*.csv

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
