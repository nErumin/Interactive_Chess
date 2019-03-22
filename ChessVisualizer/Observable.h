#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <memory>
#include <vector>
#include "Observer.h"

template <typename... T>
class Observable
{
public:
    Observable() = default;

    void registerObserver(std::shared_ptr<Observer<T...>> newObserver);
    void unregisterObserver(std::shared_ptr<Observer<T...>> removedObserver);
    void notifyToObservers(T&&... args);

    virtual ~Observable() = default;
private:
    std::vector<std::shared_ptr<Observer<T...>>> observers;
};

template <typename... T>
void Observable<T...>::registerObserver(std::shared_ptr<Observer<T...>> newObserver)
{
    observers.push_back(newObserver);
}

template <typename... T>
void Observable<T...>::unregisterObserver(std::shared_ptr<Observer<T...>> removedObserver)
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
