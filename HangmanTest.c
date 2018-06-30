#include "Hangman.h"

#define NUMLINES 60392
#define DIVISOR 10


bool _checkForCharInWord(Hangman * h, char * word, char c){
    int i = 0;
    bool fail = true;

    while (1) {
        if (word[i] == '\0'){
            break;
        }
        else if (word[i] == c){
            fail = false;
            h->solution[i] = c;
        }
        i++;
    }
    if (fail){
        alterForFailLetter(h, c);
    }
    return fail;
}

bool _checkIfNotSolved(Hangman * h, char * word){
    int i = 0;
    bool fail = false;

    while (1) {
        if (word[i] == '\0'){
            break;
        }
        else if (word[i] != h->solution[i]){
            fail = true;
            break;
        }
        i++;
    }
    return fail;
}

int main(int argc, char *argv[]) {

    int countTotal = 0;
    int countCorrect = 0;
    char words[NUMLINES][30];
    int numTries[NUMLINES] = {};
    int numFails[NUMLINES] = {};
    bool quitLoop = false;
    FILE * f = fopen("words3.txt", "r");
    if (f == NULL){
        printf("ERROR: Bad file\n");
        return EXIT_FAILURE;
    }

    while(!quitLoop){
        //printf("HERE\n");
        char s[30];
        int stringIndex = 0;
        if (countTotal > NUMLINES / DIVISOR){
            break;
        }
        while (1) {
            int c = fgetc(f);
            //printf("%c - %d\n", c, stringIndex);
            if (c == EOF){
                quitLoop = true;
                break;
            }
            else if (c == '\n' || c == 13){
                s[stringIndex++] = '\0';    //Word being tested is null-terminated
                int spaces = stringIndex;
                int limbs = 0;
                bool quit = false;
                //printf("%s , %d\n",s, stringIndex);
                Hangman h = getWords(spaces);

                while(!quit){
                    char c = mostCommonCharNew(&h);
                    if (c == '\0'){
                        printf("I give up!\n");
                        quit = true;
                        numTries[countTotal] = h.numGuessed;
                        numFails[countTotal] = limbs;
                        //if (limbs < 6){
                        //    countCorrect++;
                        //}
                        break;
                    }
                    h.charsGuessed[h.numGuessed++] = c;
                    if (_checkForCharInWord(&h, s, c)){
                        limbs++;
                        alterList(&h);
                    }
                    else {
                        if (_checkIfNotSolved(&h, s)){
                            // nothing here
                        }
                        else {
                            numTries[countTotal] = h.numGuessed;
                            numFails[countTotal] = limbs;
                            //if (limbs < 6){
                                countCorrect++;
                            //}
                            break;
                        }
                    }
                }
                printf("%d : %s - %s -- %d/%d\n", countTotal, s, h.solution, limbs, h.numGuessed);
                strcpy(words[countTotal], s);
                countTotal++;
                destroyHangman(&h);
                break;
            }
            else {
                s[stringIndex++] = c;
            }
        }
    }
    for (int i = 0; i < NUMLINES / DIVISOR; i++) {
        printf("%s - Tries: %d - Fails: %d - %s\n", numFails[i] < 6 ? "YES" : "NO", numTries[i], numFails[i], words[i]);
    }
    printf("%d/%d\n", countCorrect, countTotal);
    fclose(f);
    return 0;
}
