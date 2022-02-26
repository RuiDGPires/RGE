#pragma once
#include <stdint.h>

#define BLUE_STR "\e[1;36m"
#define YELLOW_STR "\e[1;33m"
#define BOLD_STR "\e[1;37m"
#define GREEN_STR "\e[1;32m"
#define RESET_STR "\e[0m"
#define GRAY_STR "\e[1;90m"

enum color_c {
    INIT_c = 1,
    BLUE_c,
    YELLOW_c,
    BOLD_c,
    GREEN_c,
    GRAY_c,
    RESET_c,

    COUNT_c,
};


typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;

