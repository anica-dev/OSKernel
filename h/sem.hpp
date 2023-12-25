//
// Created by os on 8/22/22.
//
#include "../lib/hw.h"
#include "../h/list.hpp"
#include "../h/tcb.hpp"

#ifndef PROJECT_BASE_SEM_H
#define PROJECT_BASE_SEM_H

static uint64 semCounter=0;
class _sem {
public:
    static _sem* createSem(unsigned init);
    static void _sem_close();
    static int _sem_wait();
    static int _sem_signal();

    uint64 getId() const {
        return id;
    }

    void setId(uint64 id) {
        _sem::id = id;
    }

    uint64 getVal() const {
        return val;
    }

    void setVal(uint64 val) {
        _sem::val = val;
    }

     List<_thread> &getBlockedQueue()  {
        return blockedQueue;
    }

private:

    explicit _sem(uint64 val):val(val) {
        id=semCounter;
        semCounter++;
    }
    uint64 id;
    uint64 val;
    List<_thread> blockedQueue;

};


#endif //PROJECT_BASE_SEM_H
