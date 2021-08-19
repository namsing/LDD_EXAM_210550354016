#include<stdio.h>
#include<sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/delay.h>

int main()
{
	int fd;
	char Ubuff[] = "This is user data from user 1.....";
	char Kbuff[100];
	fd = open("/dev/Mydevice1",O_RDWR,0777);
	if(fd<0)
	{
		printf("error opening driver\n");
		exit(1);
	}

	write(fd,Ubuff,80);
	msleep(3000);
	read(fd,Kbuff,50);
	printf("The data read from kernel is :  %s\n",Kbuff);
	close(fd);
}
