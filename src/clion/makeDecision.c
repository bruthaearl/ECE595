#include "enums_structs_etc.h"
#include <stdio.h>
#include <time.h>


void makeDecision(tempstuff_t *tempStuff, sysstuff_t *sysStuff, setpoints_t *setpointdb){


    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("time is %d %d \n", tm.tm_hour, tm.tm_min);

    for(int i = 0; i<setpointdb->count; i++){

        if( (setpointdb->time_h[i] == tm.tm_hour) && (setpointdb->time_m[i] == tm.tm_min) ){
            tempStuff->setpoint = setpointdb->temp[i];
        }

    }

    FILE *temp_file;
    temp_file = fopen("/tmp/status", "w");


    if (tempStuff->temp >= tempStuff->setpoint){

        tempStuff->heater = OFF;
        fprintf(temp_file, "%s", "OFF");

    } else {

        tempStuff->heater = ON;
        fprintf(temp_file, "%s", "ON");
    }

    fclose(temp_file);

    sysStuff->next_state = DEBUG;

}