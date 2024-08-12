#ifndef ASDA_H
#define ASDA_H

#include <stdint.h>

typedef enum
{
    SWITCH_ON_DISABLED,
    READY_TO_SWITCH_ON,
    SWITCH_ON,
    OPERATION_ENABLED,
    QUICK_STOP,
    FAULT,
    INVALID
} asda_state_t;

int pdo_remap_pp(uint16_t slave);
int pdo_remap_csp(uint16_t slave);
int pdo_remap_csv(uint16_t slave);
asda_state_t get_current_state(uint16_t status_word);
uint16_t to_servo_on(asda_state_t state);

#endif // ASDA_H
