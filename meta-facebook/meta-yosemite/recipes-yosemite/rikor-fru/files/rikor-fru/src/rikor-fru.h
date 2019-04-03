

#ifndef RIKOR_FRU_H
#define RIKOR_FRU_H

#include <linux/types.h>

#define DEFAT24ADDR 0x50
#define EEPROM_PATH "/sys/devices/platform/ast-i2c.3/i2c-3/3-00%02X/eeprom"

#define ERRCRC      -2




#pragma pack(push, 1)
typedef struct 
{
	unsigned int id;
	unsigned long long board_id;
	union
	{
		struct
		{
			unsigned int dhcp1: 1;
			unsigned int notused1: 15;
			unsigned int dhcp2: 1;
			unsigned int notused2: 15;
		};
		unsigned int netflags;
	};
	unsigned int ip1;
	unsigned int netmask1;
	unsigned int gate1;
	unsigned int ip2;
	unsigned int netmask2;
	unsigned int gate2;
	unsigned char hostname[18];
	unsigned char psw1[18];
	unsigned char psw2[18];
} rikor_fru_t;
#pragma pack(pop)

int get_fru_device(char *path);

int fru_buf_init(rikor_fru_t *const data);
int read_fru(const char *device, rikor_fru_t *const data);
int write_fru(const char *device, const rikor_fru_t *const data);

#endif // RIKOR_FRU_H
