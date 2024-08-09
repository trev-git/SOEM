#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "ethercat.h"
#include "sdo.h"

uint8_t sdo_read8(int slave, uint16_t index, uint8_t subindex)
{
    uint8_t val = 0;
    int size = sizeof(val);
    if (ec_SDOread(slave, index, subindex, FALSE, &size, &val, EC_TIMEOUTSAFE) == 0)
    {
        fprintf(stderr, "Failed to read at %04x:%02x!\n", index, subindex);
    }
    return val;
}

uint16_t sdo_read16(int slave, uint16_t index, uint8_t subindex)
{
    uint16_t val = 0;
    int size = sizeof(val);
    if (ec_SDOread(slave, index, subindex, FALSE, &size, &val, EC_TIMEOUTSAFE) == 0)
    {
        fprintf(stderr, "Failed to read at %04x:%02x!\n", index, subindex);
    }
    return val;
}

uint32_t sdo_read32(int slave, uint16_t index, uint8_t subindex)
{
    uint32_t val = 0;
    int size = sizeof(val);
    if (ec_SDOread(slave, index, subindex, FALSE, &size, &val, EC_TIMEOUTSAFE) == 0)
    {
        fprintf(stderr, "Failed to read at %04x:%02x!\n", index, subindex);
    }
    return val;
}

bool sdo_write8(int slave, uint16_t index, uint8_t subindex, uint8_t val)
{
    if (ec_SDOwrite(slave, index, subindex, FALSE, sizeof(val), &val, EC_TIMEOUTSAFE) == 0)
    {
        fprintf(stderr, "Failed to write %02x to %04x:%02x!\n", val, index, subindex);
        return false;
    }
    return true;
}

bool sdo_write16(int slave, uint16_t index, uint8_t subindex, uint16_t val)
{
    if (ec_SDOwrite(slave, index, subindex, FALSE, sizeof(val), &val, EC_TIMEOUTSAFE) == 0)
    {
        fprintf(stderr, "Failed to write %04x to %04x:%02x!\n", val, index, subindex);
        return false;
    }
    return true;
}
bool sdo_write32(int slave, uint16_t index, uint8_t subindex, uint32_t val)
{
    if (ec_SDOwrite(slave, index, subindex, FALSE, sizeof(val), &val, EC_TIMEOUTSAFE) == 0)
    {
        fprintf(stderr, "Failed to write %08x to %04x:%02x!\n", val, index, subindex);
        return false;
    }
    return true;
}
