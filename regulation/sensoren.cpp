#include "includes/sensoren.hpp"
#include "includes/wrap_drdaq.hpp"
#include <iostream>

Sensor::Sensor()
{

}

float pico_Sensor::getValue()
{
    this->updateValue();
    return this->lastValue;
}

pico_Sensor::pico_Sensor(PICO_HANDLER *picohandler, USB_DRDAQ_INPUTS channelNumber)
{
    this->picohandler = picohandler;
    this->chNumber = channelNumber;


    PICO_STATUS status = UsbDrDaqGetChannelInfo(picohandler->handle, nullptr, nullptr, nullptr, &divider, chNumber);
    is_pico_ok(status, true); //Evtl. Fehler ausgeben
    std::cout << "Divider: " << divider << std::endl;
}

void pico_Sensor::updateValue()
{
    PICO_STATUS status;
    int16_t value;
    uint16_t overflow;

    status = UsbDrDaqGetSingle(picohandler->handle, chNumber, &value, &overflow);

    if(is_pico_ok(status, true))
    {
        if(overflow)
                std::cout <<"Sensor:Es trat ein \"Overflow\" auf\n";

        this->lastValue = (float)value / divider;
    }
}
