#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include "Observable.h"

class Timer final
{
public:
    template <typename T>
    using ArgEventHandler = std::function<void (T)>;
    using EventHandler = std::function<void ()>;

    Timer();

    void start( double timeLimit);
    void resume();
    void pause();
    void stop();

    void setTickHandler(ArgEventHandler<double> tickHandler);
    void setFinishedHandler(EventHandler finishedHandler);
private:
    static void startLoop(Timer& timer, double timeLimit);

    bool isStopped() const;
    bool isPaused() const;

    void setRunning(bool running);
    void setPaused(bool paused);
private:
    bool running = false;
    bool paused = false;

    ArgEventHandler<double> tickFunctor;
    EventHandler finishedFunctor;
};

#endif // TIMER_H
