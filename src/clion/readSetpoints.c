//
// Created by jake on 10/6/18.
//
#define BUFFER_LENGTH 100

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "enums_structs_etc.h"
#include "function_decs.h"

void readSetpoints(sysstuff_t *sysStuff, setpoints_t *setpointdb){

    char *db_buffer = 0;
    long length;

    //open the file
    FILE *temp_file = fopen(DB_PATH, "r");

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
    }

    printf("%s\n", db_buffer);

    // parse the file
    cJSON *root = cJSON_Parse(db_buffer);
    cJSON *setpoints = cJSON_GetObjectItem(root, "setpoints");
    int setpoints_count = cJSON_GetArraySize(setpoints);

    int i;
    setpointdb->count = setpoints_count;
    //load setpoints into struct
    for(i=0; i<setpoints_count && i<MAX_SETPOINTS; i++){
        printf("temp: \n");
        cJSON *setpoint = cJSON_GetArrayItem(setpoints, i);
        int temp = cJSON_GetObjectItem(setpoint, "temp")->valueint;
        setpointdb->temp[i] = cJSON_GetObjectItem(setpoint, "temp")->valueint;
        setpointdb->time_h[i] = cJSON_GetObjectItem(setpoint, "time_h")->valueint;
        setpointdb->time_m[i] = cJSON_GetObjectItem(setpoint, "time_m")->valueint;
        printf("%d %d %d \n", setpointdb->temp[i], setpointdb->time_h[i], setpointdb->time_m[i]);
    }

    //fill the remaining with -1
    printf("setpoint_count: %d\n", setpoints_count);
    for(i = setpoints_count+1; i<MAX_SETPOINTS; i++){
        setpointdb->temp[i] = -1;
        setpointdb->time_h[i] = -1;
        setpointdb->time_m[i] = -1;
        printf("i: %d\n", i);
    }


    cJSON_Delete(root);
    sysStuff->next_state = MAKE_DECISION;
}

