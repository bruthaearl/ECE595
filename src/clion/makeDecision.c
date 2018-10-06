#include "enums_structs_etc.h"
#include <stdio.h>

void makeDecision(tempstuff_t *tempStuff, sysstuff_t *sysStuff, int setpoint){

    FILE *temp_file;
    temp_file = fopen("/tmp/status", "w");



    if (tempStuff->temp >= setpoint){

        tempStuff->heater = OFF;
        fprintf(temp_file, "%s", "OFF");

    } else {

        tempStuff->heater = ON;
        fprintf(temp_file, "%s", "ON");
    }

    fclose(temp_file);

    sysStuff->next_state = DEBUG;

}