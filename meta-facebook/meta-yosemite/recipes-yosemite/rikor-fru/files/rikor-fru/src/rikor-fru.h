

#ifndef RIKOR_FRU_H
#define RIKOR_FRU_H

#include <linux/types.h>

#define eeprom_path "/sys/devices/platform/ast-i2c.3/i2c-3/3-0052"


typedef struct 
{
	unsigned int id;
	unsigned long long board_id;
	unsigned int ip1;
} rikor_fru_t;

int fru_buf_init(rikor_fru_t *const data);
int read_fru(rikor_fru_t *const data);
int write_fru(const rikor_fru_t *const data);

#endif // RIKOR_FRU_H