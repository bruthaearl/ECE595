#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>

#include "function_decs.h"
#include "enums_structs_etc.h"


void getTemp(tempstuff_t *tempStuff, sysstuff_t *sysStuff){

    int temp;
    FILE *temp_file;
    temp_file = fopen("/tmp/temp", "r");
    if(temp_file == NULL){
        syslog(LOG_ERR, ERROR_FORMAT, "Error opening temperature file. Exiting");
        exit(-1);
    }
    fscanf(temp_file, "%d", &temp);
    fclose(temp_file);
    tempStuff->temp = temp;

    sysStuff->next_state = READ_SETPOINTS;


}
