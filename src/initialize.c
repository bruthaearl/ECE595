#include "enums_structs_etc.h"

void initialize(tempstuff_t *tempStuff, sysstuff_t *sysStuff){

    tempStuff->heater = 1;
    tempStuff->temp = 666;


    sysStuff->next_state = READ_THERMOCOUPLE;


}

