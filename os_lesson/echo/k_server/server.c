
#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#define NETLINK_USER 31
#define GROUP 1

struct sock *nl_sk = NULL;

static void nl_recv_msg(struct sk_buff *skb)
{

	struct nlmsghdr *nlh;
	struct sk_buff * skb_out;
	int              msg_size;
	char *           msg = "1. Hello from kernel";
	int              res;

	printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

    //接收数据
	nlh = (struct nlmsghdr *)skb->data;
	printk(KERN_INFO "Netlink received msg payload:%s\n", (char *)nlmsg_data(nlh));

	//发送数据(多播)
	msg_size = strlen(msg);
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

	return 0;
}

static void __exit server_exit(void)
{

	printk(KERN_INFO "exiting server module\n");
	netlink_kernel_release(nl_sk);
}

module_init(server_init);
module_exit(server_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LaMar");
