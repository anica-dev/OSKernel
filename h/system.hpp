//
// Created by os on 8/11/22.
//
#pragma once
#include "../h/tcb.hpp"
typedef _thread* thread_t;


int _thread_create(thread_t* handle, void(*start_routine)(void*), void* arg, void* stack_space);
int _thread_exit();
void _thread_dispatch();
void _user_mode();
void initThreads();
char _getc();
void _putc(char c);
int _sem_open(sem_t* handle,unsigned init);
int _sem_close(sem_t handle);
int _sem_wait(sem_t handle);
int _sem_signal(sem_t handle);


