#ifndef CONSTANTS_H
#define CONSTANTS_H

#define POISON  -1
#define DATA_SIZE  10

enum Error_list_t
{
    LIST_OK        = 0,
    ERROR_ADDRESS  = 1,
    ERROR_SIZE     = 2,
    ERROR_CAPACITY = 3,
    ERROR_PUSH     = 4,
    ERROR_POP      = 5,
    ERROR_ANCHOR   = 6,
    ERROR_FREE     = 7,
};

#endif
