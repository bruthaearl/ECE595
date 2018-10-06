//
// Created by jake on 10/6/18.
//
#define BUFFER_LENGTH 100

#include <stdio.h>
#include "enums_structs_etc.h"
#include "function_decs.h"

void readSetpoints(sysstuff_t *sysStuff){

    char db_buffer[2000];

    FILE *temp_file;
    temp_file = fopen(DB_PATH, "r");

    while(fgets(db_buffer, 2000, temp_file)) {
        printf("%s\n", db_buffer);
    }
    fclose(temp_file);

    parse_array(cJSON_GetObjectItem(cJSON_Parse(db_buffer),"setpoints"));

    sysStuff->next_state = MAKE_DECISION;
}

void parse_array(cJSON *array) {
    cJSON *item = array ? array->child : 0;
    while (item)
    {
        cJSON *temp = cJSON_GetObjectItem(item, "temp");
        cJSON *time_h = cJSON_GetObjectItem(item, "time_h");
        cJSON *time_m = cJSON_GetObjectItem(item, "time_m");

        item=item->next;
    }
}