#ifndef IDELAYER_H
#define IDELAYER_H



class IDelayer{
public:
    virtual ~IDelayer() = default;
    virtual void wait() = 0;
};


#endif // IDELAYER_H
