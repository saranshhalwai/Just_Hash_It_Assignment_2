#include "rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define RPC_URL "http://127.0.0.1:18443"
#define RPC_USERPWD "saransh:12345678"   // Your bitcoin.conf credentials

struct Memory {
    char *data;
    size_t size;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    mem->data = realloc(mem->data, mem->size + total + 1);
    memcpy(&(mem->data[mem->size]), contents, total);

    mem->size += total;
    mem->data[mem->size] = 0;

    return total;
}

char* rpc_call(const char* method, const char* params) {
    CURL *curl;
    CURLcode res;

    struct Memory chunk;
    chunk.data = malloc(1);
    chunk.size = 0;

    // Fix: Dynamically allocate the JSON string to prevent buffer overflows
    size_t json_len = strlen(method) + (params ? strlen(params) : 0) + 128;
    char *json = malloc(json_len);
    if (!json) return NULL;

    snprintf(json, json_len,
        "{\"jsonrpc\":\"1.0\",\"id\":\"curl\",\"method\":\"%s\",\"params\":[%s]}",
        method, params ? params : "");

    curl = curl_easy_init();
    if (!curl) {
        free(json);
        return NULL;
    }

    // Fix: Explicitly declare the content type for Bitcoin Core
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "content-type: application/json;");

    curl_easy_setopt(curl, CURLOPT_URL, RPC_URL);
    curl_easy_setopt(curl, CURLOPT_USERPWD, RPC_USERPWD);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(json);

    if (res != CURLE_OK) {
        fprintf(stderr, "RPC request failed: %s\n", curl_easy_strerror(res));
        free(chunk.data);
        return NULL;
    }

    return chunk.data;
}

char* rpc_get_result(const char* json) {
    if (!json || strlen(json) == 0) return NULL; // Fix: Prevent NULL parse crashes

    cJSON *root = cJSON_Parse(json);
    if (!root) return NULL;

    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (!result) {
        cJSON_Delete(root);
        return NULL;
    }

    char *output = NULL;
    if (cJSON_IsString(result)) {
        output = strdup(result->valuestring);
    } else {
        output = cJSON_Print(result);
    }

    cJSON_Delete(root);
    return output;
}

char* rpc_get_hex(const char* json) {
    if (!json || strlen(json) == 0) return NULL; // Fix: Prevent NULL parse crashes

    cJSON *root = cJSON_Parse(json);
    if (!root) return NULL;

    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (!result) {
        cJSON_Delete(root);
        return NULL;
    }

    cJSON *hex = cJSON_GetObjectItem(result, "hex");
    if (!hex) {
        cJSON_Delete(root);
        return NULL;
    }

    char *output = strdup(hex->valuestring);

    cJSON_Delete(root);
    return output;
}