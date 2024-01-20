#ifndef BASE64_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define BASE64_H

#include <iostream>
#include <string>
#include <stdexcept>

std::string encode(std::string);

std::string decode(std::string);

#endif