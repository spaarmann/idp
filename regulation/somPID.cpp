#include "includes/somPID.hpp"

somPID::somPID(int aim, int hysterese)
{
    this->aim = aim;
    this->hysterese = hysterese;
    this->reset();
}

void somPID::reset()
{
    //lperiod = 0;
    lastmeas = 0;
    lastval = 0;
}

void somPID::setAim(int aim, int hysterese, bool reset)
{
    if(reset) this->reset();
    this->aim = aim;
    this->hysterese = hysterese;
}

int somPID::calculate(int meas)
{
    int value = lastval;

    if(lastval == 0 && meas < hysterese)
    {
      value = 1;
    }
    else if(lastval == 1 && meas > aim)
    {
      value = 0;
    }

    //lastmeas = meas;
    lastval = value;
    return value;
}
