#ifndef DELAYER_H
#define DELAYER_H

#endif // DELAYER_H

#include "IDelayer.h"
#include <QThread>
class Delayer : public IDelayer{
private:
    unsigned int seconds;
public:
    // конструктор по значению
    Delayer(unsigned int s) : seconds(s) {}
    // конструктор по умолчанию
    Delayer(){ seconds = 1; }
    // деструктор
    ~Delayer(){}
    void wait(){
        QThread::sleep(seconds);
    }
};
