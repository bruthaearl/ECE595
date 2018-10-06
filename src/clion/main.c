#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include "enums_structs_etc.h"
#include "function_decs.h"


tempstuff_t tempStuff;
sysstuff_t sysStuff;

int main() {

    int setpoint = 45;

    sysStuff.state = INITIALIZE;
    while (1) {

        switch (sysStuff.state) {

            case INITIALIZE :
                initialize(&tempStuff, &sysStuff);
                break;
            case READ_THERMOCOUPLE :
                getTemp(&tempStuff, &sysStuff);
                break;
            case MAKE_DECISION :
                makeDecision(&tempStuff, &sysStuff, setpoint);
                break;
            case DEBUG:
                printf("current state %d\n", sysStuff.state);
                printf("setpoint: %d\n", setpoint);
                printf("temperature reading: %d\n", tempStuff.temp);
                printf("heater status %d\n", tempStuff.heater);
                sleep(5);
                sysStuff.next_state = READ_THERMOCOUPLE;
                break;
            default :
                exit(20);
        }

        sysStuff.state = sysStuff.next_state;


    }


}

