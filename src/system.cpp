//
// Created by os on 8/11/22.
//
#include "../h/system.hpp"
#include "../h/scheduler.hpp"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/sem.hpp"

static List<_sem> semList;

int _thread_create(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space){
    *handle=_thread::createThread(start_routine,arg,stack_space);
    if(*handle) return 0;
    else return -1;
}
int _thread_exit(){
    return _thread::exit();
}

void _thread_dispatch(){
    _thread::dispatch();
}

void initThreads(){
    _thread::mainThread=_thread::running=_thread::createThread(nullptr, nullptr, nullptr);
}

void _user_mode(){
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(1UL << 8));
}
extern "C" void supervisorTrap();
void setStvec(){
    __asm__ volatile("csrw stvec, %[vector] "::[vector] "r" (&supervisorTrap));
}

char _getc(){
    char c= __getc();
    //_thread::dispatch();
    return c;
}

void _putc(char c){
    __putc(c);
    //_thread::dispatch();

}

int _sem_open(sem_t* handle,unsigned init){
    (*handle)=_sem::createSem(init);
    semList.addLast(*handle);
    _thread::dispatch();
    if(*handle) return 0;
    else return -1;
}
int _sem_close(sem_t handle){
    if(!handle) return -1;
    sem_t s=semList.get(handle);
    while (s->getBlockedQueue().peekFirst()){
        thread_t t=s->getBlockedQueue().removeFirst();
        t->setStatus(READY);
        Scheduler::put(t);
    }
    delete handle;
    _thread::dispatch();
    return 0;
}

int _sem_wait(sem_t handle){
    if(!handle) return -1;
    sem_t s=semList.get(handle);
    if(!s)return -1;
    s->setVal(s->getVal()-1);
    if(s->getVal()<0){
        thread_t old=_thread::running;
        old->setStatus(BLOCKED);
        s->getBlockedQueue().addLast(old);
        _thread::dispatch();
    }
    _thread::dispatch();
    return 0;
}

int _sem_signal(sem_t handle){
    if(!handle) return -1;
    sem_t s=semList.get(handle);
    s->setVal(s->getVal()+1);
    if(s->getVal()<=0){
        thread_t thr=s->getBlockedQueue().removeFirst();
        if(thr) {
            thr->setStatus(READY);
            Scheduler::put(thr);
        }
    }
    _thread::dispatch();
    return 0;
}
