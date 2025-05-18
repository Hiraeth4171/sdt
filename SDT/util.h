#ifndef _SDT_UTIL_LIB_H
#define _SDT_UTIL_LIB_H
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    size_t size;
    char* data;
} SDTString;

static inline char* sdt_read_file(const char* filepath, long* length) {
    // access first
    FILE* fd = fopen(filepath, "r");
    fseek(fd, 0L, SEEK_END);
    long pos = ftell(fd);
    *length = pos;
    rewind(fd);
    char* _buff = (char*)malloc(pos+1);
    fread(_buff, 1, pos, fd);
    _buff[pos] = '\0';
    fclose(fd);
    return _buff;
}

/*
 *  Function provided by Octe™
 */
static inline char* sdt_match_until_but_better(char* src, const char ch, size_t *len) {
    if (!*src) return NULL;
    char* ptr = src;
    for (;*ptr != ch && *ptr != '\0'; ++ptr);
    char* str = (char*)calloc((*len = ptr - src)+1, sizeof(char));
    if (!str) return NULL;
    for (size_t i = 0; i < *len; ++i) *str++ = *src++;
    return str;
}

static inline bool sdt_check_opts(char* ptr, char* chs) {
    for (char* ch = chs; *ch != '\0'; ++ch) if (*ptr == *ch) return false;
    return true;
}

/*
 *  Function inspired by Octe™
 */
static inline char* sdt_match_until_opts_but_better(char* src, const char* chs, size_t *len) {
    if (!*src) return NULL;
    char* ptr = src; bool flag = true;
    for (const char* ch = chs; *ch != '\0'; ++ch) if (*ptr == *ch) flag = false;
    for (;!flag && *ptr != '\0'; ++ptr);
    char* str = (char*)calloc((*len = ptr - src)+1, sizeof(char));
    if (!str) return NULL;
    for (size_t i = 0; i < *len; ++i) *str++ = *src++;
    return str;
}

static inline bool sdt_check_match(char* src, const char* match, size_t len) {
    size_t i = 0;
    char *ptr = src;
    for(;*ptr == match[i] && match[i] != '\0'; ++ptr, ++i);
    return (i-len);
}

static inline char* crt_str(char* str, size_t size) {
    char* _buff = (char*)malloc(size), *src = str, *dst = _buff;
    while (*src) *dst++ = *src++;
    *dst = '\0';
    return _buff;
}
#endif
