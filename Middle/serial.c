#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>

int main(void) {
	int fd;
	
	if((fd = serialOpen("/dev/ttyACM0", 115200)) < 0) {
		perror("Erro ao abrir serial");
	}
	
	serialPuts(fd, "TESTE\n");
	
	printf("Quantia de dados para receber: %d\n",  serialDataAvail(fd));
	
	while(1) {
		putchar(serialGetchar(fd));
		fflush(stdout);
	}
	return 0;
}
