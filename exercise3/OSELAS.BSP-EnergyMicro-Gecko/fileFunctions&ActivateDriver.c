#include <stdio.h>

//static int init my init (void)
//static void exit my exit (void)

//module init (my init ); /* specifies which function will be used as init ∗/ module exit (my exit); /∗ the same, but for exit function */
//MODULELICENSE (”GPL”); /* specifies the license for the code */

//printk(KERN INFO ”Variable value %d\n”, i) 
//It prints out the value of variable i. KERN INFO means that this message is only an info message. Mark that there is no comma after KERN INFO.



/* user program that opens the driver */
/* Always the first operation performed on deivce file. Despite this, driver does not have to declare a corresponding method.
If entry is NULL, opening device always succeeds but your driver is not notified of this. */
/*  Functionality:
• Check for device-specific errors (such as device-not-ready or similar hardware problems)
• Initialize the device if it is being opened for the first time
• Update the f_op pointer, if necessary
• Allocate and fill any data structure to be put in filp->private_data
*/
static int my_open (struct inode *inode, struct file *filp) {
	struct scull_dev *dev; /* device information */

	dev = container_of(inode->i_cdev, struct scull_dev, cdev); //container_of(pointer, container_type, container_field); Macro used to find appropriate device structure
	filp->private_data = dev; /* for other methods */

	/* now trim to 0 the length of the device if open was write-only */
	if ((filp->f_flags & 0_ACCMODE) == 0_WRONLY) {
		scull_trim(dev); /* ignore errors */
	}
	return 0; /* success */
}


/* user program that closes the driver */
/* Operation invoked when file structure is being released. Can also be NULL, like open. */
/*  Functionality:
• Deallocate anything that open allocated in filp->private_data
• Shut down the device on last close
*/
static int my_release (struct inode *inode, struct file *filp) {
	return 0; //No hardware to shut down means small amount of code required
}


/* user program reads from the driver */
/* Used to retrieve data from device. A NULL pointer in this position causes the "read" system call to fail with -EINVAL (Invalid argument).
A non-negative return value represents the nmbrof Bytes successfully read. 
The return value is type "signed size", which is usually the native integer type for the target platform used. */
/*  Functionality: 
• If the value equals the count argument passed to the read system call, the
requested number of bytes has been transferred. This is the optimal case.
• If the value is positive, but smaller than count, only part of the data has been transferred. This may happen for a number of reasons, depending on the device. Most often, the application program retries the read. For instance, if you read using the fread function, the library function reissues the system call until com- pletion of the requested data transfer.
• If the value is 0, end-of-file was reached (and no data was read).
• A negative value means there was an error. The value specifies what the error was, according to <linux/errno.h>. Typical values returned on error include -EINTR (interrupted system call) or -EFAULT (bad address).
*/
static ssize_t my_read (struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	struct scull_dev *dev = filp->private_data;
	struct scull_qset *dptr;    /* the first listitem */
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset; /* how many bytes in the listitem */
	int item, s_pos, q_pos, rest;
	ssize_t retval = 0;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	if (*f_pos >= dev->size)
		goto out;
	if (*f_pos + count > dev->size)
		count = dev->size - *f_pos;

	/* find listitem, qset index, and offset in the quantum */
	item = (long)*f_pos / itemsize;
	rest = (long)*f_pos % itemsize;
	s_pos = rest / quantum; q_pos = rest % quantum;

	/* follow the list up to the right position (defined elsewhere) */
	dptr = scull_follow(dev, item);

	if (dptr == NULL || !dptr->data || ! dptr->data[s_pos]) 
		goto out; /* don't fill holes */

	 /* read only up to the end of this quantum */
	if (count > quantum - q_pos)
		count = quantum - q_pos;

	if (copy_to_user(buf, dptr->data[s_pos] + q_pos, count)) {
		retval = -EFAULT;
		goto out;
	}
	*f_pos += count;
	retval = count;

	out:
		up(&dev->sem);
		return retval;
}


/* user program that writes to the driver */
/* Sends data to the device. If NULL, -EINVAL is returned to the program calling the "write" system call. 
Return value represents nmbrof Bytes successfully written, if non-negative */
/*  Functionality: 
• If the value equals count, the requested number of bytes has been transferred.
• If the value is positive, but smaller than count, only part of the data has been
transferred. The program will most likely retry writing the rest of the data.
• If the value is 0, nothing was written. This result is not an error, and there is no reason to return an error code. Once again, the standard library retries the call to write. We’ll examine the exact meaning of this case in Chapter 6, where block- ing write is introduced.
• A negative value means an error occurred; as for read, valid error values are those defined in <linux/errno.h>.
*/
static ssize_t my_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
	struct scull_dev *dev = filp->private_data;
	struct scull_qset *dptr;
	int quantum = dev->quantum, qset = dev->qset;
	int itemsize = quantum * qset;
	int item, s_pos, q_pos, rest;
	ssize_t retval = -ENOMEM; /* value used in "goto out" statements */
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	/* find listitem, qset index and offset in the quantum */
	item = (long)*f_pos / itemsize;
	rest = (long)*f_pos % itemsize;
	s_pos = rest / quantum; q_pos = rest % quantum;

	/* follow the list up to the right position */ 
	dptr = scull_follow(dev, item);
	if (dptr == NULL)
		goto out;
	if (!dptr->data) {
		dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);
		if (!dptr->data)
			goto out;
		memset(dptr->data, 0, qset * sizeof(char *));
	}
	if (!dptr->data[s_pos]) {
		dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
		if (!dptr->data[s_pos])
			goto out; 
	}
	/* write only up to the end of this quantum */
	if (count > quantum - q_pos)
		count = quantum - q_pos;
	if (copy_from_user(dptr->data[s_pos]+q_pos, buf, count)) {
		retval = -EFAULT;
		goto out;
	}
	*f_pos += count;
	retval = count;

		/* update the size */
	if (dev->size < *f_pos)
		dev->size = *f_pos;


	out:
		up(&dev->sem);
		return retval;
}

/* Memory management
void *kmalloc(size_t size, int flags);
void kfree(void *ptr);
*/

/* Eat all RAM with scull:
cp /dev/zero /dev/scull0
*/

/* Structure to make kernel know how the functions are invoked */
//
static struct file_operations my_fops = {
	.owner = THIS_MODULE, //Pointer to module that owns the structure. Used to prevent module from getting unloaded while its operations are in use
	.read = my_read, //
	.write = my_write, 
	.open = my_open, 
	.release = my_release
};

struct cdev my_cdev;



cdev_init (&my_cdev, &my_fops);
//

/*
The scull device driver implements only the most important device methods. 
Its file_operations structure is initialized as follows:
struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.llseek = scull_llseek,
	.read = scull_read,
	.write = scull_write,
	.ioctl = scull_ioctl,
	.open = scull_open,
	.release =  scull_release,
};
*/

/* Now the cdev is initialized with a pointer to the file_operations structure, and all that remains is to pass the cdev structure to the kernel with the function: */
//cdev_add()

/* linux/module.h
#include <linux/export.h>

#define MODULE_LICENSE(__MODULE_LICENSE_value) \
	static __attribute__((unused)) const char *__MODULE_LICENSE_name = \
		__MODULE_LICENSE_value

*/

