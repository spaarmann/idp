/*-----------------
--Konfigurationen--
------------------*/
#include "includes/regconfig.hpp"
#include "includes/wrap_drdaq.hpp"
#include <fstream>

namespace CONFIGURATION{

//Standardwerte

  /*PINKONFIGURATION*/
  //const USB_DRDAQ_INPUTS CH_TEMP =  USB_DRDAQ_CHANNEL_EXT1;
  //const USB_DRDAQ_INPUTS CH_PH =  USB_DRDAQ_CHANNEL_PH;
  int PIN_AIR = 3; //Pin-Nr. nach WiringPi
  int PIN_CO2 = 2;
  /*----------------------------------------*/

  /*Wartezeiten zwischen Updates und Neueinlesen/-berechnungen der Sensoren und Schalten der Geräte */
  int WAITINGTIME = 10; //ms  //Achtung: WAITINGTIME verzögert ALLES (Also auch unsere (pseudo-)PWM) - Deshalb möglichst niedrig setzen!
  //Loggen (Ausgabe Terminal, Senden an Server)
  int LOGWAITINGTIME = 1000; //ms.
  //Sensoren auslesen und Ansteuerung neu berechnen
  int READANDCALCWAITINGTIME= 5000; //ms.

  float PH_TOLERANCE = 0.5;
  float NIGHT_AIM_PH = 6.8;
  float DAY_AIM_PH = 5.5;

  int DAY_START = 60*8; //8 * 60 Min. = 8 Uhr
  int NIGHT_START = 60*22;

  int DAY_RAISE_PH_CO2_ON_TIME_MS = 0;
  int DAY_RAISE_PH_CO2_OFF_TIME_MS = 1000;
  int DAY_REDUCE_PH_CO2_ON_TIME_MS = 3000;
  int DAY_REDUCE_PH_CO2_OFF_TIME_MS = 1000;

  int DAY_RAISE_PH_AIR_ON_TIME_MS = 3000;
  int DAY_RAISE_PH_AIR_OFF_TIME_MS = 1000;
  int DAY_REDUCE_PH_AIR_ON_TIME_MS = 1000;
  int DAY_REDUCE_PH_AIR_OFF_TIME_MS = 3000;

  int NIGHT_RAISE_PH_CO2_ON_TIME_MS = 0;
  int NIGHT_RAISE_PH_CO2_OFF_TIME_MS = 1000;
  int NIGHT_REDUCE_PH_CO2_ON_TIME_MS = 0;
  int NIGHT_REDUCE_PH_CO2_OFF_TIME_MS = 1000;

  int NIGHT_RAISE_PH_AIR_ON_TIME_MS = 3000;
  int NIGHT_RAISE_PH_AIR_OFF_TIME_MS = 1000;
  int NIGHT_REDUCE_PH_AIR_ON_TIME_MS = 1000;
  int NIGHT_REDUCE_PH_AIR_OFF_TIME_MS = 5000;



bool change_variable(std::string s, long int v)
{
    //std::cout << "Variable: " << s << " Value: " << v << std::endl;

    if((s.length() > 0 && s.at(0) == '#') || s.length() == 0 ) //Kommentar ignorieren
        return true;


    if(s == "PIN_AIR")    PIN_AIR = v;
    else if (s == "PIN_CO2")     PIN_CO2 = v;
    else if( s == "WAITINGTIME")     WAITINGTIME = v;
    else if( s == "LOGWAITINGTIME") LOGWAITINGTIME = v;
    else if( s == "READANDCALCWAITINGTIME") READANDCALCWAITINGTIME = v;
    else if( s == "PH_TOLERANCE") PH_TOLERANCE = v/10;
    else if( s == "NIGHT_AIM_PH") NIGHT_AIM_PH = v/10;
    else if( s == "DAY_AIM_PH") DAY_AIM_PH = v/10;
    else if( s == "DAY_START") DAY_START = v;
    else if( s == "DAY_RAISE_PH_CO2_ON_TIME_MS") DAY_RAISE_PH_CO2_ON_TIME_MS = v;
    else if( s == "DAY_RAISE_PH_CO2_OFF_TIME_MS") DAY_RAISE_PH_CO2_OFF_TIME_MS = v;
    else if( s == "DAY_REDUCE_PH_CO2_ON_TIME_MS") DAY_REDUCE_PH_CO2_ON_TIME_MS = v;
    else if( s == "DAY_REDUCE_PH_CO2_OFF_TIME_MS") DAY_REDUCE_PH_CO2_OFF_TIME_MS = v;
    else if( s == "DAY_RAISE_PH_AIR_ON_TIME_MS") DAY_RAISE_PH_AIR_ON_TIME_MS = v;
    else if( s == "DAY_RAISE_PH_AIR_OFF_TIME_MS") DAY_RAISE_PH_AIR_OFF_TIME_MS = v;
    else if( s == "DAY_REDUCE_PH_AIR_ON_TIME_MS") DAY_REDUCE_PH_AIR_ON_TIME_MS = v;
    else if( s == "DAY_REDUCE_PH_AIR_OFF_TIME_MS") DAY_REDUCE_PH_AIR_OFF_TIME_MS = v;
    else if( s == "NIGHT_RAISE_PH_CO2_ON_TIME_MS") NIGHT_RAISE_PH_CO2_ON_TIME_MS = v;
    else if( s == "NIGHT_RAISE_PH_CO2_OFF_TIME_MS") NIGHT_RAISE_PH_CO2_OFF_TIME_MS = v;
    else if( s == "NIGHT_REDUCE_PH_CO2_OFF_TIME_MS") NIGHT_REDUCE_PH_CO2_OFF_TIME_MS  = v;
    else if( s == "NIGHT_REDUCE_PH_CO2_ON_TIME_MS") NIGHT_REDUCE_PH_CO2_ON_TIME_MS = v;
    else if( s == "NIGHT_RAISE_PH_AIR_ON_TIME_MS") NIGHT_RAISE_PH_AIR_ON_TIME_MS = v;
    else if( s == "NIGHT_RAISE_PH_AIR_OFF_TIME_MS") NIGHT_RAISE_PH_AIR_OFF_TIME_MS = v;
    else if( s == "NIGHT_REDUCE_PH_AIR_ON_TIME_MS") NIGHT_REDUCE_PH_AIR_ON_TIME_MS = v;
    else if( s == "NIGHT_REDUCE_PH_AIR_OFF_TIME_MS") NIGHT_REDUCE_PH_AIR_OFF_TIME_MS = v;
    else return false;


    return true;
}


bool read_config()//DATEI: config.txt auslesen und die Variablen (oben) setzen
{
    std::fstream f;
    long int v; //Zahlenwerte
    std::string s, buf; //"Welcher Wert soll geändert werden"

    f.open("config.txt", std::ios::in);

    if( !(f.good()) )//Ein Fehler ist unterlaufen
        return false;

    while (!f.eof())          // Solange noch Daten vorliegen
    {
      std::getline(f, buf);        // Lese eine Zeile
      //std::cout << buf << std::endl;    // Zeige sie auf dem Bildschirm
      if(buf[0] == '#' || buf[0] == ' ' || buf[0] == '\n' || buf.empty() || (! buf.find(' '))) //Kommentar oder leere/ungültige Zeile überspringen
        continue;

      std::size_t spos = buf.find(' ');
      s = buf.substr (0,spos);
      v = std::stol(buf.substr(spos));

      change_variable(s,v);
    }

    f.close();                // Datei wieder schließen

    return true;
}

void print_configuration()
{
    std::cout << "PIN_AIR: " << PIN_AIR << std::endl;
    std::cout << "PIN_CO2: " << PIN_CO2 << std::endl;
    std::cout << "WAITINGTIME: " << WAITINGTIME << std::endl;
    std::cout << "LOGWAITINGTIME: " << LOGWAITINGTIME << std::endl;
    std::cout << "READANDCALCWAITINGTIME: " << READANDCALCWAITINGTIME << std::endl;

    std::cout << "PH_TOLERANCE: " << PH_TOLERANCE << std::endl;
    std::cout << "NIGHT_AIM_PH: " << NIGHT_AIM_PH   << std::endl;
    std::cout <<  "DAY_AIM_PH:   " << DAY_AIM_PH    << std::endl;

    std::cout <<  "DAY_START: " << DAY_START << std::endl;
    std::cout <<  "NIGHT_START: " << NIGHT_START  << std::endl;

    std::cout <<  "DAY_RAISE_PH_CO2_ON_TIME_MS " << DAY_RAISE_PH_CO2_ON_TIME_MS << std::endl;
    std::cout <<  "DAY_RAISE_PH_CO2_OFF_TIME_MS " << DAY_RAISE_PH_CO2_OFF_TIME_MS << std::endl;
    std::cout <<  "DAY_REDUCE_PH_CO2_ON_TIME_MS " << DAY_REDUCE_PH_CO2_ON_TIME_MS << std::endl;
    std::cout << "DAY_REDUCE_PH_CO2_OFF_TIME_MS " <<  DAY_REDUCE_PH_CO2_OFF_TIME_MS << std::endl;

    std::cout << "DAY_RAISE_PH_AIR_ON_TIME_MS " <<  DAY_RAISE_PH_AIR_ON_TIME_MS << std::endl;
    std::cout << "DAY_RAISE_PH_AIR_OFF_TIME_MS " <<  DAY_RAISE_PH_AIR_OFF_TIME_MS << std::endl;
    std::cout <<  "DAY_REDUCE_PH_AIR_ON_TIME_MS " << DAY_REDUCE_PH_AIR_ON_TIME_MS << std::endl;
    std::cout <<  "DAY_REDUCE_PH_AIR_OFF_TIME_MS " << DAY_REDUCE_PH_AIR_OFF_TIME_MS << std::endl;

    std::cout << "NIGHT_RAISE_PH_CO2_ON_TIME_MS " <<  NIGHT_RAISE_PH_CO2_ON_TIME_MS << std::endl;
    std::cout << "NIGHT_RAISE_PH_CO2_OFF_TIME_MS " <<  NIGHT_RAISE_PH_CO2_OFF_TIME_MS << std::endl;
    std::cout <<  "NIGHT_REDUCE_PH_CO2_ON_TIME_MS " << NIGHT_REDUCE_PH_CO2_ON_TIME_MS << std::endl;
    std::cout <<  "NIGHT_REDUCE_PH_CO2_OFF_TIME_MS " << NIGHT_REDUCE_PH_CO2_OFF_TIME_MS << std::endl;

    std::cout << "NIGHT_RAISE_PH_AIR_ON_TIME_MS " <<  NIGHT_RAISE_PH_AIR_ON_TIME_MS << std::endl;
    std::cout << "NIGHT_RAISE_PH_AIR_OFF_TIME_MS " <<  NIGHT_RAISE_PH_AIR_OFF_TIME_MS << std::endl;
    std::cout <<  "NIGHT_REDUCE_PH_AIR_ON_TIME_MS " << NIGHT_REDUCE_PH_AIR_ON_TIME_MS << std::endl;
    std::cout << "NIGHT_REDUCE_PH_AIR_OFF_TIME_MS " <<  NIGHT_REDUCE_PH_AIR_OFF_TIME_MS << std::endl;


}

/*void load_default()
{
  //PINKONFIGURATION
  //const USB_DRDAQ_INPUTS CH_TEMP =  USB_DRDAQ_CHANNEL_EXT1;
  //const USB_DRDAQ_INPUTS CH_PH =  USB_DRDAQ_CHANNEL_PH;
  PIN_AIR = 3; //Pin-Nr. nach WiringPi
  PIN_CO2 = 2;
  //----------------------------------------

  //Wartezeiten zwischen Updates und Neueinlesen/-berechnungen der Sensoren und Schalten der Geräte
  WAITINGTIME = 10; //ms  //Achtung: WAITINGTIME verzögert ALLES (Also auch unsere (pseudo-)PWM) - Deshalb möglichst niedrig setzen!
  //Loggen (Ausgabe Terminal, Senden an Server)
  LOGWAITINGTIME = 1000; //ms.
  //Sensoren auslesen und Ansteuerung neu berechnen
  READANDCALCWAITINGTIME= 5000; //ms.
}
*/

}
