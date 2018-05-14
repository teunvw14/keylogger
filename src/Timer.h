#ifndef TIMER_H
#define TIMER_H

#include "..\include\mingw.thread.h"

class Timer
{
    private:
        mingw_stdthread::thread thread1;
        bool alive = false;
        long totalFunctCalls = -1L;
        long repeatCounter = -1L;
        std::chrono::milliseconds interval = std::chrono::milliseconds(0);
        std::function<void(void)> funct = nullptr;

    public:
      static const long Infinite = -1L;

      Timer();
      Timer(const std::function<void(void)> &f);
      Timer(const std::function<void(void)> &f,
            const unsigned long &i,
            const long iterations);

      bool IsAlive() const;

      void SleepAndRun();
      void Start(bool Async);
      void Stop();
      void ThreadFunc();
      void SettotalFunctCalls(const long r);
      void SetFunction(const std::function<void(void)> &f);
      void SetInterval(const unsigned long &i);
      unsigned long GetInterval() const;
      const std::function<void(void)> &GetTimerFunction() const;
};


#endif // TIMER_H