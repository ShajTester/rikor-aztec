

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "crc16.h"
#include "rikor-fru.h"


int fru_buf_init(rikor_fru_t *const data)
{
	data->id = 0xaa5555aa;
	data->board_id = 0x1234567890abcdefL;
	data->ip1 = (192<<24) | (168<<16) | (0<<8) | (220);
	return EXIT_SUCCESS;
}



int read_fru(rikor_fru_t *const data)
{
	unsigned char buf[256];
	// if(fopen(eeprom_path, 'r') == )
	// {
	// }
	unsigned short crc = crc16(0, (const u8 *)data, sizeof(rikor_fru_t));
	printf("crc16(0) %d\n", crc);
	crc = crc16(~crc, (const u8 *)data, sizeof(rikor_fru_t));
	printf("crc16(crc16) %d\n", crc);
	printf("%d\n%d\n%d\n", (crc + (~crc)), crc * (~crc), crc - ~crc);
	return -1;
}



int write_fru(const rikor_fru_t *const data)
{
	return -1;
}

