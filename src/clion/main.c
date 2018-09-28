#include <stdio.h>
#include <stdlib.h>
#include "enums_structs_etc.h"
#include "function_decs.h"


tempstuff_t tempStuff;
sysstuff_t sysStuff;
sm_state next_state;

int main() {

    sysStuff.state = INITIALIZE;
    while (1) {

        switch (sysStuff.state) {

            case INITIALIZE :
                initialize(&tempStuff, &sysStuff);
                break;
            case READ_THERMOCOUPLE :
                getTemp(&tempStuff, &sysStuff);
                break;
            case UHBORT :
                printf("current state %d\n", sysStuff.state);
                printf("temperature reading: %d\n", tempStuff.temp);
                printf("heater status %d\n", tempStuff.heater);
                exit(10);
            default :
                exit(20);
        }

        sysStuff.state = sysStuff.next_state;


    }


}

