#include "Hangman.h"

#define WORDFILE "words2.txt"

void _printLogo(){
    printf("%c%s    __  __                                      \n   / / / /___ _____  ____ _____ ___  ____ _____ \n  / /_/ / __ `/ __ \\/ __ `/ __ `__ \\/ __ `/ __ \\\n / __  / /_/ / / / / /_/ / / / / / / /_/ / / / /\n/_/ /_/\\__,_/_/ /_/\\__, /_/ /_/ /_/\\__,_/_/ /_/ \n                  /____/                        %s\n  .__.\n",8, RED,NORM);
}

int getSpaces(){

    char input[2]= "";
    int count = 0;
    bool correctInput = false;
    int num = 0;
    _printLogo();
    while(!correctInput){
        count = 0;
        printf("%c %s%sO%s%s|  | %sWelcome to Hangman!%s\n-|-  | When entering values do it like this: %s2,5,6%s\n/|   | Please enter the number of letters in your word: ",8, UNDER, ITAL, ITALOFF, UNDEROFF, BLD, BLDOFF, BLINK, BLINKOFF);
        while (1){
            if (count > 3){
                printf("Too Long - Error try again\n");
                break;
            }
            int c = getchar();
            if (c == EOF || c == '\n'){
                correctInput = true;
                break;
            }
            else if (c > 47 && c < 58){
                input[count++] = c;
            }
            else {
                printf("Unknown - Error try again\n");
                break;
            }
        }
        num = atoi(input);
        if (num < 1 || num > 20){
            correctInput = false;
            printf("Bad Length - Error try again\n");
        }
    }
    return num;
}

int _getNum(int numSpaces){
    FILE * f = fopen(WORDFILE, "r");
    bool endOfFile = false;
    int numMatch = 0;
    while (!endOfFile){
        int count = 0;
        while(1){
            int c = fgetc(f);
            if (c == EOF){
                if (count == numSpaces){
                    numMatch++;
                }
                endOfFile = true;
                break;
            }
            else if ((c == '\n' || c == 13) && count == numSpaces){
                numMatch++;
                if (c == 13){
                    fgetc(f);
                }
                break;
            }
            else if (c == '\n'){
                break;
            }
            else{
                count++;
            }
        }
    }
    endOfFile = false;
    printf("%d\n", numMatch);
    fclose(f);
    return numMatch;
}

Hangman getWords(int numSpaces){
    FILE * f = fopen(WORDFILE, "r");
    int numMatch = _getNum(numSpaces);
    bool endOfFile = false;

    Hangman h = {
        malloc(sizeof(Word) * numMatch),
        numMatch,
        0,
        malloc((sizeof(char) * numSpaces) + 1),
    };
    for (int i = 0; i < numSpaces; i++) {
        h.solution[i] = '_';
    }
    h.solution[numSpaces] = '\0';
    int placement = 0;
    int wordIndex = 0;
    fseek(f, SEEK_SET, 0);
    while (!endOfFile){
        placement++;
        int count = 0;
        char string[35] = "";
        while(1){
            int c = fgetc(f);
            if (c == EOF){
                endOfFile = true;
                break;
            }
            else if ((c == '\n' || c == 13) && count == numSpaces){
                //printf("int: %d/%d    Word : %s     place : %d -- %d\n", wordIndex, numMatch, string, placement, count);
                h.words[wordIndex].word = malloc((sizeof(char) * numSpaces) + 1);
                h.words[wordIndex].place = placement;
                for (int i = 0; i < numSpaces; i ++){
                    h.words[wordIndex].word[i] = string[i];
                }
                h.words[wordIndex].word[numSpaces] = '\0';
                wordIndex++;
                if (c == 13){
                    fgetc(f);
                }
                break;
            }
            else if (c == '\n'){
                // printf("%s\n", string);
                break;
            }
            else{
                string[count++] = c;
            }
        }
    }
    fclose(f);
    return h;
}

void destroyWord(Word * w){
    free(w->word);
    w->place = -1;
}

void destroyHangman(Hangman * h){
    for (int i = 0; i < h->numWords; i++) {
        if (h->words[i].place != -1){
            destroyWord(&(h->words[i]));
        }
    }
    free(h->solution);
    free(h->words);
}

bool printGuess(Hangman * h, char c, int numSpaces, bool * quit){
    h->charsGuessed[h->numGuessed++] = c;
    //printf("GUESS -->: %c\n", c);
    //printf("Correct?\n");
    int posArr[25] = {};
    int counter = 0;
    bool failure = false;
    bool cont = true;
    while (counter < 25 && cont){
        posArr[counter] = getPosIfRight(numSpaces, &cont);
        if (posArr[counter] == -1){
            failure = true;
            printf("Failed\n");
            break;
        }
        else if (posArr[counter] == -2){
            printf("Goodbye.");
            *quit = true;
            break;
        }
        counter++;
    }
    if (!failure){
        //printf("Changing solution\n");
        for (int i = 0; i < counter; i++) {
            h->solution[posArr[i]] = c;
        }
        alterList(h);
    }
    else {
        alterForFailLetter(h, c);
    }
    return failure;
}
int getPosIfRight(int numSpaces, bool * end){
    bool charFailed = false;
    int num = 0;
    char input[2];
    int count = 0;
    bool quit = false;
    while (1){
        int c = getchar();
        if (c == ','){
            if (!charFailed){
                num = atoi(input);
            }
            break;
        }
        else if (c == EOF || c == '\n'){
            if (!charFailed){
                num = atoi(input);
                if (num > numSpaces || num < 1){
                    charFailed = true;
                }
            }
            (*end) = false;
            break;
        }
        else if (c > 47 && c < 58){
            input[count++] = c;
        }
        else if (c == 'q'){
            quit = true;
        }
        else if (c != 32){
            charFailed = true;
        }
    }
    //printf("num: %d\n", charFailed ? -1 : num - 1);
    return charFailed ? -1 : quit ? -2 : num - 1;

}

bool _checkWord(char * sol, char * word){   // Checks if a word in the words list is still valid with the current working solution
    int i = 0;
    bool same = true;
    while (sol[i] != '\0' && word[i] != '\0'){
        if (sol[i] != '_' && sol[i] != word[i]){
            same = false;
            break;
        }
        i++;
    }
    return same;
}

void alterList(Hangman * h){
    for (int i = 0; i < h->numWords; i++) {
        if (h->words[i].word != NULL && h->words[i].place != -1){   //Check to make sure its not gone
            if (!_checkWord(h->solution, h->words[i].word)){
                destroyWord(&h->words[i]);
            }
        }
    }
}

bool _charNotGuessed(char c, Hangman * h){
    bool notGuessed = true;
    for(int i = 0; i < h->numGuessed; i++){
        if (h->charsGuessed[i] == c){
            notGuessed = false;
        }
    }
    return notGuessed;
}

bool _checkWordForNoBadChar(char * word, char fail){   // Checks if a word in the words list is still valid with the current working solution
    int i = 0;
    bool good = true;
    while (word[i] != '\0'){
        if (word[i] == fail){
            good = false;
            break;
        }
        i++;
    }
    return good;
}

void alterForFailLetter(Hangman * h, char fail){
    for (int i = 0; i < h->numWords; i++) {
        if (h->words[i].word != NULL && h->words[i].place != -1){   //Check to make sure its not gone
            if (!_checkWordForNoBadChar(h->words[i].word, fail)) {
                destroyWord(&(h->words[i]));
            }
        }
    }
}

char mostCommonCharInWord(Word * w, Hangman * h){
    int charFreq[26] = {0};
    int count = 0;
    while (w->word[count] != '\0'){
        //printf("%c",w->word[count] );
        charFreq[w->word[count++]-97]++;
    }
    int highChar = 0;
    int highestAmount = 0;
    for (int i = 0; i < 26; i++) {
        if(charFreq[i] > highestAmount && _charNotGuessed(i + 97, h)){
            highChar = i;
            highestAmount = charFreq[i];
        }
    }
    //printf(" - %c\n", highChar + 97);
    return highChar + 97;
}

char mostCommonChar(Hangman * h){
    int charFreq[26] = {};
    for (int i = 0; i < h->numWords; i++) {
        if (h->words[i].word != NULL && h->words[i].place != -1){   //Check to make sure its not gone
            charFreq[ mostCommonCharInWord( &(h->words[i]), h) - 97 ]++;
        }
    }
    for (int i = 0; i < 26; i++) {
        //printf("%c-%d,\n", 97+i, charFreq[i]);
    }
    int highChar = 0;
    int highestAmount = 0;
    for (int i = 0; i < 26; i++) {
        if(charFreq[i] > highestAmount && _charNotGuessed(i + 97, h)){
            highChar = i;
            highestAmount = charFreq[i];
        }
    }
    if (!_charNotGuessed(highChar + 97, h)){
        return '\0';
    }
    return highChar + 97;
}

char mostCommonCharNew(Hangman * h){
    int charFreq[26] = {};
    for (int i = 0; i < h->numWords; i++) {
        if (h->words[i].word != NULL && h->words[i].place != -1){   //Check to make sure its not gone
            int count = 0;
            while (h->words[i].word[count] != '\0'){
                //printf("%c",w->word[count] );
                int index = h->words[i].word[count++]-97;
                int addTotal = 10 * (300000 - h->words[i].place) / 300000;
                charFreq[index] += addTotal;
            }
        }
    }
    // for (int i = 0; i < 26; i++) {
    //     printf("%c-%d,\n", 97+i, charFreq[i]);
    // }
    int highChar = 0;
    int highestAmount = 0;
    for (int i = 0; i < 26; i++) {
        if(charFreq[i] > highestAmount && _charNotGuessed(i + 97, h)){
            highChar = i;
            highestAmount = charFreq[i];
        }
    }
    if (!_charNotGuessed(highChar + 97, h)){
        return '\0';
    }
    return highChar + 97;
}

void printList(Hangman * h){
    int count = 0;
    for (int i = 0; i < h->numWords; i++) {
        if (h->words[i].word != NULL && h->words[i].place != -1){   //Check to make sure its not gone
            //printf("%s .. %d\n", h->words[i].word, 100 * (300000 - h->words[i].place) / 300000);
            count++;
        }
    }
    printf("Possible word count: %d\n", count );
}

int makeGuess(Hangman * h){
    int count = 0;
    int index = -1;
    for (int i = 0; i < h->numWords; i++) {
        if (h->words[i].word != NULL && h->words[i].place != -1){   //Check to make sure its not gone
            count++;
            index = i;
        }
    }
    if(count == 1){
        return index;
    }
    else {
        return -1;
    }
}

void printGuy(int limbs, char * solution, char guess){

    char * manArr[7] = {
    "     (+)========II     __  __\n      |         ||    / / / /___ _____  ____ _____ ___  ____ _____\n      |         ||   / /_/ / __ `/ __ \\/ __ `/ __ `__ \\/ __ `/ __ \\\n      |         ||  / __  / /_/ / / / / /_/ / / / / / / /_/ / / / /\n      |         || /_/ /_/\\__,_/_/ /_/\\__, /_/ /_/ /_/\\__,_/_/ /_/ \n      |         ||                   /____/\n     _|_        ||\n    /   \\       ||\n   |     |      ||\n    \\ _ /       ||\n                ||\n                ||\n                ||",
    "     (+)========II     __  __\n       \\        ||    / / / /___ _____  ____ _____ ___  ____ _____\n        \\       ||   / /_/ / __ `/ __ \\/ __ `/ __ `__ \\/ __ `/ __ \\\n         \\      ||  / __  / /_/ / / / / /_/ / / / / / / /_/ / / / /\n    .---. |     || /_/ /_/\\__,_/_/ /_/\\__, /_/ /_/ /_/\\__,_/_/ /_/ \n    | 00| |     ||                   /____/\n    | - | |     ||\n    '-.-'/      ||\n     ---<       ||\n                ||\n                ||\n                ||\n                ||",
    "     (+)========II     __  __\n       \\        ||    / / / /___ _____  ____ _____ ___  ____ _____\n        \\       ||   / /_/ / __ `/ __ \\/ __ `/ __ `__ \\/ __ `/ __ \\\n         \\      ||  / __  / /_/ / / / / /_/ / / / / / / /_/ / / / /\n    .---. |     || /_/ /_/\\__,_/_/ /_/\\__, /_/ /_/ /_/\\__,_/_/ /_/ \n    | 00| |     ||                   /____/\n    | - | |     ||\n    '-.-'/      ||\n     ---<       ||\n     |*|        ||\n     |*|        ||\n     |_|        ||\n                ||",
    "     (+)========II     __  __\n       \\        ||    / / / /___ _____  ____ _____ ___  ____ _____\n        \\       ||   / /_/ / __ `/ __ \\/ __ `/ __ `__ \\/ __ `/ __ \\\n         \\      ||  / __  / /_/ / / / / /_/ / / / / / / /_/ / / / /\n    .---. |     || /_/ /_/\\__,_/_/ /_/\\__, /_/ /_/ /_/\\__,_/_/ /_/ \n    | 00| |     ||                   /____/\n    | - | |     ||\n    '-.-'/      ||\n     ---<       ||\n   //|*|        ||\n  // |*|        ||\n[_]  |_|        ||\n                ||",
    "     (+)========II     __  __\n       \\        ||    / / / /___ _____  ____ _____ ___  ____ _____\n        \\       ||   / /_/ / __ `/ __ \\/ __ `/ __ `__ \\/ __ `/ __ \\\n         \\      ||  / __  / /_/ / / / / /_/ / / / / / / /_/ / / / /\n    .---. |     || /_/ /_/\\__,_/_/ /_/\\__, /_/ /_/ /_/\\__,_/_/ /_/ \n    | 00| |     ||                   /____/\n    | - | |     ||\n    '-.-'/      ||\n     ---<       ||\n   //|*|\\\\      ||\n  // |*| \\\\     ||\n[_]  |_|  [_]   ||\n                ||",
    "     (+)========II     __  __\n       \\        ||    / / / /___ _____  ____ _____ ___  ____ _____\n        \\       ||   / /_/ / __ `/ __ \\/ __ `/ __ `__ \\/ __ `/ __ \\\n         \\      ||  / __  / /_/ / / / / /_/ / / / / / / /_/ / / / /\n    .---. |     || /_/ /_/\\__,_/_/ /_/\\__, /_/ /_/ /_/\\__,_/_/ /_/ \n    | 00| |     ||                   /____/\n    | - | |     ||\n    '-.-'/      ||\n     ---<       ||\n   //|*|\\\\      ||\n  // |*| \\\\     ||\n[_]  |_|  [_]   ||\n    / /         ||",
    "     (+)========II     __  __\n       \\        ||    / / / /___ _____  ____ _____ ___  ____ _____\n        \\       ||   / /_/ / __ `/ __ \\/ __ `/ __ `__ \\/ __ `/ __ \\\n         \\      ||  / __  / /_/ / / / / /_/ / / / / / / /_/ / / / /\n    .---. |     || /_/ /_/\\__,_/_/ /_/\\__, /_/ /_/ /_/\\__,_/_/ /_/ \n    | XX| |     ||                   /____/\n    | - | |     ||\n    '-.-'/      ||\n     ---<       ||\n   //|*|\\\\      ||\n  // |*| \\\\     ||\n[_]  |_|  [_]   ||\n    / | \\       ||" };

    char * manArrFeet[7] = {"                ||","                ||","                ||","                ||","                ||",  "  [_]           ||","  [_]   [_]     ||"};
    char * manArrLegs[7] = {"                ||", "                ||", "                ||", "                ||", "                ||", "   /_/          ||", "   /_/ \\_\\      ||"};

    printf("%c%s  Letters: %s\n", 8,  manArr[limbs], solution);
    printf("%s\n", manArrLegs[limbs]);
    printf("%s  Guess  : %c\n", manArrFeet[limbs], guess);

    printf("                ||\n");
    printf("****************II  %s ", limbs != 6 ? "Input  :" : "I lose...");
}

void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void _readAndPrint(FILE * f, bool toEndOfLine, int numChars){
    if (toEndOfLine){
        while(1){
            int c = fgetc(f);
            if (c == '\n' || c == EOF){
                printf("\n");
                break;
            }
            printf("%c",c);
        }
    }
    else {
        for (int i = 0; i < numChars; i++) {
            int c = fgetc(f);
            if (c == '\n' || c == EOF){
                printf("\n");
                break;
            }
            printf("%c",c);
        }
    }
}

void _quickGetEnd(FILE * f){
    _readAndPrint(f, true, 0);
}

void newPrinter(Hangman * h, int limbs, int spaces, char guess){
    FILE * f = fopen("images", "r");
    // Go through all lines until the image needed
    for (int i = 0; i < limbs * IMGHEIGHT; i++) {
        while(1){
            int c = fgetc(f);
            if (c == '\n'){
                break;
            }
        }
    }
    printf("%s", BLDOFF);
    for (int i = 0; i < 8; i++) {
        printf("  ");
        _readAndPrint(f, false, IMGWIDTH);
        printf("%s", RED);
        _quickGetEnd(f);
        printf("%s", NORM);
    }

    printf("  ");
    _readAndPrint(f, false, IMGWIDTH);
    printf("  Guesses : ");
    for (int i = 0; i < h->numGuessed; i++) {
        printf("%c, ", h->charsGuessed[i]);
    }
    _quickGetEnd(f);

    for (int i = 0; i < 2; i++) {
        printf("  ");
        _quickGetEnd(f);
    }

    printf("  ");
    _readAndPrint(f, false, IMGWIDTH);
    printf("  Letters: %s", h->solution);
    _quickGetEnd(f);

    printf("  ");
    _readAndPrint(f, false, IMGWIDTH);
    printf("           %s", FAINT);
    for (int i = 1; i <= spaces; i++) {
        if (i % 2 == 0){
            printf("%d", i);
        }
        else {
            printf(" ");
        }
    }
    printf("%s", FAINTOFF);
    _quickGetEnd(f);

    printf("  ");
    _readAndPrint(f, true, 0);

    printf("  ");
    _readAndPrint(f, false, IMGWIDTH);
    printf("  Guess  : %c", guess);
    _quickGetEnd(f);

    printf("  ");
    _readAndPrint(f, true, IMGWIDTH);

    printf("  ");
    _readAndPrint(f, false, IMGWIDTH);
    printf("  Input  : %s", BLD);

    fclose(f);
}
