#ifndef scheduler_h
#define scheduler_h

#include <Arduino.h>

typedef void (*func_type)(void);

struct TaskInfo
{
    func_type Task;
    unsigned long Interval;
    unsigned long LastCall;
    TaskInfo *Next;
};

class Scheduler
{
    public:    
    Scheduler();
    ~Scheduler();    

    void Tick();
    void ScheduleTask(void task(), unsigned long interval);

    private:
    TaskInfo *_firstTask;
    unsigned long _lastTick;
};

#endif