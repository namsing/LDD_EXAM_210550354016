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


struct stats
{
	int size;
	int r_w;
	char buff[];
};
int main()
{
	int fd,result;
	struct stats *myStruct;
	fd = open("/dev/MyIOCTLstruct",O_RDWR,0777);
	if(fd<0)
	{
		printf("error opening driver\n");
		exit(1);
	}

	
	result = ioctl(fd,GETSTATS,myStruct);
		if(result<0)
	{
		printf("IOCTL Error\n");
		return(-1);
	}
		if(myStruct->r_w == 0)
		{
		printf("recent actvity was Read\n");
		printf("the string sent by Application 1 is : %s\n",myStruct->buff);
		}
		else if(myStruct->r_w ==1)
		{
			printf("recent activity was write\n");
		}

	
	close(fd);
}
