#include <stdio.h>
#include "enums_structs_etc.h"

void getTemp(tempstuff_t *tempStuff, sysstuff_t *sysStuff){

    int temp;
    FILE *temp_file;
    temp_file = fopen("/tmp/temp", "r");

    fscanf(temp_file, "%d", &temp);
    fclose(temp_file);
    tempStuff->temp = temp;

    sysStuff->next_state = READ_SETPOINTS;


}
