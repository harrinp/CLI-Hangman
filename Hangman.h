#ifndef HANGMAN_H
#define HANGMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define IMGHEIGHT 17
#define IMGWIDTH 18
#define LOGOWIDTH 37
#define LOGOHEIGHT 6

//COLORS
#define NORM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
//FONT STUFF
#define BLD "\x1B[1m"
#define BLDOFF "\x1B[22m"
#define BLINK "\x1B[5m"
#define BLINKOFF "\x1B[25m"
#define UNDER "\x1B[4m"
#define UNDEROFF "\x1B[24m"
#define ITAL "\x1B[3m"
#define ITALOFF "\x1B[23m"
#define FAINT "\x1B[2m"
#define FAINTOFF "\x1B[22m"

typedef struct _Word {
    char * word;
    int place;
} Word;

typedef struct _Hangman {
    Word * words;
    int numWords;
    int numGuessed;
    char * solution;
    char charsGuessed[26];
} Hangman;

int getSpaces();
Hangman getWords(int);  // Takes int number of spaces, returns a Hangman Struct with all the possible words in it
bool printGuess(Hangman *, char, int, bool *);
int getPosIfRight(int, bool *);

char mostCommonCharInWord(Word *, Hangman *);
char mostCommonChar(Hangman *);
char mostCommonCharNew(Hangman *);
int makeGuess(Hangman *);

void alterList(Hangman *);
void alterForFailLetter(Hangman *, char);
void printList(Hangman *);
void destroyWord(Word *);
void destroyHangman(Hangman *);

void printGuy(int, char *, char);
void newPrinter(Hangman *, int, int, char);
void clearScreen();
#endif
