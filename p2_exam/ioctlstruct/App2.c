#include<stdio.h>
#include<sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/ioctl.h>
#include <asm/ioctl.h>
#include <sys/ioctl.h>
#include "ioctl.h"



int main()
{
	int fd,result;
	char Kbuff[50];
	struct sendat myStruct = {1,2,'c'};
	fd = open("/dev/MyIOCTLstruct",O_RDWR,0777);
	if(fd<0)
	{
		printf("error opening driver\n");
		exit(1);
	}

	read(fd,Kbuff,sizeof(Kbuff));
	printf(" data received from App1 by App2 is : %s\n",Kbuff);
	close(fd);
}
