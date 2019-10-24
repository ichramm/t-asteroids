#ifndef CONDITION_H_INCLUDED
#define CONDITION_H_INCLUDED
class Condicionable;

class Condition
{
public:
    virtual bool cumple(Condicionable*) = 0;
    virtual ~Condition(){};
};

#endif // CONDITION_H_INCLUDED
