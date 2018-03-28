/*-----------------
--Konfigurationen--
------------------*/

#pragma once
#include "wrap_drdaq.hpp"
#include <fstream>

namespace CONFIGURATION{

//Konfigurationsvariablen
//------------------------------------------------------------------------------------------------------------------------------------------
/*PINKONFIGURATION*/
const USB_DRDAQ_INPUTS CH_TEMP =  USB_DRDAQ_CHANNEL_EXT1;
const USB_DRDAQ_INPUTS CH_PH =  USB_DRDAQ_CHANNEL_PH;
extern int PIN_AIR;// = 3; //Pin-Nr. nach WiringPi
extern int PIN_CO2;// = 2;
/*----------------------------------------*/

/*Wartezeiten zwischen Updates und Neueinlesen/-berechnungen der Sensoren und Schalten der Geräte */
extern int WAITINGTIME;// = 10; //ms  //Achtung: WAITINGTIME verzögert ALLES (Also auch unsere (pseudo-)PWM) - Deshalb möglichst niedrig setzen!
//Loggen (Ausgabe Terminal, Senden an Server)
extern int LOGWAITINGTIME;// = 1000; //ms.
//Sensoren auslesen und Ansteuerung neu berechnen
extern int READANDCALCWAITINGTIME;// = 5000; //ms.

extern float TEST_PH_CURRENT;

extern float PH_TOLERANCE;
extern float NIGHT_AIM_PH;
extern float DAY_AIM_PH;

extern int DAY_START;
extern int NIGHT_START;

extern int DAY_RAISE_PH_CO2_ON_TIME_MS;
extern int DAY_RAISE_PH_CO2_OFF_TIME_MS;
extern int DAY_REDUCE_PH_CO2_ON_TIME_MS;
extern int DAY_REDUCE_PH_CO2_OFF_TIME_MS;

extern int DAY_RAISE_PH_AIR_ON_TIME_MS;
extern int DAY_RAISE_PH_AIR_OFF_TIME_MS;
extern int DAY_REDUCE_PH_AIR_ON_TIME_MS;
extern int DAY_REDUCE_PH_AIR_OFF_TIME_MS;


extern int NIGHT_RAISE_PH_CO2_ON_TIME_MS;
extern int NIGHT_RAISE_PH_CO2_OFF_TIME_MS;
extern int NIGHT_REDUCE_PH_CO2_ON_TIME_MS;
extern int NIGHT_REDUCE_PH_CO2_OFF_TIME_MS;

extern int NIGHT_RAISE_PH_AIR_ON_TIME_MS;
extern int NIGHT_RAISE_PH_AIR_OFF_TIME_MS;
extern int NIGHT_REDUCE_PH_AIR_ON_TIME_MS;
extern int NIGHT_REDUCE_PH_AIR_OFF_TIME_MS;

//------------------------------------------------------------------------------------------------------------------------------------------


//Funktionsprototypen/deklarationen
bool change_variable(std::string s, long int v);


bool read_config();//DATEI: config.txt auslesen und die Variablen (oben) setzen


void print_configuration();

}
