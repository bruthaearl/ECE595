#include <cjson/cJSON.h>
#include "enums_structs_etc.h"
#ifndef CLION_FUNCTION_DECS_H
#define CLION_FUNCTION_DECS_H

#define DB_PATH "/home/jake/school/introIOT/project/res/json-server/json/jsondb.json"

void initialize(tempstuff_t *tempStuff, sysstuff_t *sysStuff);
void getTemp(tempstuff_t *tempStuff, sysstuff_t *sysStuff);
void makeDecision(tempstuff_t *tempStuff, sysstuff_t *sysStuff, int setpoint);
void readSetpoints(sysstuff_t *sysStuff);
void parse_array(cJSON *array);

#endif //CLION_FUNCTION_DECS_H
