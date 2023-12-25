//
// Created by os on 8/1/22.

#include "../h/syscall_cpp.hpp"
#include "../h/scheduler.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_cpp.hpp"

Thread::Thread(void (*body)(void*), void* arg){}

Thread::~Thread (){delete myHandle;}

int Thread::start() {
    if(myHandle) return myHandle->getPid();
    else return thread_create(&myHandle, &wrapper, this);
}
void Thread::dispatch () {thread_dispatch();}

int Thread::sleep(time_t) {return 0;}

Thread::Thread(){
    myHandle= nullptr;
}
void *operator new(size_t n)
{
    return mem_alloc(n);
}
void operator delete(void *p)
{
    mem_free(p);
}
