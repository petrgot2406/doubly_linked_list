#ifndef CONSTANTS_LIST_H
#define CONSTANTS_LIST_H

const int POISON    = -1;
const int DATA_SIZE = 10;

enum Error_list_t
{
    LIST_OK        = 0,
    ERROR_ADDRESS  = 1,
    ERROR_SIZE     = 2,
    ERROR_CAPACITY = 3,
    ERROR_ANCHOR   = 4,
    ERROR_FREE     = 5,
};

#endif
