#ifndef TRAPEZOIDAL_H
#define TRAPEZOIDAL_H

#include "phase.h"

struct ComStep{
    struct Phase*       high;
    struct Phase*        low;
    struct Phase*        off;
    uint8_t             hall;
    struct ComStep*     prev;
    struct ComStep*     next;

} _com_step[6];

struct ComStep* com_step[6];
struct ComStep* current_step;

void com_init();
void com_hall_step(uint8_t hall);
void com_activate_step(struct ComStep *step);
#endif
