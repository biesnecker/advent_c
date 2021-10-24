#pragma once

#include <stdint.h>
#include <stdlib.h>

void md5(const void* input, size_t len, uint8_t* output);

void md5HexToBuffer(const void* input, size_t len, char* buffer);

char* md5Hex(const void* input, size_t len);
