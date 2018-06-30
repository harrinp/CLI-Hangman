#include "Hangman.h"

int main(int argc, char *argv[]) {

    int spaces = getSpaces();
    Hangman h = getWords(spaces);
    int limbs = 0;
    bool quit = false;
    while(!quit){
        printf("Current solution: %s \nGuesses: ", h.solution);
        for (int i = 0; i < h.numGuessed; i++) {
            printf("%c, ", h.charsGuessed[i]);
        }
        printf("\n");
        printf("Limbs: %d\n", limbs);
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
        bool fail = printGuess(&h, c, spaces, &quit);
        if (fail){
            limbs++;
        }
        printList(&h);
        printf("\n\n");
    }

    destroyHangman(&h);
    return 0;
}
