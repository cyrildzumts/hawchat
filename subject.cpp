#include "subject.h"

Subject::Subject()
{

}
Subject::Subject(std::string name)
{
    this->name = name;
}

Subject::~Subject()
{
    observers.clear();
}

void Subject::addObserver(Observer *observer)
{
    observers.push_back(observer);
}

void Subject::removeObserver(Observer *observer)
{

}

void Subject::notify()
{
    for(auto obsever : observers)
    {
        obsever->update();
    }
}

void Subject::notify(void *args)
{
    for(auto observer : observers)
    {
        observer->update(args);
    }
}
