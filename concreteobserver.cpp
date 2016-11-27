#include "concreteobserver.h"

ConcreteObserver::ConcreteObserver()
{

}

void ConcreteObserver::update()
{
    std::cout << " Subject has changed" << std::endl;
}

void ConcreteObserver::update(void *args)
{

    std::cout << " Subject "
              <<" has changed" << std::endl;
}
