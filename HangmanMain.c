#include "Hangman.h"

int main(int argc, char *argv[]) {
    clearScreen();
    int spaces = getSpaces();
    Hangman h = getWords(spaces);
    int limbs = 0;
    bool quit = false;

    while(!quit){

        for (int i = 0; i < h.numGuessed; i++) {
            printf("%c, ", h.charsGuessed[i]);
        }
        printf("\n");
        int guessIndex = makeGuess(&h);
        if(guessIndex >= 0){
            printf("The word is: %s\n", h.words[guessIndex].word);
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
        printGuy(limbs, h.solution,c);
        bool fail = false;
        if (!quit){
            fail = printGuess(&h, c, spaces, &quit);
        }
        if (fail){
            limbs++;
        }
        //printList(&h);
    }

    destroyHangman(&h);
    return 0;
}
