//
// Created by jake on 9/27/18.
//

#ifndef CLION_ENUMS_STRUCTS_ETC_H
#define CLION_ENUMS_STRUCTS_ETC_H

enum returns{ OK = 0, ERR_SETSID = -1, ERR_CHDIR = -2, UHH_OH=-3, ERR_FORK=-4, ERR_LOCK=-5};



#include <form.h>

// state machine states
typedef enum{
    INITIALIZE = 0,
    READ_THERMOCOUPLE,
    READ_SETPOINTS,
    MAKE_DECISION,
    PUBLISH_TO_SERVER,
    LOOP
} sm_state;

// heater states
typedef enum {
    OFF = 0,
    ON
} heater_state;

// current/next state
typedef struct {
    sm_state state;
    sm_state next_state;
    int error;
} sysstuff_t;

// thermostat information
typedef struct{
    int temp;
    heater_state heater;
    int setpoint;
    int read_setpoint;
} tempstuff_t;

// setpoint database data
typedef struct{
    int temp[20];
    int time_h[20];
    int time_m[20];
    int count;
} setpoints_t;

// curl stuff
typedef struct{
    char staturl[100];
    long http_code;
    char dburl[100];
} curlstuff_t;

#endif //CLION_ENUMS_STRUCTS_ETC_H
