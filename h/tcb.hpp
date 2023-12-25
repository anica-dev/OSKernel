
#ifndef PROJECT_BASE_TCB_HPP
#define PROJECT_BASE_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "../h/syscall_c.h"
#include "../h/memory.h"

static uint threadCounter=1;
void *operator new[](size_t n);
void operator delete[](void *p) noexcept;

enum Status{READY, BLOCKED,RUNNING};

class _thread
{
public:
    ~_thread() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getPid() const;

    void setPid(uint64 pid);

    void *getArg() const;

    void setArg(void *arg);

    Status getStatus() const;

    void setStatus(Status status);

    using Body = void (*)(void*);

    Body getBody(){
        return body;
    }

    void setBody(void (*body)(void*)){
        body=body;
    }

    static _thread *createThread(Body body, void* arg, void* stack_space);

    static void dispatch();

    static int exit();

    static _thread *running;
    static _thread *mainThread;

private:
    explicit _thread(Body body, void* arg, void* stack_space) :
            body(body),
            arg(arg),

            finished(false),
            pid(threadCounter)
    {
        if(body!= nullptr) stack= new uint64[DEFAULT_STACK_SIZE];
        else stack= nullptr;

        context={body != nullptr ? (uint64) &threadWrapper : 0,
                 stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0
        };
        threadCounter++;
        if (body != nullptr) { Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void* arg;
    uint64 *stack;
    Context context;
    bool finished;
    uint64 pid;
    Status status;

    static void contextSwitch(Context *oldContext, Context *runningContext);

    friend class Riscv;

    static void threadWrapper();

};

#endif
