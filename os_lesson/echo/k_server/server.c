
#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/ctype.h>
#include <linux/unistd.h>
#include <linux/mutex.h>

#define BUF_SIZE              1024
#define NETLINK_USER          31
#define CLIENTS               10
#define PROCFS_MAX_SIZE		  1024
#define QUIT                  "quit"
#define MODE                  "mode"
#define UPPER                 "upper"
#define LOWER                 "lower"
#define PROCFS_NAME 		  "socket_config"
#define PROCFS_SERVER         "server_info"

typedef struct server_info {
    int client_id;
    char mode[10];
    int client_cnt;
}server_info;

struct server_info info[CLIENTS];
struct sock       *nl_sk = NULL;
struct mutex       mutex;
static struct proc_dir_entry *proc_file;
static struct proc_dir_entry *proc_file_info;

static int     char_cnt   = 0;       //字符计数
static int     client_cnt = 0;       //客户端计数
static int     len_check  = 1;       //防止cat打印多次
static unsigned long procfs_buffer_size = 0;
static char    gMode[10];            //默认全局配置
static char    procfs_buffer[PROCFS_MAX_SIZE];

static void upper_process(char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        data[i] = toupper(data[i]);
        msleep(200);
    }
	//printk(KERN_INFO "upper server to client\n");
    return;
}

static void lower_process(char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        data[i] = tolower(data[i]);
        msleep(200);
    }
	//printk(KERN_INFO "lower server to client\n");
    return;
}

static void normal_process(char data[], int len)
{
    int i = 0;

    for( i = 0; i < len; i++) {
        msleep(200);
    }
	//printk(KERN_INFO "normal server to client\n");
    return;
}

static int find_clinet(int id, server_info *val)
{
    int i = 0;

    mutex_lock(&mutex);
    if(client_cnt == (CLIENTS-1)) {
        printk(KERN_INFO "only support the 10 most clients");
        mutex_unlock(&mutex);
        return 2;
    }

    for( i = 0; i < CLIENTS; i++) {
        if(info[i].client_id == id) {
            strncpy(val->mode, info[i].mode, strlen(info[i].mode));
            mutex_unlock(&mutex);
            return 1;
        }
    }

    info[client_cnt].client_id = id;
    strncpy(info[client_cnt].mode, gMode, strlen(gMode));   //设置默认配置模式
    strncpy(val->mode, info[client_cnt].mode, strlen(info[client_cnt].mode));
    client_cnt++;
    mutex_unlock(&mutex);
	//printk(KERN_INFO "add the new client\n");

    return 0;
}


static void update_clinet(int id, server_info val)
{
    int i = 0;

    mutex_lock(&mutex);
    if(client_cnt == (CLIENTS-1)) {
        printk(KERN_INFO "only support the 10 most clients");
        mutex_unlock(&mutex);
        return ;
    }

    for( i = 0; i < CLIENTS; i++) {
        if(info[i].client_id == id) {
            strncpy(info[i].mode, val.mode, strlen(val.mode));
            info[i].client_id = val.client_id;
            mutex_unlock(&mutex);
            return ;
        }
    }
    mutex_unlock(&mutex);

    return ;
}

static void nl_recv_msg(struct sk_buff *skb)
{
    int              pid;
	int              msg_size;
	int              res;
	char             msg[BUF_SIZE] = {0};
	struct nlmsghdr *nlh;
	struct sk_buff  *skb_out;
    struct server_info info_buf;

    memset(&info_buf, 0, sizeof(info_buf));
	//printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

    //接收数据
	nlh = (struct nlmsghdr *)skb->data;
	//printk(KERN_INFO "Netlink received msg from user:%s\n ", (char *)nlmsg_data(nlh));

    pid = nlh->nlmsg_pid;            /* the client's pid */

    if( find_clinet(pid, &info_buf) == 2) {
        return;
    }

    //配置发送数据
    msg_size = strlen((char *)nlmsg_data(nlh));
    strncpy(msg, (char *)nlmsg_data(nlh), msg_size);
    //printk(KERN_INFO "msg from user:%s len:%d pid:%d", msg, strlen(msg), pid);
    if (strncmp(msg, QUIT, sizeof(QUIT)-1) == 0) {
        info_buf.client_id = 0;
        update_clinet(pid, info_buf);
        mutex_lock(&mutex);
        client_cnt--;
        mutex_unlock(&mutex);
        printk(KERN_INFO "Disconnected from the client %d!", pid);

        goto out_msg;
    }

    if (strncmp(info_buf.mode, UPPER, sizeof(UPPER)-1) == 0) {
        upper_process(msg, strlen(msg));
    }
    else if(strncmp(info_buf.mode, LOWER, sizeof(LOWER)-1) == 0) {
        lower_process(msg, strlen(msg));
    } else {
        normal_process(msg, strlen(msg));
    }
    mutex_lock(&mutex);
    char_cnt += (strlen(msg) - 1);
    mutex_unlock(&mutex);

out_msg:
    //发送数据(单播)
    skb_out = nlmsg_new(msg_size, 0);
    if (!skb_out) {
        printk(KERN_ERR "Failed to allocate new skb\n");
        return;
    }

    nlh  = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0;
    strncpy(nlmsg_data(nlh), msg, msg_size);

    res = nlmsg_unicast(nl_sk, skb_out, pid);
    if (res < 0)
        printk(KERN_INFO "Error while sending bak to user %d\n", res);
}


static ssize_t procfile_read(struct file *file,
			                 char *buffer,
			                 size_t length,
			                 loff_t *offset)
{
	if (len_check) {
        len_check = 0;
    } else {
 		len_check = 1;
 		return 0;
	}

	//将buffer的数据送回给用户
	copy_to_user(buffer,procfs_buffer,procfs_buffer_size);
    //printk(KERN_INFO "the user write:%s", procfs_buffer);

	return procfs_buffer_size;
}


static ssize_t procfile_write(struct file *file,
			                  const char __user *buf,
			                  size_t size,
	 		                  loff_t *offset)
{
	//printk(KERN_INFO "proc called write %d bytes for the write\n", size);
    int  ret              = 0;
    int  pid              = 0;
    char key_tag[10]      = {0};
    char mode_buf[10]     = {0};
    struct server_info info_buf;

    memset(&info_buf, 0, sizeof(info_buf));
	procfs_buffer_size = size;
	if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}

	//将用户数据写入buffer
	if ( copy_from_user(procfs_buffer, buf, procfs_buffer_size) ) {
		return -EFAULT;
	}

    //配置服务器模式
    ret = sscanf(procfs_buffer, "mode %s %s", key_tag, mode_buf);
    if (ret < 2 ) {
        printk(KERN_INFO "the config not adapte,again");
        return procfs_buffer_size;
    }
	//printk(KERN_INFO "pid:%s mode:%s", key_tag, mode_buf);

    if (key_tag[0] == '0') {
        mutex_lock(&mutex);
        memcpy(gMode, mode_buf, strlen(mode_buf));
        mutex_unlock(&mutex);
        return procfs_buffer_size;
    } else {
        pid = simple_strtol(key_tag, NULL, 10);
        if( find_clinet(pid, &info_buf) == 1 ) {
            memcpy(info_buf.mode, mode_buf, strlen(mode_buf));
            info_buf.client_id = pid;
            update_clinet(pid, info_buf);
        }
    }

	return procfs_buffer_size;
}


static ssize_t procfile_info_read(struct file *file,
			                 char *buffer,
			                 size_t length,
			                 loff_t *offset)
{
	if (len_check) {
        len_check = 0;
    } else {
 		len_check = 1;
 		return 0;
	}

    char buf[PROCFS_MAX_SIZE] = {0};

    mutex_lock(&mutex);
    sprintf(buf, "Server info:\n"
                 "Global mode: %s\n"
                 "Characters: %d\n"
                 "Clients: %d\n", gMode, char_cnt, client_cnt);
    mutex_unlock(&mutex);
	//将buffer的数据送回给用户
	copy_to_user(buffer, buf, strlen(buf));
    //printk(KERN_INFO "the user read:%s \n length:%d\n", buf, strlen(buf));

	return strlen(buf);
}


// the operations for socket_config
static struct file_operations cmd_file_ops = {
	.owner = THIS_MODULE,
	.read = procfile_read,
	.write = procfile_write,
};

// the operations for server_info
static struct file_operations cmd_file_ops_info = {
	.owner = THIS_MODULE,
	.read = procfile_info_read,
};

static int __init server_init(void)
{
	printk("Entering: %s\n", __FUNCTION__);

    memset(info, 0, CLIENTS*sizeof(struct server_info));
    memcpy(gMode, UPPER, sizeof(UPPER));

	struct netlink_kernel_cfg cfg = {
		.input  = nl_recv_msg,   // 接收回调函数
	};

    //创建netlink连接
	nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
	if (!nl_sk) {
		printk(KERN_ALERT "Error creating socket.\n");
		return -10;
	}

	//创建proc
	proc_file = proc_create(PROCFS_NAME, S_IFREG | S_IRUGO | S_IWUSR,
	                        NULL, &cmd_file_ops);
    if (proc_file == NULL) {
        printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", PROCFS_NAME);
        return -ENOMEM;
    }
	proc_file_info = proc_create(PROCFS_SERVER, S_IFREG | S_IRUGO | S_IWUSR,
                                 NULL, &cmd_file_ops_info);
    if (proc_file_info == NULL) {
        printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", PROCFS_SERVER);
        return -ENOMEM;
    }

    mutex_init(&mutex);
    //KUIDT_INIT is a macro defined in the file 'linux/uidgid.h'. KGIDT_INIT also appears here.
    //proc_set_user(proc_file, KUIDT_INIT(0), KGIDT_INIT(0));
    //proc_set_size(proc_file, 37);

    //proc_set_user(proc_file_info, KUIDT_INIT(1), KGIDT_INIT(1));
    //proc_set_size(proc_file_info, 38);

	printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);
	printk(KERN_INFO "/proc/%s created\n", PROCFS_SERVER);

	return 0;
}

static void __exit server_exit(void)
{

	printk(KERN_INFO "exiting server module\n");
	netlink_kernel_release(nl_sk);
	remove_proc_entry(PROCFS_NAME, NULL);
	remove_proc_entry(PROCFS_SERVER, NULL);

	printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
	printk(KERN_INFO "/proc/%s removed\n", PROCFS_SERVER);
}

module_init(server_init);
module_exit(server_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LaMar");



