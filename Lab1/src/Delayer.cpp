#include "Delayer.h"


// конструктор по значению
Delayer::Delayer(unsigned int s) { seconds = (s<1)? 1: s; }

// конструктор по умолчанию
Delayer::Delayer(){ seconds = 1; }

// деструктор
Delayer::~Delayer(){}

// функция задержки
void Delayer::wait(){ QThread::sleep(seconds); }

