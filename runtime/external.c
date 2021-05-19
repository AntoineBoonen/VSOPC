/*
 * external.c
 *
 * by Antoine Boonen
 *
 * This file contains the implementation of the external code as defined in 'external.h'.
 *
 * Created   18/05/21
 * Modified  18/05/21
 */
#include "external.h"

int64_t power(int32_t x, int32_t y) {
    int64_t pow = x;
    for(int i = 1; i < y; ++i) {
        pow *= x;
    }
    return pow;
}
