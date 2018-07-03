
PREFIX?=/usr/local
BINDIR?=$(PREFIX)/bin

LIBS=

all: hangman

hangman:
	gcc Hangman.c HangmanMain.c -o hangman $(LIBS)

install: hangman
	mkdir -p $(DESTDIR)$(BINDIR)
	cp hangman $(DESTDIR)$(BINDIR)/
	chmod 755 ${DESTDIR}${BINDIR}/hangman
