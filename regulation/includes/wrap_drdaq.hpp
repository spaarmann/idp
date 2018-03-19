#pragma once

#include <libusbdrdaq-1.0/PicoStatus.h>
#include <libusbdrdaq-1.0/usbDrDaqApi.h>

#include <iostream>

typedef struct pico_handler{
            int16_t handle;
} PICO_HANDLER;

bool pico_general_init(PICO_HANDLER *picohandler);
bool is_pico_ok(PICO_STATUS status, bool printError=false);
void LED(PICO_HANDLER picohandler, short enable,unsigned short red, unsigned short green, unsigned short blue);

#define LED_SHOWGREEN(handle) LED((handle), 1, 0, 255, 0)
#define LED_SHOWBLUE(handle) LED((handle), 1, 0, 0, 255)
#define LED_SHOWRED(handle) LED((handle), 1, 255, 0, 0)
#define LED_OFF(handle) LED((handle), 0)
