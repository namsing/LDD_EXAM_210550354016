#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/wait.h>
#include <linux/sched.h>

#define NAME MyDevice

int NAME_open(struct inode *inode, struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);
int NAME_open2(struct inode *inode, struct file *filp);
int NAME_release2(struct inode *inode, struct file *filp);
ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);
ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);
ssize_t NAME_read2(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp);
ssize_t NAME_write2(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp);


struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = NAME_open,
	.read = NAME_read,
	.write = NAME_write,
	.release = NAME_release,
};

struct file_operations fops2 = 
{
	.owner = THIS_MODULE,
	.open = NAME_open,
	.read = NAME_read2,
	.write = NAME_write2,
	.release = NAME_release,
};

struct cdev *my_cdev;
struct cdev *my_cdev2;
char Kbuff[50];
struct semaphore sem_dev;
int busWr = 0;
int busRd1 = 0;
int busRd2 = 0;
dev_t Mydev1,Mydev2;




DECLARE_WAIT_QUEUE_HEAD(mywait);
DECLARE_WAIT_QUEUE_HEAD(mywait2);

static int __init chardev_init(void)
{
	int result1,result2;
	int MAJOR1,MINOR1;
	Mydev1 = MKDEV(256,0);
	Mydev2 = MKDEV(256,1);
	MAJOR1 = MAJOR(Mydev1);
	MINOR1 = MAJOR(Mydev1);
	printk("\nThe major is %d, and minor is %d",MAJOR1,MINOR1);
	result1 = register_chrdev_region(Mydev1,1,"MyDevice1");
	result2 = register_chrdev_region(Mydev2,1,"MyDevice2");
		
	if(result1<0||result2<0)
	{
		printk("\nError allocation region");
		return(-1);
	}
	my_cdev = cdev_alloc();
	my_cdev->ops = &fops;
	my_cdev2 = cdev_alloc();
	my_cdev2->ops = &fops2;

	result1 = cdev_add(my_cdev,Mydev1,1);
	result2 = cdev_add(my_cdev2,Mydev2,1);
	if(result1<0||result2<0)
	{
		printk("\nerror");
		unregister_chrdev_region(Mydev1,1);
		unregister_chrdev_region(Mydev2,1);
		return(-1);
	}
	sema_init(&sem_dev,1);
	return 0;
}


void __exit chardev_exit(void)
{

	unregister_chrdev_region(Mydev1,1);
	unregister_chrdev_region(Mydev2,1);
	cdev_del(my_cdev);
	cdev_del(my_cdev2);
	printk("\nUnregistered");
	
	
}

int NAME_open(struct inode *inode, struct file *filp)
{
	printk("\nThis is an open call for application 1");
	return 0;
}


int NAME_release(struct inode *inode, struct file *filp)
{
	printk("\nThis is a close call for application 1");
	return 0;
}

int NAME_open2(struct inode *inode, struct file *filp)
{
	printk("\nThis is an open call for application 2");
	return 0;
}


int NAME_release2(struct inode *inode, struct file *filp)
{
	printk("\nThis is a close call for application 2");
	return 0;

ssize_t NAME_read1(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{

	ssize_t ret;
	unsigned long remaining;
	printk("\nread command called :");
	if(down_interruptible(&sem_dev))
		return -ERESTARTSYS;

	remaining = copy_to_user((char *)Ubuff,(char *)Kbuff,count);

	if(remaining == 0)
	{
		printk("\ndata succesfully copied");
		busrd1 = 1;
		wake_up_interruptible(&mywait2);
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
	up(&sem_dev);
	return 0;
}
ssize_t NAME_read2(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{

	ssize_t ret;
	unsigned long remaining;
	printk("\nread command called :");
	if(down_interruptible(&sem_dev))
		return -ERESTARTSYS;

	remaining = copy_to_user((char *)Ubuff,(char *)Kbuff,count);

	if(remaining == 0)
	{
		printk("\ndata succesfully copied");
		busRd2 = 1;
		wake_up_interruptible(&mywait2);
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
	up(&sem_dev);
	return 0;
}

ssize_t NAME_write2(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp)
{
if(busyWr == 1)
{
	wait_event_interruptible(mywait,busWr == 0)
}

	unsigned long remaining;
	ssize_t ret;
	buswr = 1;

do{	printk("\nwriting data: \n");
	if(down_interruptible(&sem_dev))
		return -ERESTARTSYS;
	busyWr = 1;
	remaining = copy_from_user((char *)Kbuff,(char *)Ubuff,count)

	if(remaining == 0)
	{
		printk("\ndata succesfully copied");
		return count;
	}
	else if(remaining==30)
	{
		printk("data still remaining to be read");	
		wait_event_interruptible(mywait2,busRd2 == 1);
		return (count - remaining);
	}
	else
	{
		printk("\n error reading data\n");
		ret = -EFAULT;
		return ret;
	}
}
	while(remaining !=0);
	busyWr = 0;
	up(&sem_dev);
	return 0;
}

ssize_t NAME_write1(struct file *filp, const char __user *Ubuff, size_t count, loff_t *offp)
{
if(busyWr == 1)
{
	wait_event_interruptible(mywait,busWr == 0)
}


	unsigned long remaining;
	ssize_t ret;
	busWr = 1;
do{	printk("\nwriting data: \n");
	if(down_interruptible(&sem_dev))
		return -ERESTARTSYS;
	busyWr = 1;
	remaining = copy_from_user((char *)Kbuff,(char *)Ubuff,count)

	if(remaining == 0)
	{
		printk("\ndata succesfully copied");
		return count;
	}
	else if(remaining==30)
	{
		printk("data still remaining to be read");
		wait_event_interruptible(mywait2,busRd1 == 1);
		return (count - remaining);
	}
	else
	{
		printk("\n error reading data\n");
		ret = -EFAULT;
		return ret;
	}
}
	while(remaining !=0);
	up(&sem_dev);
	busyWr = 0;
	return 0;
}

module_init(chardev_init);
module_exit(chardev_exit);





