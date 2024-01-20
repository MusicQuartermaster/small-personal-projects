#include <iostream>
#include <string>
#include <stdexcept>
#include "base64.h"

// base64 alphabet
std::string b64_characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// use a union to easily convert 3 8-bit binary strings into 4 6-bit binary strings and vice-versa
union data {
    struct base64{
        unsigned int d : 6;
        unsigned int c : 6;
        unsigned int b : 6;
        unsigned int a : 6;
    };

    struct decimal{
        unsigned int c : 8;
        unsigned int b : 8;
        unsigned int a : 8;
    };

    base64 b64;
    decimal dec;
};

// encode an entire 3-byte string into base64
std::string full_encode(std::string plaintext){
    data base64StringPart;

    // put decimal data into union
    base64StringPart.dec.a = plaintext.at(0);
    base64StringPart.dec.b = plaintext.at(1);
    base64StringPart.dec.c = plaintext.at(2);

    // use the base64 numerical values as an index into the base64 alphabet and add the corresponding characters to the result
    std::string res(1, b64_characters.at(base64StringPart.b64.a));
    res += b64_characters.at(base64StringPart.b64.b);
    res += b64_characters.at(base64StringPart.b64.c);
    res += b64_characters.at(base64StringPart.b64.d);
    return res;
}

// encode only a 2-byte string into base64
std::string encode2(std::string plaintext){
    data base64StringPart;

    // put decimal data into union
    base64StringPart.dec.a = plaintext.at(0);
    base64StringPart.dec.b = plaintext.at(1);
    base64StringPart.dec.c = 0;
    
    // use the base64 numerical values as an index into the base64 alphabet and add the corresponding characters to the result
    std::string res(1, b64_characters.at(base64StringPart.b64.a));
    res += b64_characters.at(base64StringPart.b64.b);
    res += b64_characters.at(base64StringPart.b64.c);
    // pad last character with '='
    res += "=";
    return res;
}

// encode only a 1-byte string into base64
std::string encode1(std::string plaintext){
    data base64StringPart;

    // put decimal data into union
    base64StringPart.dec.a = plaintext.at(0);
    base64StringPart.dec.b = 0;
    base64StringPart.dec.c = 0;
    
    // use the base64 numerical values as an index into the base64 alphabet and add the corresponding characters to the result
    std::string res(1, b64_characters.at(base64StringPart.b64.a));
    res += b64_characters.at(base64StringPart.b64.b);
    // pad last 2 characters with '='
    res += "==";
    return res;
}

// encode an ASCII string into base64
std::string encode(std::string plaintext){
    std::string res = "";

    // as long as there are at least 3 characters to encode, we can do a full encode (3 ASCII characters -> 4 base64 characters)
    while(plaintext.length() >= 3){
        std::string first3 = plaintext.substr(0, 3);
        plaintext = plaintext.substr(3, plaintext.length() - 3);

        res += full_encode(first3);
    }

    // encode any remaining characters
    if(plaintext.length() == 2){
        res += encode2(plaintext);
    }else if(plaintext.length() == 1){
        res += encode1(plaintext);
    }

    return res;
}

// decode a base64 string back into ASCII characters
std::string decode(std::string ciphertext){
    std::string res = "";
    while(ciphertext.length() > 0){
        // if the ciphertext is not a multiple of 4, pad it with '=' to fill in the remaining values
        while(ciphertext.length() < 4){
            ciphertext += "=";
        }

        // decode the base64 4 characters at a time
        std::string first4 = ciphertext.substr(0, 4);
        ciphertext = ciphertext.substr(4, ciphertext.length() - 4);

        data base64StringPart;
        // put base64 data into union
        base64StringPart.dec.a = 0;
        base64StringPart.dec.b = 0;
        base64StringPart.dec.c = 0;

        std::string currentChar;
        int loc;

        // if the first character is an '=', the remaining string is just padding (or a malformed base64 string). either way, it can be ignored
        if(first4.at(0) == '='){
            break;
        }

        // find the index of the character in the base64 alphabet
        currentChar = "";
        currentChar.append(1, first4.at(0)); 
        loc = b64_characters.find(currentChar);

        // if the character is not in the alphabet, the base64 string is invalid
        if(loc == std::string::npos){
            throw std::invalid_argument("Invalid Base64 String");
        }

        // put the index into the union
        base64StringPart.b64.a = loc;

        // if the second character is an '=', the remaining string is just padding (or a malformed base64 string). either way, it can be ignored
        if(first4.at(1) == '='){
            res += (char)(base64StringPart.dec.a);
            res += (char)(base64StringPart.dec.b);
            break;
        }

        // find the index of the character in the base64 alphabet
        currentChar = "";
        currentChar.append(1, first4.at(1)); 
        loc = b64_characters.find(currentChar);

        // if the character is not in the alphabet, the base64 string is invalid
        if(loc == std::string::npos){
            throw std::invalid_argument("Invalid Base64 String");
        }

        // put the index into the union
        base64StringPart.b64.b = loc;

        // if the third character is an '=', the remaining string is just padding (or a malformed base64 string). either way, it can be ignored
        if(first4.at(2) == '='){
            res += (char)(base64StringPart.dec.a);
            res += (char)(base64StringPart.dec.b);
            // only add the third value if it is a valid ASCII value
            if(base64StringPart.dec.c > 0){
                res += (char)(base64StringPart.dec.c);
            }   
            break;
        }

        // find the index of the character in the base64 alphabet
        currentChar = "";
        currentChar.append(1, first4.at(2)); 
        loc = b64_characters.find(currentChar);

        // if the character is not in the alphabet, the base64 string is invalid
        if(loc == std::string::npos){
            throw std::invalid_argument("Invalid Base64 String");
        }

        // put the index into the union
        base64StringPart.b64.c = loc;

        // if the fourth (and final) character is an '=', it's just padding and can be ignored
        if(first4.at(3) == '='){
            res += (char)(base64StringPart.dec.a);
            res += (char)(base64StringPart.dec.b);
            // only add the third value if it is a valid ASCII value
            if(base64StringPart.dec.c > 0){
                res += (char)(base64StringPart.dec.c);
            }   
            break;
        }
        
        // find the index of the character in the base64 alphabet
        currentChar = "";
        currentChar.append(1, first4.at(3)); 
        loc = b64_characters.find(currentChar);

        // if the character is not in the alphabet, the base64 string is invalid
        if(loc == std::string::npos){
            throw std::invalid_argument("Invalid Base64 String");
        }

        // put the index into the union
        base64StringPart.b64.d = loc;
        
        // store the ASCII values into the result
        res += (char)(base64StringPart.dec.a);
        res += (char)(base64StringPart.dec.b);
        res += (char)(base64StringPart.dec.c);
    }

    return res;
}