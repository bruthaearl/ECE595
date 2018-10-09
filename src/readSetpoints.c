//
// Created by jake on 10/6/18.
//
#define BUFFER_LENGTH 100

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "enums_structs_etc.h"
#include "function_decs.h"


struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        syslog(LOG_ERR, ERROR_FORMAT, "malloc fail. Exiting");
        exit(-1);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        syslog(LOG_ERR, ERROR_FORMAT, "realloc fail. Exiting");
        exit(-1);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size*nmemb;
}

void readSetpoints(sysstuff_t *sysStuff, setpoints_t *setpointdb, tempstuff_t *tempStuff, curlstuff_t *curlStuff){

//    char *db_buffer = 0;
//    long length;

    //open the file
 /*   FILE *temp_file = fopen(DB_PATH, "r");

    // load file into buffer, zero-terminate
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
    } else{
        syslog(LOG_ERR, ERROR_FORMAT, "Error opening database file. Exiting");
        exit(-1);
    }
*/
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    struct string s;
    init_string(&s);

    if(curl){

        // Set up URL
        curl_easy_setopt(curl, CURLOPT_URL, curlStuff->dburl);
        curl_easy_setopt(curl, CURLOPT_FRESH_CONNECT, 1);
        curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
        // load the json headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        headers = curl_slist_append(headers, "Cache-Control: no-cache");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        // perform the request
        res = curl_easy_perform(curl);
        // *curl_code = res;
        if(res != CURLE_OK){
            curl_easy_cleanup(curl);
            syslog(LOG_INFO, "Curl error: bad request");
        }


        curl_easy_cleanup(curl);

    } else{
        syslog(LOG_INFO, "Curl error: bad request");
    }


   // syslog(LOG_INFO, s.ptr);
    // parse the file with cJSON
    cJSON *root = cJSON_Parse(s.ptr);

    if(root== NULL){

        syslog(LOG_INFO, s.ptr);
        syslog(LOG_ERR, ERROR_FORMAT, "Error: database not in correct format");

        exit(-1);
    }
    cJSON *setpoints = cJSON_GetObjectItem(root, "setpoints");
    int setpoints_count = cJSON_GetArraySize(setpoints);


    int i;
    setpointdb->count = setpoints_count;
    //load setpoints into struct
    for(i=0; i<setpoints_count && i<MAX_SETPOINTS; i++){
        cJSON *setpoint = cJSON_GetArrayItem(setpoints, i);
        setpointdb->temp[i] = cJSON_GetObjectItem(setpoint, "temp")->valueint;
        setpointdb->time_h[i] = cJSON_GetObjectItem(setpoint, "time_h")->valueint;
        setpointdb->time_m[i] = cJSON_GetObjectItem(setpoint, "time_m")->valueint;
  //      printf("%d %d %d \n", setpointdb->temp[i], setpointdb->time_h[i], setpointdb->time_m[i]);
    }

    //fill the remaining with -1
    printf("setpoint_count: %d\n", setpoints_count);
    for(i = setpoints_count+1; i<MAX_SETPOINTS; i++){
        setpointdb->temp[i] = -1;
        setpointdb->time_h[i] = -1;
        setpointdb->time_m[i] = -1;
    }

    //read manual setpoint
    cJSON *stats = cJSON_GetObjectItem(root, "stats");
    cJSON *stat = cJSON_GetArrayItem(stats, 0);
    tempStuff->read_setpoint = cJSON_GetObjectItem(stat, "current_setpoint")->valueint;
    printf("read setpoint: %d\n", tempStuff->read_setpoint);

    cJSON_Delete(setpoints);

    free(s.ptr);

    sysStuff->next_state = MAKE_DECISION;
}