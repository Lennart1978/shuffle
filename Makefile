# Projektname und Version
PROJECT = shuffle
VERSION = 1.2.0

# Compiler und Compiler-Flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -s
LDFLAGS = -s

# Quellen und Objekte
SOURCES = main.c shuffle.c
OBJECTS = $(SOURCES:.c=.o)

# Installationsverzeichnis
PREFIX = /usr

# Zielregel
all: $(PROJECT)

# Regel zum Erstellen des ausführbaren Programms
$(PROJECT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Regel zum Kompilieren der Objektdateien
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Installationsregel
install: $(PROJECT)
	install -d $(PREFIX)/bin
	install -m 755 $(PROJECT) $(PREFIX)/bin/

# Regel zum Bereinigen des Build-Verzeichnisses
clean:
	rm -f $(PROJECT) $(OBJECTS)

.PHONY: all install clean
