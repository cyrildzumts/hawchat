#ifndef SUBJECT_H
#define SUBJECT_H
#include "observer.h"

class Subject
{
public:
    Subject();
    Subject(std::string name);
    virtual ~Subject();
    void addObserver(Observer *observer);
    void removeObserver(Observer* observer);
    virtual void notify();
    virtual void notify(void *args);


protected:
    std::vector<Observer*>observers;
    std::string name;
};

#endif // SUBJECT_H
