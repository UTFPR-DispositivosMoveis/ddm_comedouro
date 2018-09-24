#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
	int serial_port ;
	char dat;
	if ((serial_port = serialOpen ("/dev/ttyACM0", 115200)) < 0)	/* open serial port */
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}

	if (wiringPiSetup () == -1)					/* initializes wiringPi setup */
	{
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return 1 ;
	}
	dat = 'a';

	while(1)
	{
		serialPutchar(serial_port, dat);		/* transmit character serially on port */
	}

}
