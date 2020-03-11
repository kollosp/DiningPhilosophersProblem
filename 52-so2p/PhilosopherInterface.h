#ifndef PHILOSOPHERINTERFACE_H
#define PHILOSOPHERINTERFACE_H


class PhilosopherInterface
{
public:
    PhilosopherInterface();void init();

    virtual void run() = 0;
    virtual int getState() const = 0;

    virtual float getLeftThinkTime() const = 0;
    virtual float getLeftEatTime() const = 0;
    virtual float getLeftLiveTime() const = 0;

    virtual void startEating() = 0;
};

#endif // PHILOSOPHERINTERFACE_H
