#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include "ioctl.h"
#define NAME MyIOCTLstruct
#define SUCCESS 1
struct stats
{
	int size;
	int r_w;
	char buff[];
};
int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
long NAME_ioctl(struct file *filp,unsigned int cmd, unsigned long arg);


struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = NAME_open,
	.read = NAME_read,
	.wirte = NAME_write,
	.release = NAME_release,
	.unlocked_ioctl = NAME_ioctl,
};
struct cdev *my_cdev;
struct stats *myStruct;

static int __init chardev_init(void)
{
	int result;
	int MAJOR,MINOR;
	dev_t Mydev;
	Mydev = MKDEV(257,0);
	MAJOR = MAJOR(Mydev);
	MINOR = MINOR(Mydev);
	printk("\nThe major number is %d, and minor number  is %d",MAJOR,MINOR);
	result = register_chrdev_region(Mydev,1,"MyIOCTLstruct");
	if(result<0)
	{
		printk("\nError");
		return(-1);
	}
	my_cdev = cdev_alloc();
	my_cdev->ops = &fops;

	result = cdev_add(my_cdev,Mydev,1);
	if(result<0)
	{
		printk("\nerror obtaining region");
		unregister_chrdev_region(Mydev,1);
		return(-1);
	}
	return 0;
}


void __exit chardev_exit(void)
{
	dev_t Mydev;
	int MAJOR, MINOR;
	Mydev = MKDEV(257,0);
	MAJOR = MAJOR(Mydev);
	MINOR = MINOR(Mydev);
	printk("\nThe major number is %d, and minor number is %d",MAJOR,MINOR);
	unregister_chrdev_region(Mydev,1);
	cdev_del(my_cdev);
	printk("\nUnregistered");
	return;
	
}

int NAME_open(struct inode *inode, struct file *filp)
{
	printk("\nThis is an open call");
	return 0;
}


int NAME_release(struct inode *inode, struct file *filp)
{
	printk("\nThis is a close call");
	return 0;
}
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
	ssize_t ret;
	unsigned long remaining;
	printk("\nread command called :");
	myStruct->r_w = 0;
	remaining = copy_to_user((char *)Ubuff,(char *)myStruct->buff,count);

	if(remaining == 0)
	{
		printk("\ndata succesfully copied");
		myStruct->size = count;
		return count;
	}
	else if(remaining>0)
	{
		printk("data still remaining to be read");
		return (count - remaining);
	}
	else
	{
		printk("\n error reading data\n");
		ret = -EFAULT;
		return ret;
	}
	return 0;
}

ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp)
{
	unsigned long remaining;
	ssize_t ret;
	printk("\nwriting data: \n");
	myStruct->r_w = 1;
	remaining = copy_from_user(((char *)myStruct->buff,(char *)Ubuff,count);
	if(remaining == 0)
	{
		printk("data successfully read from user : %s",Ubuff);
		myStruct->size = count;
		return count;
	}
	else if(remaining>0)
	{
		printk("data partially read from user : %s",Kbuff);
		return (count-remaining);
	}
	else
	{
		printk("error writing data\n");
		ret = -EFAULT;
		return ret;
	}
	
	return 0;
}
long NAME_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{

	char Ubuff[100];
	struct sendat *myStruct;
	ssize_t retval = SUCCESS;
	int res;
	printk("IOCTL function : \n");
	if(cmd == GETSTATS)
	{
		res = copy_to_user((struct stats *)arg,myStruct,sizeof(struct stats));
		if(res==0)
		{
		printk("status sent succesfully");
		}
		else{
		printk("status not completely sent\n");
		}

	}

			
	return retval;
}


module_init(chardev_init);
module_exit(chardev_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("NAMRATHA");
MODULE_DESCRIPTION("A Complex IOCTL program");

