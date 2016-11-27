#ifndef CONCRETEOBSERVER_H
#define CONCRETEOBSERVER_H

#include "observer.h"
#include <iostream>
class ConcreteObserver : public Observer
{
public:
    ConcreteObserver();
    virtual void update()override;
    virtual void update(void *args)override;
};

#endif // CONCRETEOBSERVER_H
