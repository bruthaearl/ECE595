#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include "enums_structs_etc.h"
#include "function_decs.h"
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>


#define DAEMON_NAME "HOTBOX"


tempstuff_t tempStuff;
sysstuff_t sysStuff;
setpoints_t setpointdb;
curlstuff_t curlStuff;

char staturl[] = "http://18.224.222.33:3000/stats/1";
char dburl[] ="http://18.224.222.33/db/jsondb.json";


static void _signal_handler(const int signal){
    switch(signal){
        case SIGHUP:
            break;
        case SIGTERM:
            syslog(LOG_INFO, "received SIGTERM, exiting.");
            closelog();
            exit(OK);
            break;
        default:
            syslog(LOG_INFO, "received unhandled signal");
            break;
    }
}


int main() {

    openlog(DAEMON_NAME, LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_DAEMON);
    syslog(LOG_INFO, "starting the HOTBOX daemon");

    // fork
    pid_t pid = fork();

    if(pid < 0){
        syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
        closelog();
        return ERR_FORK;
    }

    if(pid > 0) {
        closelog();
        return OK;
    }

    if(setsid() < -1) {
        syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
        closelog();
        return ERR_SETSID;
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    umask(S_IRUSR | S_IWUSR);

    if(chdir("/") < 0) {
        syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
        return ERR_CHDIR;
    }

    signal(SIGTERM, _signal_handler);
    signal(SIGHUP, _signal_handler);

    // quick inits
    strcpy(curlStuff.staturl, staturl);
    strcpy(curlStuff.dburl, dburl);
    tempStuff.setpoint = 47;

    sysStuff.state = INITIALIZE;
    while (1) {

        switch (sysStuff.state) {

            case INITIALIZE :
                initialize(&tempStuff, &sysStuff);
                break;
            case READ_THERMOCOUPLE :
                getTemp(&tempStuff, &sysStuff);
                break;
            case MAKE_DECISION :
                makeDecision(&tempStuff, &sysStuff, &setpointdb);
                break;
            case READ_SETPOINTS :
                readSetpoints(&sysStuff, &setpointdb, &tempStuff, &curlStuff);
                break;
            case PUBLISH_TO_SERVER :
                publish(&tempStuff, &curlStuff, &sysStuff);
                break;
            case LOOP:
                syslog(LOG_INFO, "TEMP: %d", tempStuff.temp);
                syslog(LOG_INFO, "SETPOINT: %d", tempStuff.setpoint);
                sleep(6);
                sysStuff.next_state = READ_THERMOCOUPLE;
                break;
            default :
                exit(20);
        }
        sysStuff.state = sysStuff.next_state;
    }

    closelog();
    return UHH_OH; // how did we get here?
}

