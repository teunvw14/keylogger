#include <chrono>
#include "..\include\mingw.thread.h"
#include "Timer.h"

Timer::Timer() {}
Timer::Timer(const std::function<void(void)> &f) : funct(f) {}
Timer::Timer(const std::function<void(void)> &f,
             const unsigned long &i, 
             const long iterations = Timer::Infinite) : funct(f),
                                                    interval(std::chrono::milliseconds(i)),
                                                    totalFunctCalls(iterations)
                                                    {}

void Timer::SleepAndRun()
{
    mingw_stdthread::this_thread::sleep_for(interval);
    if (alive)
        GetTimerFunction()();

}

void Timer::Start(bool Async = true)
{
    if (!IsAlive())
    {
        alive = true;
        repeatCounter = totalFunctCalls;
        if (Async)
            thread1 = mingw_stdthread::thread(&ThreadFunc, this);
        else
            this->ThreadFunc();
    }
}

void Timer::Stop()
{
    alive = false;
    thread1.join();
}

bool Timer::IsAlive() const { return alive; }

void Timer::SetFunction(const std::function<void(void)> &f)
{
    funct = f;
}

void Timer::SettotalFunctCalls(const long r)
{
    if (!alive)
        totalFunctCalls = r;
}

void Timer::SetInterval(const unsigned long &i)
{
    if (!alive)
    {
        interval = std::chrono::milliseconds(i);
    }
}

unsigned long Timer::GetInterval() const {return interval.count();}

const std::function<void(void)> &Timer::GetTimerFunction() const 
{
    return funct;
}

void Timer::ThreadFunc()
{
    if (totalFunctCalls == Infinite)
    {
        while(alive)
        {
            SleepAndRun();
        }
    }
    else
    {
        while (repeatCounter > 0)
        {
            repeatCounter --;
            SleepAndRun();
        }
    }
}
