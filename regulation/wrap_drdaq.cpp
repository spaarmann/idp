#include "includes/wrap_drdaq.hpp"



bool pico_general_init(PICO_HANDLER *picohandler)
{
    PICO_STATUS status;
    status = UsbDrDaqOpenUnit(&picohandler->handle);

    return is_pico_ok(status, true);
}

bool is_pico_ok(PICO_STATUS status, bool printError)
{
    switch(status)
    {
        case PICO_OK:
            return true;
            break;
        /*case PICO_INVALID_HANDLE:
            //break;
        case PICO_NO_SAMPLES_AVAILABLE:
            //break;
        case PICO_DEVICE_SAMPLING:
            //break;
        case PICO_NULL_PARAMETER:
            //break;
        case PICO_INVALID_PARAMETER:
            //break;
        case PICO_DATA_NOT_AVAILABLE:
            //break;
        case PICO_INVALID_CALL:
           // break;
        case PICO_NOT_RESPONDING:
          //  break;
        case PICO_MEMORY:
           // break;
           */

        default:
            if(printError)std::cout << "PICO: Ein Fehler ist aufgetreten: Errorcode:" << status << " Siehe Dokumentation/PICO_STATUS für Hinweise\n" << std::endl;
            return false;
    }

}


void LED(int16_t handle, short enable,unsigned short red = 0, unsigned short green = 0, unsigned short blue = 0)
{
    UsbDrDaqEnableRGBLED(handle, (short)enable);
    if(enable)
        UsbDrDaqSetRGBLED(handle, red, green, blue);

}
