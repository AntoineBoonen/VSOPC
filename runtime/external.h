/*
 * external.h
 *
 * by Antoine Boonen
 *
 * This file contains the declaration for the external code used by the compiler.
 *
 * Created   18/05/21
 * Modified  18/05/21
 */
#ifndef POWER_H_
#define POWER_H_

#include <stdint.h>

/*
 * power
 *
 * returns (x^y)
 */
int64_t power(int32_t x, int32_t y);

#endif//POWER_H_
