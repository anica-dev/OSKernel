#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

_thread *_thread::running;
_thread *_thread::mainThread;


_thread *_thread::createThread(Body body,void* arg,void* stack_space)
{
    return new _thread(body,arg,stack_space);
}

void _thread::dispatch()
{
    _thread *old = running;
    if (!old->isFinished() && old->getStatus()!=BLOCKED && old!= nullptr) {
        Scheduler::put(old);
        old->setStatus(READY);
    }
    running = Scheduler::get();
    running->setStatus(RUNNING);
    _thread::contextSwitch(&old->context, &running->context);
}

void _thread::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->getArg());
    running->setFinished(true);
    thread_dispatch();
}

int _thread::exit() {
    if (running == mainThread) return -1;
    _thread *newThr = Scheduler::get();
    if(newThr) {
        _thread* old=running;
        old->setFinished(true);
        _thread::contextSwitch(&old->context, &newThr->context);
        _mem_free(old->stack); //da li ce se ikad vratiti ovde
    }
    return 0;
}


uint64 _thread::getPid() const {
    return pid;
}

void _thread::setPid(uint64 pid) {
    _thread::pid = pid;
}

void *_thread::getArg() const {
    return arg;
}

void _thread::setArg(void *arg) {
    _thread::arg = arg;
}

Status _thread::getStatus() const {
    return status;
}

void _thread::setStatus(Status status) {
    _thread::status = status;
}

void *operator new[](size_t n)
{
    return __mem_alloc(n);
}

void operator delete[](void *p) noexcept
{
    _mem_free(p);
}