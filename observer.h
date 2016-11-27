#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>
#include <string>
class Observer
{
public:
    virtual ~Observer(){}
    virtual void update()= 0;
    virtual void update(void *args) = 0;

};

#endif // OBSERVER_H
