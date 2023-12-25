#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
#include "../h/system.hpp"

extern void initMem();
extern void setStvec();
extern void userMain();


int main(){
    setStvec();
    initMem();
    initThreads();
    user_mode();
    userMain();
    return 0;
}