#include "scheduler.h"

using namespace std;

Scheduler::Scheduler()
{
    _lastTick = millis();    
}

Scheduler::~Scheduler()
{
    TaskInfo *nextTask = _firstTask;
    while(!nextTask)
    {        
        TaskInfo *t = nextTask;
        nextTask = nextTask->Next;
        delete t;
    }
}

void Scheduler::Tick()
{
    _lastTick = millis();
    TaskInfo *nextTask = _firstTask;
    while(nextTask != NULL)
    {
        if( (_lastTick - nextTask->LastCall ) > nextTask->Interval )
        {
            nextTask->Task();
            nextTask->LastCall = _lastTick;            
        }
        nextTask = nextTask->Next;
    }
}

void Scheduler::ScheduleTask(void task(), unsigned long interval)
{
    TaskInfo *taskInfo = new TaskInfo();
    taskInfo->Task = task;
    taskInfo->Interval = interval; 
    taskInfo->LastCall = _lastTick;

    if(_firstTask == NULL)
    {
        _firstTask = taskInfo;
        return;
    }

    TaskInfo *nextTask = _firstTask;
    while(nextTask->Next != NULL)
    {
        nextTask = nextTask->Next;
    }

    nextTask->Next = taskInfo;
}

