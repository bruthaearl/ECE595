//
// Created by jake on 10/6/18.
//
#define BUFFER_LENGTH 100

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdlib.h>
#include "enums_structs_etc.h"
#include "function_decs.h"

void readSetpoints(sysstuff_t *sysStuff, setpoints_t *setpointdb, tempstuff_t *tempStuff){

    char *db_buffer = 0;
    long length;

    //open the file
    FILE *temp_file = fopen(DB_PATH, "r");

    // load file into buffer, zero-terminate
    if (temp_file) {
        fseek (temp_file, 0, SEEK_END);
        length = ftell (temp_file);
        fseek (temp_file, 0, SEEK_SET);
        db_buffer = malloc(length + 1);
        if (db_buffer)
        {
            fread (db_buffer, 1, length+1, temp_file);
            db_buffer[length] = '\0';
        }

        fclose (temp_file);
    } else{
        syslog(LOG_ERR, ERROR_FORMAT, "Error opening database file. Exiting");
        exit(-1);
    }

    // parse the file with cJSON
    cJSON *root = cJSON_Parse(db_buffer);
    if(root == NULL){
        syslog(LOG_ERR, ERROR_FORMAT, "Error: database not in correct format");
    }
    cJSON *setpoints = cJSON_GetObjectItem(root, "setpoints");
    int setpoints_count = cJSON_GetArraySize(setpoints);

    int i;
    setpointdb->count = setpoints_count;
    //load setpoints into struct
    for(i=0; i<setpoints_count && i<MAX_SETPOINTS; i++){
        cJSON *setpoint = cJSON_GetArrayItem(setpoints, i);
        setpointdb->temp[i] = cJSON_GetObjectItem(setpoint, "temp")->valueint;
        setpointdb->time_h[i] = cJSON_GetObjectItem(setpoint, "time_h")->valueint;
        setpointdb->time_m[i] = cJSON_GetObjectItem(setpoint, "time_m")->valueint;
  //      printf("%d %d %d \n", setpointdb->temp[i], setpointdb->time_h[i], setpointdb->time_m[i]);
    }

    //fill the remaining with -1
    printf("setpoint_count: %d\n", setpoints_count);
    for(i = setpoints_count+1; i<MAX_SETPOINTS; i++){
        setpointdb->temp[i] = -1;
        setpointdb->time_h[i] = -1;
        setpointdb->time_m[i] = -1;
    }

    //read manual setpoint
    cJSON *stats = cJSON_GetObjectItem(root, "stats");
    cJSON *stat = cJSON_GetArrayItem(stats, 0);
    tempStuff->read_setpoint = cJSON_GetObjectItem(stat, "current_setpoint")->valueint;
    printf("read setpoint: %d\n", tempStuff->read_setpoint);

    cJSON_Delete(root);

    sysStuff->next_state = MAKE_DECISION;
}

