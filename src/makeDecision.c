#include "enums_structs_etc.h"
#include <stdio.h>
#include <time.h>
#include <syslog.h>


void makeDecision(tempstuff_t *tempStuff, sysstuff_t *sysStuff, setpoints_t *setpointdb){

    // Get time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    //Does read setpoint match loaded setpoint? If not, setpoint was set manually
    if(tempStuff->setpoint != tempStuff->read_setpoint){
        tempStuff->setpoint = tempStuff->read_setpoint;
        syslog(LOG_INFO, "New manual setpoint: %d", tempStuff->setpoint);
    };

    // Compare current time to scheduled times
    for(int i = 0; i<setpointdb->count; i++){
        if( (setpointdb->time_h[i] == tm.tm_hour) && (setpointdb->time_m[i] == tm.tm_min) ){
            tempStuff->setpoint = setpointdb->temp[i];
            syslog(LOG_INFO, "New scheduled setpoint: %d", tempStuff->setpoint);
        }
    }

    // Open status file for write
    FILE *temp_file;
    temp_file = fopen("/tmp/status", "w");

    // If temperature is hotter than the setpoint, turn off heater.
    if (tempStuff->temp >= tempStuff->setpoint){
        tempStuff->heater = OFF;
        fprintf(temp_file, "%s", "OFF");
    } else {
        tempStuff->heater = ON;
        fprintf(temp_file, "%s", "ON");
    }

    fclose(temp_file);

    sysStuff->next_state = PUBLISH_TO_SERVER;

}