//
// Created by os on 8/22/22.
//
#include "../h/sem.hpp"

_sem* _sem::createSem(unsigned init){
    return new _sem(init);
}