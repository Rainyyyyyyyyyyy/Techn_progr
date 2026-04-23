#ifndef DELAYER_H
#define DELAYER_H


#include "IDelayer.h"
#include <QThread>


class Delayer : public IDelayer{
private:
    // количество секунд для задержки в wait()
    unsigned int seconds;
public:
    // конструктор по значению
    Delayer(unsigned int s);
    // конструктор по умолчанию
    Delayer();
    // деструктор
    ~Delayer();
    // функция задержки
    void wait();
};

#endif // DELAYER_H
