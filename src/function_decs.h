#include "cJSON.h"
#include "enums_structs_etc.h"
#ifndef CLION_FUNCTION_DECS_H
#define CLION_FUNCTION_DECS_H

#define DB_PATH "/home/jake/school/introIOT/project/res/json-server/json/jsondb.json"
#define MAX_SETPOINTS 20
#define ERROR_FORMAT "%s"


void initialize(tempstuff_t *tempStuff, sysstuff_t *sysStuff);
void getTemp(tempstuff_t *tempStuff, sysstuff_t *sysStuff);
void makeDecision(tempstuff_t *tempStuff, sysstuff_t *sysStuff, setpoints_t *setpointdb);
void readSetpoints(sysstuff_t *sysStuff, setpoints_t *setpointdb, tempstuff_t *tempStuff, curlstuff_t *curlStuff);
void publish(tempstuff_t *tempstuff, curlstuff_t *curlstuff, sysstuff_t *sysstuff);

#endif //CLION_FUNCTION_DECS_H
