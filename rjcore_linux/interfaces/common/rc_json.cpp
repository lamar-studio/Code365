#include "rc_json.h"
#include "json.h"
#include "rj_commom.h"

using std::string;


namespace RcJson
{
static bool __init_key_node(cJSON* & json, cJSON* & pMsg, const string& input, const string& key)
{
    json = cJSON_Parse(input.c_str());
    pMsg = cJSON_GetObjectItem (json, key.c_str());
    if(pMsg == NULL) {
        rjlog_error("can not find json key %s", key.c_str());
        return false;
    }
    return true;
}
static void __destroy_key_node(cJSON* json)
{
    cJSON_Delete(json);
}

const string rc_json_get_string(const string& input, const string& key)
{
    string value;
    cJSON* json = NULL;
    cJSON* pMsg = NULL;
    value.clear();
    if(__init_key_node(json, pMsg, input, key)) {
        if(pMsg->valuestring != NULL) {
            value = pMsg->valuestring;
        } else {
            rjlog_error("json key %s is not a string", key.c_str());
            value.clear();
        }
    } else {
        rjlog_error("can not find json key %s", key.c_str());
        value.clear();
    }
    __destroy_key_node(json);
    return value;
}

int rc_json_get_int(const string& input, const string& key)
{
    int value = 0;
    cJSON* json = NULL;
    cJSON* pMsg = NULL;
    if(__init_key_node(json, pMsg, input, key)) {
        value = pMsg->valueint;
    } else {
        rjlog_error("can not find json key %s", key.c_str());
        value = ERROR_INT;
    }
    __destroy_key_node(json);
    return value;
}

const string rc_json_get_child(const string& input, const string& key)
{
    string value;
    char* print_ret = NULL;
    cJSON* json = NULL;
    cJSON* pMsg = NULL;
    value.clear();
    if(__init_key_node(json, pMsg, input, key)) {
        print_ret = cJSON_PrintUnformatted(pMsg);
        if(print_ret != NULL) {
            value = print_ret;
        } else {
            rjlog_error("json key %s is not a child json", key.c_str());
            value.clear();
        }
    } else {
        rjlog_error("can not find json key %s", key.c_str());
        value.clear();
    }
    if(print_ret) {
        free(print_ret);
    }
    __destroy_key_node(json);
    return value;
}

static void __init_cjson(const std::string& input, cJSON* & json, char* & print_ret)
{
    json = NULL;
    if(input.empty()) {
        json = cJSON_CreateObject();
    } else {
        json = cJSON_Parse(input.c_str());
    }
    if(!json) {
        rjlog_error("rc_json_add input is error");
        rjlog_error("input is %s",input.c_str());
    }
    print_ret = NULL;
}
static void __destroy_cjson(std::string& input, cJSON* json, char* print_ret)
{
    print_ret = cJSON_PrintUnformatted(json);
    if(print_ret) {
        input = print_ret;
        free(print_ret);
    }
    cJSON_Delete(json);
}

void rc_json_add_string(std::string& input, const std::string& key, const std::string& value)
{
    cJSON* json = NULL;
    char* print_ret = NULL;
    __init_cjson(input, json, print_ret);
    cJSON_AddStringToObject(json, key.c_str(), value.c_str());
    __destroy_cjson(input, json, print_ret);
}

void rc_json_add_int(std::string& input, const std::string& key, int value)
{
    cJSON* json = NULL;
    char* print_ret = NULL;
    __init_cjson(input, json, print_ret);
    cJSON_AddNumberToObject(json, key.c_str(), value);
    __destroy_cjson(input, json, print_ret);
}

void rc_json_add_bool(std::string& input, const std::string& key, bool value)
{
    cJSON* json = NULL;
    char* print_ret = NULL;
    __init_cjson(input, json, print_ret);
    cJSON_AddBoolToObject(json, key.c_str(), value);
    __destroy_cjson(input, json, print_ret);
}

void rc_json_add_child(std::string& input, const std::string& key, const std::string& value)
{
    cJSON* json = NULL;
    char* print_ret = NULL;
    __init_cjson(input, json, print_ret);
    cJSON_AddStringToObject(json, key.c_str(), value.c_str());
    __destroy_cjson(input, json, print_ret);
}
}
