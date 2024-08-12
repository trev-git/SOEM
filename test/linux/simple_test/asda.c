#include <stdint.h>
#include <stdio.h>

#include "asda.h"
#include "sdo.h"
#include "ethercat.h"

#define MODE_PROFILE_POSITION 0x01
#define MODE_CYCLIC_SYNCHRONOUS_POSITION 0x08

/* PDO addresses */
#define RXPDO 0x1601
#define TXPDO 0x1a01
#define SM2   0x1c12
#define SM3   0x1c13

/* RxPDO addresses */
#define PROFILE_MODE     0x6060
#define CONTROL_WORD     0x6040
#define TARGET_POSITION  0x607a
#define TARGET_VELOCITY  0x60ff
#define PROFILE_VELOCITY 0x6081
#define PROFILE_ACCEL    0x6083
#define PROFILE_DECEL    0x6084

/* TxPDO addresses*/
#define STATUS_WORD      0x6041
#define POSITION_ACTUAL  0x6064

int pdo_remap_pp(uint16_t slave)
{
    /* disable PDO configuration */
    sdo_write8(slave, SM2, 0, 0);
    sdo_write8(slave, SM3, 0, 0);

    /* disable PDO mapping */
    sdo_write8(slave, RXPDO, 0, 0);
    sdo_write8(slave, TXPDO, 0, 0);

    /* map RxPDO for Profile Position mode */
    sdo_write32(slave, RXPDO, 1, ((PROFILE_MODE << 16)+0x8));
    sdo_write32(slave, RXPDO, 2, ((CONTROL_WORD << 16) + 0x10));
    sdo_write32(slave, RXPDO, 3, ((TARGET_POSITION << 16) + 0x20));
    sdo_write32(slave, RXPDO, 4, ((PROFILE_VELOCITY << 16) + 0x20));
    sdo_write32(slave, RXPDO, 5, ((PROFILE_ACCEL << 16) + 0x20));
    sdo_write32(slave, RXPDO, 6, ((PROFILE_DECEL << 16) + 0x20));
    sdo_write8(slave, RXPDO, 0, 6);


    /* map TxPDO for Profile Position mode */
    sdo_write32(slave, TXPDO, 1, ((STATUS_WORD << 16) + 0x10));
    sdo_write32(slave, TXPDO, 2, ((POSITION_ACTUAL << 16) + 0x20));
    sdo_write8(slave, TXPDO, 0, 2);

    /* set PDO mapping */
    sdo_write16(slave, SM2, 1, RXPDO);
    sdo_write16(slave, SM3, 1, TXPDO);

    /* enable PDO configuration */
    sdo_write8(slave, SM2, 0, 1);
    sdo_write8(slave, SM3, 0, 1);

    return 1;
}

int pdo_remap_csp(uint16_t slave)
{
    /* disable PDO configuration */
    sdo_write8(slave, SM2, 0, 0);
    sdo_write8(slave, SM3, 0, 0);

    /* disable PDO mapping */
    sdo_write8(slave, RXPDO, 0, 0);
    sdo_write8(slave, TXPDO, 0, 0);

    /* map RxPDO for Cyclic Synchronous Position mode */
    sdo_write32(slave, RXPDO, 1, ((PROFILE_MODE << 16)+0x8));
    sdo_write32(slave, RXPDO, 2, ((CONTROL_WORD << 16) + 0x10));
    sdo_write32(slave, RXPDO, 3, ((TARGET_POSITION << 16) + 0x20));
    sdo_write8(slave, RXPDO, 0, 3);

    /* map TxPDO for Cyclic Synchronous Position mode mode */
    sdo_write32(slave, TXPDO, 1, ((STATUS_WORD << 16) + 0x10));
    sdo_write32(slave, TXPDO, 2, ((POSITION_ACTUAL << 16) + 0x20));
    sdo_write8(slave, TXPDO, 0, 2);

    /* set PDO mapping */
    sdo_write16(slave, SM2, 1, RXPDO);
    sdo_write16(slave, SM3, 1, TXPDO);

    /* enable PDO configuration */
    sdo_write8(slave, SM2, 0, 1);
    sdo_write8(slave, SM3, 0, 1);

    return 1;
}

int pdo_remap_csv(uint16_t slave)
{
    /* disable PDO configuration */
    sdo_write8(slave, SM2, 0, 0);
    sdo_write8(slave, SM3, 0, 0);

    /* disable PDO mapping */
    sdo_write8(slave, RXPDO, 0, 0);
    sdo_write8(slave, TXPDO, 0, 0);

    /* map RxPDO for Cyclic Synchronous Velocity mode */
    sdo_write32(slave, RXPDO, 1, ((CONTROL_WORD << 16) + 0x10));
    sdo_write32(slave, RXPDO, 2, ((TARGET_VELOCITY << 16) + 0x20));
    sdo_write8(slave, RXPDO, 0, 2);

    /* map TxPDO for Cyclic Synchronous Velocity mode */
    sdo_write32(slave, TXPDO, 1, ((STATUS_WORD << 16) + 0x10));
    sdo_write32(slave, TXPDO, 2, ((POSITION_ACTUAL << 16) + 0x20));
    sdo_write8(slave, TXPDO, 0, 2);

    /* set PDO mapping */
    sdo_write16(slave, SM2, 1, RXPDO);
    sdo_write16(slave, SM3, 1, TXPDO);

    /* enable PDO configuration */
    sdo_write8(slave, SM2, 0, 1);
    sdo_write8(slave, SM3, 0, 1);

    return 1;
}

// Эти две функции относятся к машине состояний двигателя,
// но к сожалению они работают странно и я их не использую.

asda_state_t get_current_state(uint16_t status_word)
{
    if (status_word & (1 << 6))
    {
        return SWITCH_ON_DISABLED;
    }
    else if ((status_word & 0b1111) == 1)
    {
        return READY_TO_SWITCH_ON;
    }
    else if ((status_word & 0b1111) == 0b11)
    {
        return SWITCH_ON;
    }
    else if ((status_word & 0b1111) == 0b111 &&
        (status_word & (1 << 5)))
    {
        return OPERATION_ENABLED;
    }
    else if ((status_word & 0b1111) == 0b111 &&
        !(status_word & (1 << 5)))
    {
        return QUICK_STOP;
    }
    else if ((status_word & 0b1111) == 0b1111)
    {
        return FAULT;
    }
    else
    {
        return INVALID;
    }
}

uint16_t to_servo_on(asda_state_t state)
{
    switch (state)
    {
    case SWITCH_ON_DISABLED: // take transition #2
        printf("Switch on disabled\n");
        return 0b110;
    case READY_TO_SWITCH_ON: // take transition #3
        printf("Ready to switch on\n");
        return 0b111;
    case SWITCH_ON: // take transition #4
        printf("Switch on\n");
        return 0b1111;
    case OPERATION_ENABLED: // that's it
        printf("Operation enabled\n");
        return 0;
    case QUICK_STOP: // take transition #12
        printf("Quick stop\n");
        return 0b01101;
    case FAULT: // take transition #15
        printf("Fault\n");
        return 0b10000000;
    case INVALID:
        printf("Invalid state\n");
        return 0;
    }
}
