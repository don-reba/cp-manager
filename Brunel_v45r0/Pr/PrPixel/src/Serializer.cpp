
//-----------------------------------------------------------------------------
// Serializer
//
// 2014-03-25 : Daniel Campora
//-----------------------------------------------------------------------------

#include "Serializer.h"

Serializer::Serializer(){
    size = 0;
    reservedSize = 0;
}

void Serializer::init(int s){
    item = 0;
    size = s;
    
    // Attempt to use previously malloc'ed chunk
    if (size < reservedSize){
        free(mem_chunk);
        reservedSize = size;
        mem_chunk = (char*) malloc(reservedSize);
    }
}

void Serializer::add(void * elem, int s){
    if (size >= item + s)
        // There will be additionally a segfault
        std::cerr << "Requested size is over memory limit" << std::endl;

    memcpy(&mem_chunk[item], elem, s);
    // std::copy(elem[0], elem[s], &(mem_chunk[item]));

    item += s;
}

char* Serializer::getPointer(){
    return mem_chunk;
}

int Serializer::getSize(){
    return size;
}
