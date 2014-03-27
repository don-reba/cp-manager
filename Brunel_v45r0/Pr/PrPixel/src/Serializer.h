
#pragma once

//-----------------------------------------------------------------------------
// Serializer
//
// 2014-03-25 : Daniel Campora
//-----------------------------------------------------------------------------

#include <cstring>
#include <iostream>
#include <algorithm>

class Serializer {
friend class PrPixelSerializer;

private:
    int size, reservedSize, item;
    char* mem_chunk;

    void init(int size);
    void add(void* elem, int size);

public:
    Serializer();
    char* getPointer();
    int getSize();
};
