#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <linux/videodev2.h>


///////////////////////////////////
#define  IOCTL_IO_READ    0x1103
#define  IOCTL_IO_WRITE   0x1104
#define  IOCTL_AUTOMODE_TRIGGER         0x1111
#define  IOCTL_PASS3_TRIGGER   0x1112
#define  IOCTL_I2C_READ        0x1113
#define  IOCTL_I2C_WRITE       0x1114



int main (int argc, char* argv[])
{

	/*Read Params*/
	char *device_name;
	device_name = "/dev/ast-video";

	/*Open Device*/
	int  file_device = open(device_name, O_RDWR);

	if (file_device == -1)
	{
		printf ("%s error %d, %s\n", device_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	/*Read Params From Device*/
	struct v4l2_capability device_params;

//memset(&device_params, 0, sizeof(struct v4l2_capability));

	int retval = ioctl(file_device, VIDIOC_QUERYCAP, &device_params);

	printf("retval ioctl = %d\n", retval);

	if (retval == -1)
	{
		printf ("\"VIDIOC_QUERYCAP\" error %d, %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("driver : %s\n", device_params.driver);
	printf("card : %s\n", device_params.card);
	printf("bus_info : %s\n", device_params.bus_info);
	printf("version : %d.%d.%d\n", ((device_params.version >> 16) & 0xFF), ((device_params.version >> 8) & 0xFF), (device_params.version & 0xFF));
	printf("capabilities: 0x%08x\n", device_params.capabilities);
	printf("device capabilities: 0x%08x\n", device_params.device_caps);

	ioctl(file_device, IOCTL_I2C_READ, &device_params);

	/* Close Device */
	if (-1 == close (file_device))
	{
		printf ("\"close\" error %d, %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	file_device = -1;

	return 0;
}
