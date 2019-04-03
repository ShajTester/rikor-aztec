

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>

#include "crc16.h"
#include "rikor-fru.h"


#ifndef DEBUG
#define DEBUG
#endif


// Отсюда
// https://github.com/KyleBanks/XOREncryption/blob/master/C%2B%2B/main.cpp
// https://toster.ru/q/489264
static char key[] = {'K', 'C', 'Q'}; //Any chars will work, in an array of any size

void encryptDecrypt(char *toEncrypt, int len)
{
    for (int i = 0; i < len; i++)
        toEncrypt[i] = toEncrypt[i] ^ key[i % (sizeof(key) / sizeof(char))];
}



int fru_buf_init(rikor_fru_t *const data)
{
	data->id = 0xaa5555aa;
	data->board_id = 0x1234567890abcdefL;
	data->ip1 = (192<<24) | (168<<16) | (0<<8) | (220);
	return EXIT_SUCCESS;
}


int get_fru_device(char *path)
{
	int retval = 0;
	int at24addr = DEFAT24ADDR;

	FILE *ffaddr = fopen("/tmp/rikor-fru-address", "rb");
	if(ffaddr == NULL)
	{
		syslog(LOG_ERR, "Can not open file: %s, error: %s\n", "/tmp/rikor-fru-address", strerror(errno));
		retval = 1;
	}
	else
	{
		fscanf(ffaddr, "%x", &at24addr);
		if((at24addr < 0x50) || (at24addr > 0x57))
		{
			syslog(LOG_ERR, "Wrong AT24C02 address %d\n", at24addr);
			at24addr = 0x50;
			retval = 2;
		}
	}

	sprintf(path, EEPROM_PATH, at24addr);
	return retval;
}


int read_fru(const char *device, rikor_fru_t *const data)
{
	// unsigned char buf[256];
	FILE *fp;
	unsigned short crc;
	unsigned short fcrc;
	int rc;

	fp = fopen(device, "rb");
	if (!fp)
	{
		int err = errno;
		syslog(LOG_ERR, "failed to open device %s", device);
		return err;
	}

	rc = fread(data, sizeof(rikor_fru_t), 1, fp);
	rc += fread(&fcrc, 2, 1, fp);
	fclose(fp);

	if (rc != 2)
	{
		syslog(LOG_ERR, "failed to read device %s", device);
		return ENOENT;
	} 
	else
	{
		crc = crc16(0, (const u8 *)data, sizeof(rikor_fru_t));
		if(fcrc != crc)
		{ // Ошибка CRC
			syslog(LOG_ERR, "CRC error in device %s", device);
			return ERRCRC;
		}
		return 0;
	}

	// crc = crc16(0, (const u8 *)data, sizeof(rikor_fru_t));
	// fprintf(stderr, "crc16(0) %d\n", crc);
	// fprintf(stderr, "%d\n%d\n%d\n", (crc + (~crc) + 1), crc * (~crc), crc - ~crc);
	// return -1;
}



int write_fru(const char *device, const rikor_fru_t *const data)
{
	FILE *fp;
	unsigned short crc;
	int rc;

	fp = fopen(device, "wb");
	if (!fp)
	{
		int err = errno;

		syslog(LOG_ERR, "failed to open device %s", device);
		return err;
	}

	rc = fwrite(data, sizeof(rikor_fru_t), 1, fp);
	crc = crc16(0, (const u8 *)data, sizeof(rikor_fru_t));
	rc += fwrite(&crc, 2, 1, fp);
	fclose(fp);

	return 0;
}

