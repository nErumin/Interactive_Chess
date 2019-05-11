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

    void registerObserver(Observer<T...>* newObserver, size_t priority = 0);
    void unregisterObserver(Observer<T...>* removedObserver);
    void notifyToObservers(T&&... args);

    virtual ~Observable() = default;
private:
    std::vector<std::pair<Observer<T...>*, size_t>> observers;
};

#include <iostream>

template <typename... T>
void Observable<T...>::registerObserver(Observer<T...>* newObserver, size_t priority)
{
    observers.emplace_back(newObserver, priority);

    std::sort(observers.begin(), observers.end(), [](const auto& left, const auto& right)
    {
        return left.second > right.second;
    });
}

template <typename... T>
void Observable<T...>::unregisterObserver(Observer<T...>* removedObserver)
{
    auto foundIterator = std::find_if(observers.cbegin(), observers.cend(), [removedObserver](const auto& pair)
    {
        return pair.first == removedObserver;
    });

    if (foundIterator != observers.cend())
    {
        observers.erase(foundIterator);
    }
}

template <typename... T>
void Observable<T...>::notifyToObservers(T&&... args)
{
    for (const auto& observerPriority : observers)
    {
        observerPriority.first->notify(std::forward<T>(args)...);
    }
}

#endif // OBSERVABLE_H
