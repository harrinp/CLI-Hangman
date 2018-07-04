
PREFIX?=/usr/local
BINDIR?=$(PREFIX)/bin
DATADIR?=$(PREFIX)/opt
LIBS=

all: hangman

hangman:
	gcc Hangman.c HangmanMain.c -o hangman $(LIBS)

install: hangman
	mkdir -p $(DESTDIR)$(BINDIR)
	cp hangman $(DESTDIR)$(BINDIR)/
	chmod 755 ${DESTDIR}${BINDIR}/hangman
	mkdir -p $(DESTDIR)$(DATADIR)/Hangman
	cp -R data $(DESTDIR)$(DATADIR)/Hangman/data
	cp -R art $(DESTDIR)$(DATADIR)/Hangman/art

clean:
	rm ${DESTDIR}${BINDIR}/hangman
	rm -rf $(DESTDIR)$(DATADIR)/Hangman
