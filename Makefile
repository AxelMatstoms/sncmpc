TARGET = sncmpc
LIBS = -lmpdclient $(shell ncursesw6-config --libs)
CC = gcc
CFLAGS = -Wall -O3 $(shell ncursesw6-config --cflags)


SECTION = 1
MANFN = $(TARGET).$(SECTION)
DOCS = doc/$(MANFN)
PREFIX = /usr
BINPREFIX = /bin
MANPREFIX = /share/man/man$(SECTION)

.PHONY: default all clean install uninstall

default: $(TARGET)
all: default $(DOCS).gz

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f src/*.o
	-rm -f $(TARGET)
	-rm -f $(DOCS).gz

debug: CFLAGS += -DDEBUG
debug: default

$(DOCS).gz: $(DOCS)
	gzip -fk $(DOCS)

install: $(TARGET) $(DOCS).gz
	install -d $(PREFIX)$(BINPREFIX)
	install -m 755 $(TARGET) $(PREFIX)$(BINPREFIX)/
	install -d $(PREFIX)$(MANPREFIX)
	install -m 644 $(DOCS).gz $(PREFIX)$(MANPREFIX)/

uninstall:
	rm -f $(PREFIX)$(BINPREFIX)/$(TARGET)
	rm -f $(PREFIX)$(MANPREFIX)/$(MANFN).gz
