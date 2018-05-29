
#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/ctype.h>
#include <linux/unistd.h>




#define BUF_SIZE  1024
#define NETLINK_USER 31
#define GROUP 1
#define MODE         "mode"
#define UPPER        "upper"
#define LOWER        "lower"

//procfs
#define PROCFS_MAX_SIZE		1024
#define PROCFS_NAME 		"socket_config"
#define PROCFS_SERVER       "server_info"

struct sock *nl_sk = NULL;

//procfs
//This structure hold information about the /proc file
static struct proc_dir_entry *proc_file;
//for printf function
static int len_check = 1;
//The buffer used to store character for this module
static char procfs_buffer[PROCFS_MAX_SIZE];
//The size of the buffer
static unsigned long procfs_buffer_size = 0;

static char mode[10] = {0};

static void upper_process(char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        data[i] = toupper(data[i]);
        msleep(200);
    }
	printk(KERN_INFO "upper server to client\n");

    return;
}

static void lower_process(char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        data[i] = tolower(data[i]);
        msleep(200);
    }
	printk(KERN_INFO "lower server to client\n");

    return;
}

static void normal_process(char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        msleep(200);
    }
	printk(KERN_INFO "normal server to client\n");

    return;
}


static void nl_recv_msg(struct sk_buff *skb)
{

	struct nlmsghdr *nlh;
	struct sk_buff * skb_out;
	int              msg_size;
	char             msg[BUF_SIZE] = {0};
	int              res;

	printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

    //接收数据
	nlh = (struct nlmsghdr *)skb->data;
	printk(KERN_INFO "Netlink received msg from user:%s\n", (char *)nlmsg_data(nlh));

    //配置发送数据
	msg_size = strlen((char *)nlmsg_data(nlh));
	printk(KERN_INFO "Entering: %s\n", __FUNCTION__);
    strncpy(msg, (char *)nlmsg_data(nlh), msg_size);
	printk(KERN_INFO "msg from user:%s len:%d\n", msg, strlen(msg));


    upper_process(msg, strlen(msg));

	//发送数据(多播)
	skb_out = nlmsg_new(msg_size, 0);
	if (!skb_out) {
		printk(KERN_ERR "Failed to allocate new skb\n");
		return;
	}

	nlh  = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
	NETLINK_CB(skb_out).dst_group = GROUP;
	strncpy(nlmsg_data(nlh), msg, msg_size);

	res = nlmsg_multicast(nl_sk, skb_out, 0, GROUP, 0);
	if (res < 0)
		printk(KERN_INFO "Error while sending bak to user %d\n", res);
}


static ssize_t procfile_read(struct file *file,
			                 char *buffer,
			                 size_t length,
			                 loff_t *offset)
{
	if (len_check)
 	len_check = 0;
	else {
 		len_check = 1;
 		return 0;
	}

	printk(KERN_INFO "procfile_read (/proc/%s) called\nlength:%d\n"
           "procfs_buffer_size:%d\noffset:%d\n", PROCFS_NAME, length,
           procfs_buffer_size, offset);

		//将buffer的数据送回给用户
		copy_to_user(buffer,procfs_buffer,procfs_buffer_size);

        procfs_buffer[PROCFS_MAX_SIZE-1] = 0;
        //printk(KERN_INFO "the user write:%s", procfs_buffer);

		return procfs_buffer_size;
}


static ssize_t procfile_write(struct file *file,
			                  const char __user *buf,
			                  size_t size,
	 		                  loff_t *offset)	/* a seek position */
{
	printk(KERN_INFO "proc called write %d bytes for the write\n", size);

	procfs_buffer_size = size;
	if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}

	//将用户数据写入buffer
	if ( copy_from_user(procfs_buffer, buf, procfs_buffer_size) ) {
		return -EFAULT;
	}
    procfs_buffer[PROCFS_MAX_SIZE-1] = 0;
	printk(KERN_INFO "the user write:%s", procfs_buffer);

	return procfs_buffer_size;
}

/*
 * The file is opened - we don't really care about
 * that, but it does mean we need to increment the
 * module's reference count.
 */
int procfs_open(struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);
	return 0;
}

/*
 * The file is closed - again, interesting only because
 * of the reference count.
 */
int procfs_close(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	return 0;		/* success */
}

static struct file_operations cmd_file_ops = {
	.owner = THIS_MODULE,
	.read = procfile_read,
	.write = procfile_write,
	.open = procfs_open,
	.release = procfs_close,
};


static int __init server_init(void)
{
	printk("Entering: %s\n", __FUNCTION__);

	struct netlink_kernel_cfg cfg = {
		.input  = nl_recv_msg,   // 接收回调函数
		.groups = GROUP,         // 组编号
	};

	nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
	if (!nl_sk) {

		printk(KERN_ALERT "Error creating socket.\n");
		return -10;
	}

	//创建proc
	proc_file = proc_create(PROCFS_NAME, S_IFREG | S_IRUGO | S_IWUSR, NULL, &cmd_file_ops);
    if (proc_file == NULL) {
        printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", PROCFS_NAME);
        return -ENOMEM;
    }

    /*
     * KUIDT_INIT is a macro defined in the file 'linux/uidgid.h'. KGIDT_INIT also appears here.
     */
    proc_set_user(proc_file, KUIDT_INIT(0), KGIDT_INIT(0));
    proc_set_size(proc_file, 37);

	printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);

	return 0;
}

static void __exit server_exit(void)
{

	printk(KERN_INFO "exiting server module\n");
	netlink_kernel_release(nl_sk);
	remove_proc_entry(PROCFS_NAME, NULL);
	printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}

module_init(server_init);
module_exit(server_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LaMar");
