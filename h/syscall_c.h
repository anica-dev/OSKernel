#pragma once
#include "../lib/hw.h"

class _thread;
typedef _thread* thread_t;

class _sem;
typedef _sem* sem_t;

#ifdef __cplusplus
extern "C" {
#endif

const int EOF=-1;

uint64 call_system(uint64 num,...);
void* mem_alloc(size_t size);
int mem_free(void*);
int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();
int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
void user_mode();
void putc(char c);
char getc();

#ifdef __cplusplus
}
#endif





