#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "observer.h"
#include <iostream>
#include <vector>

using namespace std;

class Observable {
private:
    vector<Observer*> observers;

public:
    void addObserver(Observer* obs) {
        observers.push_back(obs);
    }

    void deleteObserver(Observer* obs) {
        int location = -1;
        for(unsigned int i = 0; i < observers.size(); i++) {
            if(obs == observers[i]) {
                location = i;
                break;
            }
        }
        observers.erase(observers.begin() + location);
    }

    void notifyObservers() {
        for(Observer* obs : observers) {
            obs->update();
        }
    }
};

#endif // OBSERVABLE_H
