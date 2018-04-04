#include "includes/regelung.hpp"
#include <wiringPi.h>
#include "includes/wrap_drdaq.hpp"
#include "includes/regconfig.hpp"
#include "includes/somPID.hpp"
#include <string>
#include <iostream>
#include "includes/pseudo_pwm.hpp"
#include <ctime>
#include "includes/Date.h"

Regelung::Regelung(bool testMode) //Konstruktor, Alles initalisieren
{
    air = false;
    co2 = false;

    this->testMode = testMode;

    tempValue = 0;
    phValue = 0;

    if (!testMode) {
        if(! pico_general_init(&picohandler))
            running = false;


        tempSensor = new pico_Sensor(&picohandler, CONFIGURATION::CH_TEMP);
        phSensor = new pico_Sensor(&picohandler, CONFIGURATION::CH_PH);
    }

    pidAir =  new somPID(60, 55);
    pidCo2 = new somPID(60, 55);

    wiringPiSetup () ;
    pinMode (CONFIGURATION::PIN_AIR, OUTPUT);
    pinMode (CONFIGURATION::PIN_CO2, OUTPUT);

    digitalWrite (CONFIGURATION::PIN_AIR, false) ;
    digitalWrite (CONFIGURATION::PIN_CO2, false) ;

    output = new Output("uploader_input");

    running = true;

    pwmAir = new PSEUDO_PWM(CONFIGURATION::PIN_AIR);
    pwmCo2 = new PSEUDO_PWM(CONFIGURATION::PIN_CO2);

    /*Initalisiere pseudo PWM ------*/
    pwmAir->run_pwm = true;
    pwmCo2->run_pwm = true;

    //Test-werte
    pwmAir->on_time_ms = 0;  //Zeit wie lange Luft an ist
    pwmAir->off_time_ms = 0; //Zeit wie lange Luft aus ist
    pwmCo2->on_time_ms = 0; //Zeit wie lange Co2 an ist
    pwmCo2->off_time_ms = 0; //...aus ist

    /*-----------------------------*/
}

Regelung::~Regelung()
{
    if (!testMode)
        UsbDrDaqCloseUnit(picohandler.handle);
    
    delete output;
    delete pidAir;
    delete pidCo2;
}

bool Regelung::isRunning()
{
    return running;
}

void Regelung::update()
{

    /*Aktuelle Uhrzeit in Millisekunden erhalten -> in now speichern*/
    struct timespec ts_current;
    clock_gettime(CLOCK_MONOTONIC, &ts_current);
    long double now = ts_current.tv_sec*1000 + ((long double) ts_current.tv_nsec/1000000); //Millisekunden an now übergeben

    long double tdiff;  //Temporäre Variable um Zeitdifferenzen zu berechnen

    tdiff = now - lastCalc;
    if(tdiff > CONFIGURATION::READANDCALCWAITINGTIME)
    {
      readSensors();
      calculateState();
      lastCalc = now;
    }

    //performState immer ausführen! Nötig für schnelle PWM!!!
    performState(now);

    tdiff = now - lastLog;

    if(tdiff > CONFIGURATION::LOGWAITINGTIME)
    {
      logState();
      lastLog = now;
    }
}

void Regelung::readSensors()
{
    if (!testMode) {
        tempValue = tempSensor->getValue();
        phValue = phSensor->getValue();
    } else {
        tempValue = 20;
        phValue = CONFIGURATION::TEST_PH_CURRENT;
    }
}

void Regelung::calculateState()
{
    //Tag-Nacht setzen/berechnen
    /*Gewünschte Tag-Nacht-Zeiten sind in der Konfiguration:
    int CONFIGURATION::DAY_START;
    int CONFIGURATION::NIGHT_START;

    Die Angaben sind in Minuten (8:30 Uhr wäre z.B.: 8*60 + 30 = 510)
    */
    bool Nacht = false;
    bool Tag = true;

    if ((Nacht) && phValue < (CONFIGURATION::NIGHT_AIM_PH - CONFIGURATION::PH_TOLERANCE) )
    {
      pwmAir->on_time_ms = CONFIGURATION::NIGHT_RAISE_PH_AIR_ON_TIME_MS;
      pwmAir->off_time_ms = CONFIGURATION::NIGHT_RAISE_PH_AIR_OFF_TIME_MS;
      pwmCo2->on_time_ms = CONFIGURATION::NIGHT_RAISE_PH_CO2_ON_TIME_MS;
      pwmCo2->off_time_ms = CONFIGURATION::NIGHT_RAISE_PH_CO2_ON_TIME_MS;
    }
    else if ((Nacht) && phValue > (CONFIGURATION::NIGHT_AIM_PH + CONFIGURATION::PH_TOLERANCE) )
    {
      pwmAir->on_time_ms = CONFIGURATION::NIGHT_REDUCE_PH_AIR_ON_TIME_MS;
      pwmAir->off_time_ms = CONFIGURATION::NIGHT_REDUCE_PH_AIR_OFF_TIME_MS;
      pwmCo2->on_time_ms = CONFIGURATION::NIGHT_REDUCE_PH_CO2_ON_TIME_MS;
      pwmCo2->off_time_ms = CONFIGURATION::NIGHT_REDUCE_PH_CO2_ON_TIME_MS;
    }
    else if ((Tag) && phValue < (CONFIGURATION::DAY_AIM_PH - CONFIGURATION::PH_TOLERANCE) )
    {
      pwmAir->on_time_ms = CONFIGURATION::DAY_RAISE_PH_AIR_ON_TIME_MS;
      pwmAir->off_time_ms = CONFIGURATION::DAY_RAISE_PH_AIR_OFF_TIME_MS;
      pwmCo2->on_time_ms = CONFIGURATION::DAY_RAISE_PH_CO2_ON_TIME_MS;
      pwmCo2->off_time_ms = CONFIGURATION::DAY_RAISE_PH_CO2_ON_TIME_MS;
    }
    else if ((Tag) && phValue > (CONFIGURATION::DAY_AIM_PH + CONFIGURATION::PH_TOLERANCE) )
    {
      pwmAir->on_time_ms = CONFIGURATION::DAY_REDUCE_PH_AIR_ON_TIME_MS;
      pwmAir->off_time_ms = CONFIGURATION::DAY_REDUCE_PH_AIR_OFF_TIME_MS;
      pwmCo2->on_time_ms = CONFIGURATION::DAY_REDUCE_PH_CO2_ON_TIME_MS;
      pwmCo2->off_time_ms = CONFIGURATION::DAY_REDUCE_PH_CO2_ON_TIME_MS;
    }
}

void Regelung::performState(long double now)
{
      /*PWM Ansteuerung!*/
    pwmAir->run_update(now);
    pwmCo2->run_update(now);
}

void Regelung::logState()
{
    output->Log(this->tempValue, this->phValue);
}
