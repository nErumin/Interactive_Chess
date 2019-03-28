#include "Timer.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono;

Timer::Timer() = default;

template <typename Rep, typename Count>
Rep systemCountToSecond(Count count)
{
    auto nowPoint = system_clock::now();
    Rep systemClockRatio = static_cast<Rep>(decltype(nowPoint.time_since_epoch())::period::num) /
                                            decltype(nowPoint.time_since_epoch())::period::den;

    return count * systemClockRatio;
}

void Timer::startLoop(Timer& timer, double timeLimit)
{
    auto startPoint = system_clock::now();
    auto startCount = startPoint.time_since_epoch().count();

    constexpr double systemClockRatio = static_cast<double>(decltype(startPoint.time_since_epoch())::period::num) /
                                        decltype(startPoint.time_since_epoch())::period::den;
    decltype(auto) limitCount = static_cast<decltype(startPoint.time_since_epoch())::rep>(timeLimit / systemClockRatio);

    timer.setPaused(false);
    timer.setRunning(true);

    auto diffCount = limitCount;
    while ((diffCount = (system_clock::now().time_since_epoch().count() - startCount)) <= limitCount)
    {
        while (timer.isPaused())
        {
            startCount = system_clock::now().time_since_epoch().count() + diffCount;
        }

        if (timer.isStopped())
        {
            return;
        }

        timer.notifyToObservers(systemCountToSecond<double>(limitCount - diffCount));
    }

    // End
    timer.notifyToObservers(0.0);
}

void Timer::start(double timeLimit)
{
    std::thread timerThread{ Timer::startLoop, std::ref(*this), timeLimit };

    timerThread.detach();
}

void Timer::resume()
{
    setRunning(true);
    setPaused(false);
}

void Timer::pause()
{
    setRunning(true);
    setPaused(true);
}

void Timer::stop()
{
    setRunning(false);
    setPaused(false);
}

bool Timer::isStopped() const
{
    std::lock_guard<std::mutex> guard{ timerMutex };
    return !running && !paused;
}

bool Timer::isPaused() const
{
    std::lock_guard<std::mutex> guard{ timerMutex };
    return running && paused;
}

void Timer::setRunning(bool running)
{
    std::lock_guard<std::mutex> guard{ timerMutex };
    this->running = running;
}

void Timer::setPaused(bool paused)
{
    std::lock_guard<std::mutex> guard{ timerMutex };
    this->paused = paused;
}
