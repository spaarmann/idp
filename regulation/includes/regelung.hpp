#pragma once
#include "sensoren.hpp"
#include "logger.hpp"
#include "somPID.hpp"
#include "pseudo_pwm.hpp"
#include <ctime>

using namespace std;

class Regelung
{
    public:
        Regelung(bool testMode);
        ~Regelung();
        void update();
        bool isRunning();

    private:
        void readSensors();
        void calculateState();
        void performState(long double now);
        void logState();

        bool testMode;

        bool air;
        bool co2;

        PSEUDO_PWM *pwmAir;
        PSEUDO_PWM *pwmCo2;

        float tempValue;
        float phValue;

        somPID *pidAir;
        somPID *pidCo2;


        PICO_HANDLER picohandler;
        pico_Sensor *tempSensor;
        pico_Sensor *phSensor;

        Output *output;

        bool running;

        //Zeiten berechnen / Differenzen...
         long double lastLog;
         long double lastCalc;
         long double lastPerform;
         long double lastRead;


};
