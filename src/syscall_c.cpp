//
// Created by os on 8/19/22.
//
#include "../h/syscall_c.h"

uint64 call_system(uint64 num, ...){
    uint64 r=-1;
    __asm __volatile("ecall");
    __asm __volatile("mv %[ime], a0" : [ime] "=r" (r));
    return r;
}
void* mem_alloc(size_t size){
    uint64 numOfBlocks=size/MEM_BLOCK_SIZE + (size%MEM_BLOCK_SIZE?1:0)+1;
    return (void*) call_system(0x01, numOfBlocks);
}

int mem_free(void* v){
    return call_system(0x02,(uint64) v);
}

int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg){
    void* stack_space=mem_alloc(DEFAULT_STACK_SIZE);

    return call_system(0x11,(uint64) handle ,(uint64)start_routine,(uint64)arg,(uint64)stack_space);
}

int thread_exit(){
    return call_system(0x12);
}

void thread_dispatch(){
    call_system(0x13);
}

int sem_open(sem_t* handle, unsigned init){
    return call_system(0x21,(uint64)handle,init);
}
int sem_close(sem_t handle){
    return call_system(0x22,(uint64)handle);
}
int sem_wait(sem_t id){
    return call_system(0x23,id);
}
int sem_signal(sem_t id){
    return call_system(0x24,id);
}

char getc(){
    return  (char)call_system(0x41);
}

void putc(char c){
    call_system(0x42,(uint64) c);

}
void user_mode(){
    call_system(0x43);
}


