#pragma once

#include "pch.h"

#define log_info(str, ...) \
engine_log(stdout, str, ##__VA_ARGS__);

#define log_error(str, ...) \
engine_log(stderr, str, ##__VA_ARGS__);

#define engine_assert(x) \
if (!(x)) {\
    log_error("%s failed:%s:%d\n", #x, __FILE__, __LINE__)\
    exit(-1);\
}\

#define vec3_right (vec3) { -1, 0, 0 }
#define vec3_left (vec3) { 1, 0, 0 }
#define vec3_up (vec3) { 0, -1, 0}
#define vec3_down (vec3) { 0, 1, 0}

inline void engine_log(FILE* stream, const char* log, ...) {
    va_list list;
    va_start(list, log);
    vfprintf(stream, log, list);
    va_end(list);
}

char* read_file_contents(const char* path);