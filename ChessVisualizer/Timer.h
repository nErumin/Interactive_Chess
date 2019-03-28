#ifndef TIMER_H
#define TIMER_H

#include <mutex>
#include "Observable.h"

class Timer : public Observable<double>
{
public:
    Timer();

    void start(double timeLimit);
    void pause();
    void stop();
private:
    static void startLoop(Timer& timer, double timeLimit);

    bool isStopped() const;
    bool isPaused() const;

    void setRunning(bool running);
    void setPaused(bool paused);
private:
    mutable std::mutex timerMutex;
    bool running = false;
    bool paused = false;
};

#endif // TIMER_H
