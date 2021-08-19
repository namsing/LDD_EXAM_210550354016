#include<stdio.h>
#include<sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>



int main()
{
	int fd,result;
	char ubuff[50]  = "This is 1st App writing data";
	fd = open("/dev/MyIOCTLstruct",O_RDWR,0777);
	if(fd<0)
	{
		printf("error opening driver\n");
		exit(1);
	}

	write(fd,Ubuff,sizeof(Ubuff));
	close(fd);
}
