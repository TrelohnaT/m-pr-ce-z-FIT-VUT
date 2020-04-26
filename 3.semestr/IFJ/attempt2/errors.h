#ifndef ERRORS_H_INDLUDED
#define ERRORS_H_INCLUDED

#include <stdlib.h>

typedef enum error_code{
    NOERROR_OK = 0,
    ERROR_LEX = 1,
    ERROR_SYN = 2,
    ERROR_SEM_VAR = 3,
    ERROR_SEM_FUN = 4,
    ERROR_SEM_ARG = 5,
    ERROR_SEM = 6,
    ERROR_ZERO_DIV = 9,
    ERROR_COMPILATOR = 99,
} ERROR_CODE;

#endif