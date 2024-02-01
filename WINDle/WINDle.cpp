#include <iostream>
#include <string>
#include <conio.h>
#include <stdlib.h>
#include <cstdlib>
#include <windows.h>
#include <unistd.h>
#include "base64.h"

int wordLength = 5;

// Windows console color codes
    int gray = 15;
    int yellow = 96;
    int green = 160; // or 47

// returns true if the entire string is only alpha characters (a-z & A-Z)
boolean stringIsAlpha(std::string s){
    for(int i = 0; i < s.length(); i++){
        if(!isalpha(s.at(i))){
            return false;
        }
    }
    return true;
}

// checks the results of a given guess against the correct word and stores the corresponding colors in the results[] array
void checkLetters(std::string word, std::string guess, int results[]){
    // represents the color that each letter should be drawn with in the console
    int gray = 0;
    int yellow = 1;
    int green = 2;

    // check correct letters (correct placements) - green
    for(int i = 0; i < word.length(); i++){
        if(word.at(i) == guess.at(i)){
            results[i] = green;
            // mark letters as checked
            word[i] = '-';
            guess[i] = '-';
        }
    }

    // check correct letters (incorrect placements) - yellow
    for(int i = 0; i < guess.length(); i++){
        // if letter has already been checked, move on
        if(guess.at(i) == '-'){
            continue;
        }
        // compare each letter in the guess to the correct letters in the actual word
        for(int j = 0; j < word.length(); j++){
            // this check isn't really that necessary since correct placements have already been checked, but I like having it here
            if(i == j){
                continue;
            }

            // if the word contains the letter in the guess
            if(word.at(j) == guess.at(i)){
                results[i] = yellow;
                // mark letters as checked
                word[j] = '-';
                guess[i] = '-';
            }
        }
    }
}

// takes a string and returns a new string with all the alpha characters as uppercase
std::string toUpper(std::string original){
    std::string upper;

    for(int i = 0; i < original.length(); i++){
        // ignore chars that aren't letters
        if(!isalpha(original.at(i))){
            continue;
        }
        // if the letter is lowercase
        if(original.at(i) >= 'a'){
            // get the number as it corresponds to the alphabet:
            // 'a' = 0, 'b' = 1, etc.
            // then add 'A' to put it in the lowercase letter range
            upper += (original.at(i) - 'a' + 'A');
            // NOTE: we could simply subtract 32 from each lowercase letter, but this is obtuse and is not clear why it works if you do not have an understanding of ASCII codes
        }else{
            // if the letter is already uppercase, add it and move on
            upper += original.at(i);
        }
    }
    return upper;
}

// takes a string and returns a new string with all the alpha characters as lowercase
std::string toLower(std::string original){
    std::string lower;

    for(int i = 0; i < original.length(); i++){
        // ignore chars that aren't letters
        if(!isalpha(original.at(i))){
            continue;
        }
        // if the letter is uppercase
        if(original.at(i) < 'a'){
            // get the number as it corresponds to the alphabet:
            // 'A' = 0, 'B' = 1, etc.
            // then add 'a' to put it in the lowercase letter range
            lower += (original.at(i) - 'A' + 'a');
            // NOTE: we could simply add 32 to each uppercase letter, but this is obtuse and is not clear why it works if you do not have an understanding of ASCII codes
        }else{
            // if the letter is already lowercase, add it and move on
            lower += original.at(i);
        }
    }
    return lower;
}

// get a base64 string and decode it back into ASCII
std::string getWord(){
    std::cout << "\nEnter the WINDle code:\n";
    std::string code;
    getline(std::cin,code,'\n');

    // decode the base64 string
    std::string word = decode(code);
    return word;
}

// begins a game of WINDle from a pre-generated base64 code
void playGame(){
    // get the word from the user and capitalize it
    std::string word = getWord();
    word = toUpper(word);

    // if the word is not the correct length or is not only alpha, the code is invalid
    if(word.length() != wordLength || !stringIsAlpha(word)){
        std::cout << "You must enter a valid code\n";
        return;
    }

    // clear the screen to begin the game
    std::cout << "\033[H\033[2J";

    // controls the text color in the console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // user has 6 chances to guess the correct word
    for(int round = 1; round <= 6;){
        std::cout << round << "/6: ";

        // get a guess from the user and capitalize it
        std::string guess;
        getline(std::cin,guess,'\n');
        guess = toUpper(guess);

        // if the guess is invalid, restart the round and get another guess from the user
        if(guess.length() != wordLength || !stringIsAlpha(guess)){
            std::cout << "You must enter a " << wordLength << "-letter word\n";
            continue;
        }

        int colors[3] = {gray, yellow, green};

        // stores the color of each letter based on how close it is to the actual word
        int guessResults[guess.length()] = {0};

        /* populate the array with the correct color indicators:
         *
         * gray     = 0 (wrong letter)
         * yellow   = 1 (right letter, wrong spot)
         * green    = 2 (right letter, right spot)
         *
         * these correspond to the indices of the colors[] array
         */
        checkLetters(word, guess, guessResults);

        // display each letter using the corresponding color
        for(int i = 0; i < guess.length(); i++){
            // set the console text color
            SetConsoleTextAttribute(hConsole, colors[guessResults[i]]);
            // print out each letter individually
            std::cout << guess[i];
            // add a delay between each letter (to mimic how wordle displays each letter and add some suspense)
            Sleep(500); // 500 ms -- 750 is more suspenseful, but becomes agonizing during testing...
        }

        // reset the text color back to black and white
        SetConsoleTextAttribute(hConsole, colors[0]);
        std::cout << "\n\n";
        round++;

        // if the user correctly guessed the word, end the game
        if(word == guess){
            std::cout << "You did it!" << "\n\n";
            return;
        }
    }

    // if the loop ends, the user did not correctly guess the word
    std::cout << "The word was: " << word << "\n\n";
}

// takes a word of a specific, predefined length and prints out the corresponding base64 encoding
void generateCode(){
    std::cout << "\nEnter a " << wordLength << "-letter word:\n";
    std::string word;
    getline(std::cin,word,'\n');

    // make sure the user entered a valid string
    if(word.length() != wordLength){
        std::cout << "\nYou must enter a " << wordLength << "-letter word\n\n";
        return;
    }

    if(!stringIsAlpha(word)){
        std::cout << "\nYou can only enter letters\n\n";
        return;
    }

    // get the corresponding base64 code
    std::string code = encode(toLower(word));

    // remove all trailing '='
    int lastChar = code.length();
    for(int i = code.length() - 1; i >= 0; i--){
        if(code[i] == '='){
            lastChar = i;
        }else{
            break;
        }
    }
    code = code.substr(0, lastChar);
    std::cout << "\nCode: " << code << "\n\n";
}

// program switchboard
void menu(){
    while(true){
        std::cout << "Enter a selection\n";
        std::cout << "1. Play a Game\n";
        std::cout << "2. Generate a WINDle code\n";
        std::cout << "3. Exit\n";

        std::string selection;
        getline(std::cin,selection,'\n');

        // I would use a switch-statement here, but that only works with int-type values in C++, and there's a strange loop glitch if the user types in a string with a space in it
        if(selection == "1"){
            playGame();
        }else if(selection == "2"){
            generateCode();
        }else if(selection == "3"){
            return;
        }else{
            std::cout << "\nEnter a valid selection\n\n";
        }
    }
}

int main(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, gray);
    std::cout << "Welcome to ";
    SetConsoleTextAttribute(hConsole, green);
    std::cout << "WINDle";
    SetConsoleTextAttribute(hConsole, gray);
    std::cout << ", the Windows-based Wordle game!\n";
    menu();
    return 0;
}

// int main(){
//     HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//   // you can loop k higher to see more color choices
//   for(int k = 1; k < 255; k++)
//   {
//     // pick the colorattribute k you want
//     SetConsoleTextAttribute(hConsole, k);
//     std::cout << k << " I want to be nice today!" << std::endl;
//   }
//   int i;
//   std::cin >> i;
// }
