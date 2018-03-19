#include "includes/pseudo_pwm.hpp"
#include <wiringPi.h>

PSEUDO_PWM::PSEUDO_PWM(int pin)
{
    this->pin = pin;
    is_on = false;
    last_changed = 0;
    run_pwm = false;
    on_time_ms = 0;
    off_time_ms = 0;
}

void PSEUDO_PWM::run_update(long double now)
{
    if (run_pwm)
    {
        long double tdiff = now - last_changed;

        if (is_on && off_time_ms > 0 && tdiff > on_time_ms) //Gas "lange genug an" -> muss ausgeschaltet werden
        {
            digitalWrite(pin, false);

            last_changed = now;
            is_on = false;
        }
        else if (!is_on && on_time_ms > 0 && tdiff > off_time_ms)
        {
            digitalWrite(pin, true);

            last_changed = now;
            is_on = true;
        }
    }
  }
