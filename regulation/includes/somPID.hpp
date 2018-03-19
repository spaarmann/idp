#pragma once


/*Something like PID-Regulation*/
class somPID{
    public:
        somPID(int aim = 0, int hysterese = 0);
        void setAim(int aim, int hysterese, bool reset = false);
        int calculate(int meas);

    private:
        void reset();

        int aim;
        int hysterese;
        int lastmeas;
        int lastval;  //"value" = An oder Aus / Ansteuerung / (evtl. "Stärke")

};
