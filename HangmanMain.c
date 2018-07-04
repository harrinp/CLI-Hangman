#include "Hangman.h"

int main(int argc, char *argv[]) {
    clearScreen();
    int spaces = getSpaces();
    Hangman h = getWords(spaces);
    int limbs = 0;
    bool quit = false;

    while(!quit){

        printf("\n");
        int guessIndex = makeGuess(&h);
        if(guessIndex >= 0){
            printf("%sThe word is: %s%s\n", BLDOFF, BLU, h.words[guessIndex].word);
            break;
        }
        char c = mostCommonCharNew(&h);
        if (c == '\0'){
            printf("I give up! You win\n");
            quit = true;
            break;
        }
        else if (limbs == 6){
            quit = true;
        }
        clearScreen();
        newPrinter(&h, limbs, spaces, c);
        bool fail = false;
        if (!quit){
            fail = printGuess(&h, c, spaces, &quit);
        }
        if (fail){
            limbs++;
        }
    }

    destroyHangman(&h);
    return 0;
}
