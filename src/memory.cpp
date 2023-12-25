//
// Created by os on 8/18/22.
//
#include "../h/memory.h"

static block* freeMem; //pokazivac na pocetak slobodne mem
static block* occupiedMem; //pok na pocetak zauzete


void tryToJoin(block* cur){ //povezuje segment cur sa narednim
    if(!cur) return;
    if(cur->next && (char*)cur+cur->size==(char*)(cur->next)){
        cur->size+=cur->next->size;
        cur->next=cur->next->next;
        if(cur->next) cur->next->prev=cur;
    }
}
void removeFromFreeMem(block* blk,size_t size){
    if(blk==freeMem){  //ako hocemo da izbacimo prvi iz liste
        freeMem=freeMem->next;
        if(freeMem) freeMem->prev=0;  //ako to nije istovremeno i prvi i poslednji
    }
    block* prev=blk->prev;
    block* next=blk->next;
    if(prev) prev->next=blk->next;
    if(next) next->prev=blk->prev;
    blk->prev=blk->next=0;
}

void addToFreeMem(block* blk, size_t size){
    blk->size=size;
    blk->next=blk->prev= nullptr;
    if(freeMem){
        if((char*)blk<(char*)freeMem){ //ako se novi blok dodaje na mesto pre pocetka slobodne mem
            blk->next=freeMem;
            freeMem->prev=blk;
            freeMem=blk;
            tryToJoin(blk);
            return;
        }
        for (block* cur=freeMem;cur!=0;cur=cur->next){
            if(!cur->next){   //poslednji u listi slobodnih, spajamo ga samo s prethodnim blokom
                cur->next=blk;
                blk->prev=cur;
                tryToJoin(cur);
                return;
            }
            if((char*)blk>(char*)cur && (char*)blk<(char*)cur->next ){ //dodajemo u sredini liste
                blk->next=cur->next;
                cur->next=blk;
                cur->next->prev=blk;
                blk->prev=cur;
                tryToJoin(blk);
                tryToJoin(cur);
                return;
            }
        }

    }
    else {
        freeMem=blk;
    }
}

void addToOccMem(block* blk, size_t size){
    blk->size=size;
    if(occupiedMem){
        if((char*)blk<(char*)occupiedMem){ //ako se novi blok dodaje na mesto pre pocetka zauzete mem
            blk->next=occupiedMem;
            occupiedMem->prev=blk;
            occupiedMem=blk;
            return;
        }
        for (block* cur=occupiedMem;cur!=0;cur=cur->next){
            if(!cur->next){
                cur->next=blk;
                blk->prev=cur;
                return;
            }
            if((char*)blk>(char*)cur && (char*)blk<(char*)cur->next ){
                blk->next=cur->next;
                cur->next=blk;
                cur->next->prev=blk;
                blk->prev=cur;
                return;
            }
        }

    }
    else {
        occupiedMem=blk;
    }
}
void removeFromOccMem(block *blk) {
    if(blk==occupiedMem){  //ako hocemo da izbacimo prvi iz liste
        occupiedMem=occupiedMem->next;
        if(occupiedMem) occupiedMem->prev=0;  //ako to nije istovremeno i prvi i poslednji
    }
    block* prev=blk->prev;
    block* next=blk->next;
    if(prev) prev->next=blk->next;
    if(next) next->prev=blk->prev;
    blk->prev=blk->next=0;
}

void initMem(){
    freeMem=(block*)HEAP_START_ADDR;
    freeMem->size=(size_t)HEAP_END_ADDR-(size_t)HEAP_START_ADDR;
}

void* __mem_alloc(size_t numOfBits){
    if(!freeMem)return 0; //nema vise slobodne memorije
    uint64 numOfBlocks=(numOfBits+sizeof(block))/MEM_BLOCK_SIZE + ((numOfBits+ sizeof(block))%MEM_BLOCK_SIZE?1:0);
    size_t size=numOfBlocks*MEM_BLOCK_SIZE;

    block *blk=0;

    for(block* cur=freeMem; cur != 0; cur=cur->next){ //krecemo od pocetka i trazimo prvi slobodan blok dovoljne velicine
        if(cur->size>=size){
            blk=cur;
            break;
        }
    }
    if(blk==0) return 0;
    removeFromFreeMem(blk,blk->size);
    if(blk->size>size) addToFreeMem((block*)((char*)blk+size),blk->size-size);
    addToOccMem(blk,size);
    return (char*)blk+ sizeof(block);
}

void* _mem_alloc(size_t numOfBlocks){
    if(!freeMem)return 0; //nema vise slobodne memorije
    size_t size=numOfBlocks*MEM_BLOCK_SIZE;

    block *blk=0;

    for(block* cur=freeMem; cur != 0; cur=cur->next){ //krecemo od pocetka i trazimo prvi slobodan blok dovoljne velicine
        if(cur->size>=size){
            blk=cur;
            break;
        }
    }
    if(blk==0) return 0;
    removeFromFreeMem(blk,blk->size);
    if(blk->size>size) addToFreeMem((block*)((char*)blk+size),blk->size-size);
    addToOccMem(blk,size);
    return (char*)blk+ sizeof(block);
}

int _mem_free(void *addr){
    if(!addr)return -1;
    if(occupiedMem){ //ako uopste ima zauzete memorije
        block* blk=(block*)((char*)addr-sizeof(block));
        for(block* cur=occupiedMem;cur!=0;cur=cur->next){
            if(cur==blk){
                removeFromOccMem(blk);
                addToFreeMem(blk,blk->size);
                return 0;
            }
        }
    }
    return -2;
}



