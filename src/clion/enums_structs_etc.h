//
// Created by jake on 9/27/18.
//

#ifndef CLION_ENUMS_STRUCTS_ETC_H
#define CLION_ENUMS_STRUCTS_ETC_H



typedef enum{
    INITIALIZE = 0,
    READ_THERMOCOUPLE,
    READ_SETPOINTS,
    MAKE_DECISION,
    WRITE_TO_FILE,
    PUBLISH_TO_SERVER,
    DEBUG
} sm_state;

typedef enum {
    OFF = 0,
    ON
} heater_state;

typedef struct {
    sm_state state;
    sm_state next_state;
    int error;
} sysstuff_t;

typedef struct{
    int temp;
    heater_state heater;
} tempstuff_t;

#endif //CLION_ENUMS_STRUCTS_ETC_H
