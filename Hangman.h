#ifndef HANGMAN_H
#define HANGMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

char mostCommonCharInWord(Word *, Hangman * h);
char mostCommonChar(Hangman *);

void alterList(Hangman *);
void printList(Hangman *);
void destroyWord(Word *);
void destroyHangman(Hangman *);
#endif
