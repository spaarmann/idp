#pragma once

/*PWM / Nur bei Zeiten über ~0.1 Sek... Ansonsten WiringPi-PWM-Funktionen für schnellere PWM nutzen!...*/

/*Test - PWM ohne Thread*/
class PSEUDO_PWM{

  public:
  PSEUDO_PWM(int pin);

  bool run_pwm; //Funktion soll weiter laufen / AN?

  double on_time_ms;  //Zeit wie lange .. an ist
  double off_time_ms; //Zeit  wie lange .. aus ist

  /*Parameter now: Aktuelle Zeit in Millisekunden*/
  void run_update(long double now);

  void change_pin(int pin);

  private:
  int pin;
  bool is_on; //Aktueller status des Pins
  long double last_changed;

};
