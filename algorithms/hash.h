#pragma once

#include <iomanip>
#include <openssl/sha.h>
#include <sstream>
#include <string>

// create hash string from string using sha256 algorithm
std::string sha256(const std::string& str);