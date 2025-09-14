#include "support_functions.h"

int len_char(const char* q) {
    int len = 0;
    while(*q != '\0'){
        q++;
        len++;
    }
    return len;
}

int char_to_int(char* q, int start, int end) {
    if(end == -1) end = len_char(q);
    bool is_minus = false;

    if(q[start] == '-'){
        is_minus = true;
        ++start;
    }

    int u = 0, n = 1;
    for(int i = end - 1; i > start - 1; --i){
        
        u += (q[i] - '0') * n;
        n *= 10;
    }

    if(is_minus) u*=-1;

    return u;
}

bool found_int(char* q, int start, int end) {
    if(end == -1) end = len_char(q);
    for(int i = start; i < end; ++i){
        if(!(q[i] >= '0' && q[i] <= '9')){
            return false;
        }
    }
    return true;
}

bool string_identity(const char* a, const char* b) {
    int len_a = len_char(a);
    int len_b = len_char(b);

    if(len_a != len_b){
        return false;
    }

    while(*a != '\0'){
        if(*a != *b){
            return false;
        }

        a++;
        b++;
    }

    return true;
}