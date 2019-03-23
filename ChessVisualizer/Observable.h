#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <memory>
#include <vector>
#include <algorithm>
#include "Observer.h"

template <typename... T>
class Observable
{
public:
    Observable() = default;

    void registerObserver(Observer<T...>* newObserver);
    void unregisterObserver(Observer<T...>* removedObserver);
    void notifyToObservers(T&&... args);

    virtual ~Observable() = default;
private:
    std::vector<Observer<T...>*> observers;
};

template <typename... T>
void Observable<T...>::registerObserver(Observer<T...>* newObserver)
{
    observers.push_back(newObserver);
}

template <typename... T>
void Observable<T...>::unregisterObserver(Observer<T...>* removedObserver)
{
    auto foundIterator = std::find(observers.cbegin(), observers.cend(), removedObserver);

    if (foundIterator != observers.cend())
    {
        observers.erase(foundIterator);
    }
}

template <typename... T>
void Observable<T...>::notifyToObservers(T&&... args)
{
    for (const auto& observer : observers)
    {
        observer->notify(std::forward<T>(args)...);
    }
}

#endif // OBSERVABLE_H
