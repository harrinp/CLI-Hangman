#include "Hangman.h"

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
    char words[60392][30];
    int numTries[60392] = {};
    int numFails[60392] = {};
    bool quitLoop = false;
    FILE * f = fopen("words3.txt", "r");

    while(!quitLoop){
        //printf("HERE\n");
        char s[30];
        int stringIndex = 0;
        if (countTotal > 60392 / 10){
            break;
        }
        while (1) {
            int c = fgetc(f);
            //printf("%c - %d\n", c, stringIndex);
            if (c == EOF){
                quitLoop = true;
                break;
            }
            else if (c == '\n'){
                s[stringIndex++] = '\0';    //Word being tested is null-terminated
                int spaces = stringIndex;
                int limbs = 0;
                bool quit = false;
                //printf("%s , %d\n",s, stringIndex);
                Hangman h = getWords(spaces);

                while(!quit){
                    char c = mostCommonChar(&h);
                    if (c == '\0'){
                        printf("I give up!\n");
                        quit = true;
                        break;
                    }
                    h.charsGuessed[h.numGuessed++] = c;
                    if (_checkForCharInWord(&h, s, c)){
                        limbs++;
                    }
                    else {
                        if (_checkIfNotSolved(&h, s)){
                            alterList(&h);
                        }
                        else {
                            numTries[countTotal] = h.numGuessed;
                            numFails[countTotal] = limbs;
                            if (limbs < 6){
                                countCorrect++;
                            }
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
    for (int i = 0; i < 60392 / 10; i++) {
        printf("%s - Tries: %d - Fails: %d - %s\n", numFails[i] < 6 ? "YES" : "NO", numTries[i], numFails[i], words[i]);
    }
    fclose(f);
    return 0;
}
