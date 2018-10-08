#include <strings.h>
#include <string.h>
#include <curl/curl.h>
#include <syslog.h>
#include "enums_structs_etc.h"
#include "enums_structs_etc.h"
#include "function_decs.h"

void publish(tempstuff_t *tempstuff, curlstuff_t *curlstuff, sysstuff_t *sysStuff){
    CURL *curl;
    CURLcode res;
    long httpcode = 0;

    // build json data "the easy way"
    char data[100];
    sprintf(data, "{\"current_temp\" : %d, \"current_setpoint\" : %d,\"heater_status\" : %d}", tempstuff->temp, tempstuff->setpoint, tempstuff->heater);

    // start the curl
    curl = curl_easy_init();
    if(curl){
        // Set up URL
        curl_easy_setopt(curl, CURLOPT_URL, curlstuff->url);
        // load the json headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");

        // json-server accepts PATCH :)
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));

        // perform the request
        res = curl_easy_perform(curl);
        // *curl_code = res;
        if(res != CURLE_OK){
            curl_easy_cleanup(curl);
            syslog(LOG_INFO, "Curl error: bad request");
        }

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpcode);
        curlstuff->http_code = httpcode;

        curl_easy_cleanup(curl);

    } else{
        syslog(LOG_INFO, "Curl error: bad request");
    }

    sysStuff->next_state = LOOP;
}