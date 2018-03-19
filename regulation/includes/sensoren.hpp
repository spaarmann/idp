#pragma once
#include "wrap_drdaq.hpp"

class Sensor
{
    public:
        Sensor();
        //~Sensor();
        float getValue();

    protected:
        //virtual void updateValue();
        float lastValue;
};


class pico_Sensor : public Sensor
{
    public:
        pico_Sensor(PICO_HANDLER *picohandler, USB_DRDAQ_INPUTS channelNumber);
        ~pico_Sensor();
        void updateValue();
        float getValue();
    protected:


    private:
    PICO_HANDLER *picohandler;
    USB_DRDAQ_INPUTS chNumber;

    int16_t divider;

};
