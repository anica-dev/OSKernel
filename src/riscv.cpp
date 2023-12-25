#include "../h/riscv.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap() {
    uint64 scause;
    uint64 a0, a1, a2, a3, a4;
    __asm __volatile("csrr %[ime], scause" : [ime] "=r" (scause));

    switch (scause) {

        case 0x01UL << 63 | 0x01:
        { //timer interrupt

            __asm __volatile("csrc sip, 0x02");
        }break;

        case 0x08:{ //ecall iz korisnickog rezima
            __asm __volatile("mv %[ime], a0" : [ime] "=r" (a0));
            __asm __volatile("mv %[ime], a1" : [ime] "=r" (a1));
            __asm __volatile("mv %[ime], a2" : [ime] "=r" (a2));
            __asm __volatile("mv %[ime], a3" : [ime] "=r" (a3));
            __asm __volatile("mv %[ime], a4" : [ime] "=r" (a4));

            switch (a0) {

                case 0x01: {
                    a0=(uint64)_mem_alloc(a1);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;

                case 0x02:{
                    a0=_mem_free((void *)a1);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;

                case 0x11:{
                    a0=(uint64)_thread_create((thread_t*)a1,(void(*)(void*))a2,(void*)a3,(void*)a4);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));

                }break;

                case 0x12:{_thread_exit();} break;

                case 0x13:{
                    uint64 volatile sepc,sstatus;
                    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
                    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
                    _thread_dispatch();
                    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
                    __asm__ volatile ("csrw  sepc, %[sepc]" : : [sepc] "r"(sepc));
                } break;

                case 0x21:{
                    a0= _sem_open((sem_t*)a1,a2);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;
                case 0x22:{
                    a0= _sem_close((sem_t)a1);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;
                case 0x23:{
                    a0= sem_wait((sem_t)a1);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;
                case 0x24:{
                    a0= sem_signal((sem_t)a1);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;
                case 0x31:{}break;

                case 0x41:{
                    a0=_getc();
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;

                case 0x42:{
                    _putc((char)a1);
                }break;

                case 0x43:{_user_mode();}break;

                default:{}break;
            }
            uint64 volatile sepc;
            __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
            sepc+=4;
            __asm__ volatile ("csrw sepc, %[ime]" : : [ime] "r"(sepc));
            __asm __volatile("csrc sip, 0x02");
        }break;
        case 0x09:{ //ecall iz sistemskog rezima
            __asm __volatile("mv %[ime], a0" : [ime] "=r" (a0));
            __asm __volatile("mv %[ime], a1" : [ime] "=r" (a1));
            __asm __volatile("mv %[ime], a2" : [ime] "=r" (a2));
            __asm __volatile("mv %[ime], a3" : [ime] "=r" (a3));
            __asm __volatile("mv %[ime], a4" : [ime] "=r" (a4));

            switch (a0) {

                case 0x01: {
                    a0=(uint64)_mem_alloc(a1);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;

                case 0x02:{
                    a0=_mem_free((void *)a1);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;

                case 0x11:{
                    a0=(uint64)_thread_create((thread_t*)a1,(void(*)(void*))a2,(void*)a3,(void*)a4);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));

                }break;

                case 0x12:{_thread_exit();} break;

                case 0x13:{
                    uint64 volatile sepc,sstatus;
                    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
                    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
                    _thread_dispatch();
                    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
                    __asm__ volatile ("csrw  sepc, %[sepc]" : : [sepc] "r"(sepc));
                } break;

                case 0x21:{
                    a0= _sem_open((sem_t*)a1,a2);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;
                case 0x22:{
                    a0= _sem_close((sem_t)a1);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;
                case 0x23:{
                    a0= sem_wait((sem_t)a1);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;
                case 0x24:{
                    a0= sem_signal((sem_t)a1);
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;
                case 0x31:{}break;
                case 0x41:{
                    a0=(uint64)_getc();
                    __asm__ volatile ("mv a0, %[ime]" : : [ime] "r"(a0));
                }break;

                case 0x42:{
                    _putc(a1);
                }break;
                case 0x43:{_user_mode();}break;

                default:{}break;
            }
            uint64 volatile sepc;
            __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
            sepc+=4;
            __asm__ volatile ("csrw sepc, %[ime]" : : [ime] "r"(sepc));
            __asm __volatile("csrc sip, 0x02");
        }
    }
    console_handler();
}