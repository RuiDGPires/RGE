#pragma once

#include <iostream>

#define ASSERT(expr, ...) \
    do{\
        if (!(expr)){\
            fflush(stdout);\
            fprintf(stderr, "Fatal Error: ");\
            fprintf(stderr, __VA_ARGS__);\
            exit(1);\
        }\
    }while(0)
