//
// Created by os on 8/18/22.
//

#ifndef PROJECT_BASE_MEMORY_H
#define PROJECT_BASE_MEMORY_H
#include "../lib/hw.h"
#include "../test/printing.hpp"

typedef struct block{
    size_t size;
    struct block* next;
    struct block* prev;
} block;

void* __mem_alloc(size_t size);
void* _mem_alloc(size_t numOfBlocks);

int _mem_free(void*);
#endif
