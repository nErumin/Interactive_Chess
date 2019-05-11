#ifndef OBSERVER_H
#define OBSERVER_H

template <typename... T>
class Observer
{
public:
    virtual void notify(T&&... args) = 0;
    virtual ~Observer() = default;
};

#endif // OBSERVER_H
