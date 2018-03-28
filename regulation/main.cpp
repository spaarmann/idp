#include "includes/regelung.hpp"
#include "includes/wrap_delay.hpp"
#include "includes/regconfig.hpp"

int main(int argc, char **argv)
{
    CONFIGURATION::read_config();
    CONFIGURATION::print_configuration();

    bool testMode = argc == 2;

    Regelung regelung(testMode); //Regelung ist unsere Klasse für Alles!

    while(regelung.isRunning())
    {
        regelung.update();
        delay(CONFIGURATION::WAITINGTIME);
    }

    return 0;
}
