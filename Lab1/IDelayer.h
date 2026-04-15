#ifndef IDELAYER_H
#define IDELAYER_H

#endif // IDELAYER_H


class IDelayer{
public:
    virtual ~IDelayer() = default;
    //IDelayer() = default;
    virtual void wait() = 0;
};
