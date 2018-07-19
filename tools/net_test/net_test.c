#include "net_test.h"

#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
//#include <linux/ethtool.h>
#include <linux/sockios.h>

#define TEST_PROTO   (0xaaaa)
#define TEST_MAGIC   (0xffffeeee)
#define TEST_ANDROID

#define ETH_LINK_SPEED  100 /* Mbps */

#define INFO_LOG_PATH  "/sdcard/log/network_test_info.log"
#define ERROR_LOG_PATH  "/sdcard/log/network_test_err.log"

typedef struct tagMacPacket {
	uint8 dst_mac[MAC_ADDR_LEN];
	uint8 src_mac[MAC_ADDR_LEN];
	uint16 type;
	uint32 magic;
	uint32 index;
	uint8 data[100];
} MAC_PACKET_T;

NET_INFO_T* g_net_info = NULL;

int net_sprintf_mac_addr(uint8* src, uint8* dst) {

	int ret = 0;
	ret = sprintf(dst, "%02x:%02x:%02x:%02x:%02x:%02x", src[0], src[1], src[2],
			src[3], src[4], src[5]);
	if (ret < 0) {
		return FAIL;
	}

	return SUCCESS;
}

int net_get_mac_addr(uint8_t *mac, uint8 *hw_buf) {

    int i;

	if (mac == NULL || hw_buf == NULL) {
		return FAIL;
	}
    if (g_net_info == NULL) {
        return FAIL;
    }

    for (i = 0; i < MAC_ADDR_LEN; i++) {
        mac[i] = g_net_info->mac[i];
    }
	net_sprintf_mac_addr(mac, hw_buf);

	return SUCCESS;
}

int net_get_mac_addr0(char* eth_name, uint8* hw_buf) {

	int fd = -1;
	struct ifreq ifr;
	uint8 buf[128] = { 0, };

	if (NULL == eth_name || NULL == hw_buf) {
		return FAIL;
	}

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "create socket failed\n");
		return FAIL;
	}

	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, eth_name, ETH_NAME_LEN);

	if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "get mac addr failed\n");
		close(fd);
		return FAIL;
	}

	memcpy(hw_buf, ifr.ifr_hwaddr.sa_data, MAC_ADDR_LEN);
	close(fd);

	net_sprintf_mac_addr(hw_buf, buf);

	return SUCCESS;
}

int net_get_eth_index(char* eth_name, uint32* index) {

	int fd = -1;
	struct ifreq ifr;

	if (NULL == eth_name) {
		return FAIL;
	}

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "create socket failed\n");
		return FAIL;
	}

	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, eth_name, ETH_NAME_LEN);

	if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "get eth index failed\n");
		close(fd);
		return FAIL;
	}

	*index = ifr.ifr_ifindex;
	// WRITE_INFO(INFO_LOG_PATH, "get net_get_eth_index %d\n", ifr.ifr_ifindex);
	close(fd);

	return SUCCESS;
}

static int net_get_eth_name(char* eth_name, int size) {

	int i = 0;
	int num = 0;
	int fd = -1;
	int ret = 0;
	struct ifconf ifconf;
	uint8 buf[512] = { 0, };
	struct ifreq *ifreq = NULL;    

	if (NULL == eth_name) {
		return FAIL;
	}

    // set default value
    strcpy(eth_name, "eth0");    

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "create socket failed\n");
		return FAIL;
	}

	ifconf.ifc_len = 512;
	ifconf.ifc_buf = buf;

	ret = ioctl(fd, SIOCGIFCONF, &ifconf);
	if (ret < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "get net config failed\n");
		close(fd);
		return FAIL;
	}

	ifreq = (struct ifreq*) buf;
	num = ifconf.ifc_len / sizeof(struct ifreq);

	for (i = 0; i < num; i++) {
		// WRITE_INFO(INFO_LOG_PATH, "net card name %s\n", ifreq->ifr_name);
		if (0 == strncmp("eth", ifreq->ifr_name, 3)) {
			strncpy(eth_name, ifreq->ifr_name, size);
		}

		ifreq++;
	}

	close(fd);

	return SUCCESS;
}

static inline int net_test_ioctl(int fd, char *eth_name, void *cmd)
{
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, eth_name, ETH_NAME_LEN);
    ifr.ifr_data = cmd;
    return ioctl(fd, SIOCETHTOOL, &ifr);
}

static int net_get_eth_status(int fd, char *eth_name, int *status) {

	struct ifreq ifr;

	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, eth_name, ETH_NAME_LEN);
	if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "get eth status failed: %s\n", strerror(errno));
		return FAIL;
	}

	// ifr.ifr_flags & IFF_RUNNING  --->link
	if ((ifr.ifr_flags & IFF_UP)) {
		*status = ETH_STATUS_UP;
	} else {
		*status = ETH_STATUS_DOWN;
	}

	// WRITE_INFO(INFO_LOG_PATH, "cur eth up: %d\n", !*status);
	return SUCCESS;
}

static int net_get_eth_info(NET_INFO_T *info)
{
    int fd, ret;
#ifndef TEST_ANDROID
    struct ethtool_cmd ecmd;
    struct ethtool_value edata;
#else
    char fileName[50] = {0};
    FILE *fp = NULL;
    char line[10] = {0};
	size_t len = 0;
    ssize_t read;
#endif

    if (info == NULL || info->eth_name == NULL) {
    	WRITE_ERROR(ERROR_LOG_PATH, "pointer NULL error!\n");
        return FAIL;
    }
    fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "create socket failed: %s\n", strerror(errno));
		return FAIL;
	}

    /* Get eth status */
    ret = net_get_eth_status(fd, info->eth_name, &info->eth_status);
    if (ret == FAIL)
        return FAIL;
#ifndef TEST_ANDROID
    /* Get eth link */
    edata.cmd = ETHTOOL_GLINK;
    ret = net_test_ioctl(fd, info->eth_name, &edata);
    if (ret < 0) {
    	WRITE_ERROR(ERROR_LOG_PATH, "get eth link failed: %s\n", strerror(errno));
        return FAIL;
    }
    info->eth_link = edata.data;

    /* Get seed & duplex */
    ecmd.cmd = ETHTOOL_GSET;
    ret = net_test_ioctl(fd, info->eth_name, &ecmd);
    if (ret < 0) {
    	WRITE_ERROR(ERROR_LOG_PATH, "get eth speed & duplex info failed: %s\n", strerror(errno));
        return FAIL;
    }
    info->eth_speed = (ecmd.speed_hi << 16) | ecmd.speed;
    info->eth_duplex = ecmd.duplex;
#else
    snprintf(fileName, 50, "/sys/class/net/%s/duplex", info->eth_name);
    if((fp=fopen(fileName,"r")) == NULL)
    {
    	WRITE_ERROR(ERROR_LOG_PATH, "cat open file:%s", fileName);
    	return FAIL;
	}
    if (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "half", 4) == 0) {
            info->eth_duplex = DUPLEX_HALF;
		} else if (strncmp(line, "full", 4) == 0) {
    		info->eth_duplex = DUPLEX_FULL;
		} else {
			WRITE_INFO(INFO_LOG_PATH, "\tERROR: unknown duplex mode (%s)!!\n", line);
		}
	}
    fclose(fp);
    fp = NULL;

    memset(line, 0, sizeof(line));
    snprintf(fileName, 50, "/sys/class/net/%s/speed", info->eth_name);
    if((fp=fopen(fileName,"r")) == NULL)
	{
    	WRITE_ERROR(ERROR_LOG_PATH, "cat open file:%s", fileName);
		return FAIL;
	}
    if (fgets(line, sizeof(line), fp) != NULL) {
    	info->eth_speed = atoi(line);
	}
	fclose(fp);
	fp = NULL;
#endif
    close(fd);
    return SUCCESS;
}

/*
int net_send_msg(char* src_mac, char* dst_mac, uint32 index, uint32 seq) {

	int fd = -1;
	int ret = 0;
	MAC_PACKET_T packet;
	struct sockaddr_ll sll;

	fd = socket(PF_PACKET, SOCK_RAW, htons(TEST_PROTO));
	if (fd < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "create raw sock failed\n");
		return FAIL;
	}

	memset(&sll, 0, sizeof(struct sockaddr_ll));
	sll.sll_ifindex = index;
	memset(&packet, 0, sizeof(MAC_PACKET_T));

	memcpy(&packet.dst_mac, dst_mac, MAC_ADDR_LEN);
	memcpy(&packet.src_mac, src_mac, MAC_ADDR_LEN);
	packet.type = TEST_PROTO;
	packet.magic = TEST_MAGIC;
	packet.index = seq;
//	WRITE_INFO(INFO_LOG_PATH, "send packet dstMac:%02x:%02x:%02x:%02x:%02x:%02x, srcMac:%02x:%02x:%02x:%02x:%02x:%02x, "
//			"type:%d, magic:%d, index:%d\n", packet.dst_mac[0], packet.dst_mac[1], packet.dst_mac[2], packet.dst_mac[3], packet.dst_mac[4], \
//			packet.dst_mac[5], packet.src_mac[0], packet.src_mac[1], packet.src_mac[2], packet.src_mac[3], packet.src_mac[4], packet.src_mac[5], \
//			packet.type, packet.magic, packet.index);

	ret = sendto(fd, (void*) &packet, sizeof(packet), 0,
			(struct sockaddr *) &sll, sizeof(sll));
	if (ret < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "send failed ret=%d errno=%d\n", ret, errno);
		close(fd);
		return FAIL;
	}
//	WRITE_INFO(INFO_LOG_PATH, "send packet size:%d\n", ret);
	close(fd);
	return SUCCESS;
}
*/

int net_msg_sender_open() {
	int fd = -1;

	fd = socket(PF_PACKET, SOCK_RAW, htons(TEST_PROTO));
	if (fd < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "create raw sock failed\n");
		return -1;
	}

	return fd;
}

int net_msg_sender_write(int fd, uint8* src_mac, uint8* dst_mac, uint32 index, uint32 seq) {

	int ret = 0;
	MAC_PACKET_T packet;
	struct sockaddr_ll sll;

	memset(&sll, 0, sizeof(struct sockaddr_ll));
	sll.sll_ifindex = index;
	memset(&packet, 0, sizeof(MAC_PACKET_T));

	memcpy(&packet.dst_mac, dst_mac, MAC_ADDR_LEN);
	memcpy(&packet.src_mac, src_mac, MAC_ADDR_LEN);
	packet.type = TEST_PROTO;
	packet.magic = TEST_MAGIC;
	packet.index = seq;
//	WRITE_INFO(INFO_LOG_PATH, "send packet dstMac:%02x:%02x:%02x:%02x:%02x:%02x, srcMac:%02x:%02x:%02x:%02x:%02x:%02x, "
//			"type:%d, magic:%d, index:%d\n", packet.dst_mac[0], packet.dst_mac[1], packet.dst_mac[2], packet.dst_mac[3], packet.dst_mac[4], \
//			packet.dst_mac[5], packet.src_mac[0], packet.src_mac[1], packet.src_mac[2], packet.src_mac[3], packet.src_mac[4], packet.src_mac[5], \
//			packet.type, packet.magic, packet.index);

	ret = sendto(fd, (void*) &packet, sizeof(packet), 0,
			(struct sockaddr *) &sll, sizeof(sll));
	if (ret < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "send failed ret=%d errno=%d\n", ret, errno);
		return FAIL;
	}

	return SUCCESS;
}

void net_msg_sender_close(int fd)
{
	close(fd);
}


int net_send_broadcast_msg(NET_INFO_T* info, int num) {

	int i = 0;
	int ret = 0;
	int fd = -1;
	uint8 dest_mac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	fd = net_msg_sender_open();
	if (fd < 0)
		return -1;

	for (i = 0; i < num; i++) {
		ret |= net_msg_sender_write(fd, info->mac, dest_mac, info->eth_index, info->seq++);
		usleep(10000);
	}
	
	net_msg_sender_close(fd);

	return ret;
}

void* net_recv_loopback_msg(void *arg) {

	int fd = -1;
	int sender_fd = -1;
	int ret = 0;
	int len = 0;
	MAC_PACKET_T recv_packet;
	MAC_PACKET_T send_packet;
	struct sockaddr_ll recv_sll;
	struct sockaddr_ll send_sll;
	NET_INFO_T* info = NULL;
	uint8 buf[128] = { 0, };
	char bc_mac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    pthread_detach(pthread_self());
	info = (NET_INFO_T*) arg;

	start: fd = socket(PF_PACKET, SOCK_RAW, htons(TEST_PROTO));
	if (fd < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "create raw sock failed\n");
		return NULL;
	}

	sender_fd = net_msg_sender_open();
	if (sender_fd < 0) {
		return NULL;
	}

	memset(&recv_sll, 0, sizeof(struct sockaddr_ll));
	recv_sll.sll_family = PF_PACKET;
	recv_sll.sll_ifindex = info->eth_index;
	recv_sll.sll_protocol = htons(TEST_PROTO);
	// WRITE_INFO(INFO_LOG_PATH, "socket recv index:%d, protocol:%d\n", recv_sll.sll_ifindex, recv_sll.sll_protocol);
	ret = bind(fd, (struct sockaddr *) &recv_sll, sizeof(recv_sll));
	if (ret < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "bind recv socket failed ret=%d errno=%d\n", ret, errno);
		close(fd);
		net_msg_sender_close(sender_fd);
		return NULL;
	}

	while (1) {
		// WRITE_INFO(INFO_LOG_PATH, "start recv packet\n");
		len = recvfrom(fd, &recv_packet, sizeof(recv_packet), 0, 0, 0);
		if (len != sizeof(recv_packet)) {
			WRITE_ERROR(ERROR_LOG_PATH, "recv len failed %d\n", len);
			close(fd);
			net_msg_sender_close(sender_fd);
			usleep(10000);
			goto start;
		}

		if (TEST_MAGIC != recv_packet.magic) {
			WRITE_ERROR(ERROR_LOG_PATH, "recv magic=%d is invalid\n", recv_packet.magic);
			close(fd);
			net_msg_sender_close(sender_fd);
			goto start;
		}

		// exchange will transform message
		if (0 != memcmp(recv_packet.dst_mac, bc_mac, MAC_ADDR_LEN)) {
//			WRITE_INFO(INFO_LOG_PATH, "recv roll back msg index=%d\n", recv_packet.index);
			info->recv_num++;
		} else {
			// send back
			net_msg_sender_write(sender_fd, info->mac, recv_packet.src_mac, info->eth_index,
					recv_packet.index);

			net_sprintf_mac_addr(recv_packet.src_mac, buf);

//			WRITE_INFO(INFO_LOG_PATH, "recv dst mac: %02x:%02x:%02x:%02x:%02x:%02x, send back to %s index=%d\n", recv_packet.dst_mac[0], \
//					recv_packet.dst_mac[1], recv_packet.dst_mac[2], recv_packet.dst_mac[3], recv_packet.dst_mac[4], recv_packet.dst_mac[5], buf, recv_packet.index);
		}
	}

	return NULL;
}

static inline char *net_get_duplex_desc(uint8_t duplex)
{
    switch (duplex) {
    case DUPLEX_HALF:
        return "Half";
    case DUPLEX_FULL:
        return "Full";
    default:
        return "Unknown!";
    }
}

int net_test_all() {

	int i = 0;
	int ret = 0;
	NET_INFO_T *info = NULL;
	time_t timep;

	info = g_net_info;
    if (NULL == info){
    	WRITE_ERROR(ERROR_LOG_PATH, "net info is null");
        return FAIL;    
    }
    
    time(&timep);
    WRITE_INFO(INFO_LOG_PATH, "\nnet test start: %s", ctime(&timep));
    WRITE_INFO(INFO_LOG_PATH, "\tNetwork card name: \t\t\t%s \n", info->eth_name);
    WRITE_INFO(INFO_LOG_PATH, "\tNetwork card addr: \t\t\t%02x:%02x:%02x:%02x:%02x:%02x \n", info->mac[0], info->mac[1],
			info->mac[2], info->mac[3], info->mac[4], info->mac[5]);
    WRITE_INFO(INFO_LOG_PATH, "\tNetwork card index: \t\t\t%d \n", info->eth_index);

	ret = net_get_eth_info(info);
	if (ret == FAIL) {
		WRITE_ERROR(ERROR_LOG_PATH, "get eth status failed!\n");
		goto error;
	}

	if (info->eth_status == ETH_STATUS_UP) {
		WRITE_INFO(INFO_LOG_PATH, "\tNetwork card status: \t\tup\n");
	} else {
		WRITE_INFO(INFO_LOG_PATH, "\tNetwork card status: \t\tdown\n");
		WRITE_INFO(INFO_LOG_PATH, "\tERROR: network is down!\n");
        ret = FAIL;
        goto error;
	}

#ifndef TEST_ANDROID
	if (info->eth_link) {
		WRITE_INFO(INFO_LOG_PATH, "\tNetwork link detected: \t\tyes\n");
	} else {
		WRITE_INFO(INFO_LOG_PATH, "\tNetwork link detected: \t\tno\n");
		WRITE_INFO(INFO_LOG_PATH, "\tERROR: network is not linked!\n");
        ret = FAIL;
        goto error;
	}
#endif

    if (info->eth_speed == 0
        || info->eth_speed == (uint16_t)(-1)
        || info->eth_speed == (uint32_t)(-1)) {
    	WRITE_INFO(INFO_LOG_PATH, "\tNetwork card speed: \t\tUnknown!\n");
        ret = FAIL;
    } else {
    	WRITE_INFO(INFO_LOG_PATH, "\tNetwork card speed: \t\t%uMbps\n", info->eth_speed);
        if (info->eth_speed != ETH_LINK_SPEED) {
        	WRITE_INFO(INFO_LOG_PATH, "\tERROR: Network speed must be %uMbps but current is %uMbps\n",
                ETH_LINK_SPEED, info->eth_speed);
            ret = FAIL;
        }
    }
    WRITE_INFO(INFO_LOG_PATH, "\tNetwork card duplex: \t\t%s (%i)\n",
    net_get_duplex_desc(info->eth_duplex), info->eth_duplex);
    if (info->eth_duplex != DUPLEX_FULL) {
     	WRITE_INFO(INFO_LOG_PATH, "\tERROR: Network duplex mode must be DUPLEX_FULL!!\n");
		ret = FAIL;
	}

	info->recv_num = 0;
	sleep(2);
	net_send_broadcast_msg(info, 100);

	// wait for 2 seconds
	while (1) {
		i++;
		if (100 == info->recv_num
			|| 100 == i){
			break;
		}
		usleep(20000);
	}

	if (info->recv_num < 95) {
		ret = FAIL;
	}

	WRITE_INFO(INFO_LOG_PATH, "\tsend package num:\t\t%d\n", 100);
	WRITE_INFO(INFO_LOG_PATH, "\trecv package num:\t\t%d\n", info->recv_num);

	error:
	WRITE_INFO(INFO_LOG_PATH, "net test result: \t\t\t\t%s", PRINT_RESULT1(ret));
	time(&timep);
	WRITE_INFO(INFO_LOG_PATH, "\nnet test end: %s", ctime(&timep));
	return ret;
}

int net_test_init() {

	int ret = 0;
	pthread_t pid;
	NET_INFO_T* info;

	if (access(LOG_DIR, 0) == -1)
	{
		int flag = mkdir(LOG_DIR, 0777);
		if (flag != 0)
		{
			WRITE_ERROR(ERROR_LOG_PATH, "\ncreate %s failed\n", LOG_DIR);
			return FAIL;
		}
	}

	if (access(INFO_LOG_PATH, 0) == 0)
	{
		remove(INFO_LOG_PATH);
	}
	if (access(ERROR_LOG_PATH, 0) == 0)
	{
		remove(ERROR_LOG_PATH);
	}

	info = (NET_INFO_T *)malloc(sizeof(NET_INFO_T));
	if (!info) {
		return FAIL;
	}

	memset(info, 0, sizeof(NET_INFO_T));

	//目前只测eth0
	strncpy(info->eth_name, "eth0", sizeof(info->eth_name));
	/*
	ret = net_get_eth_name(info->eth_name, ETH_NAME_LEN);
	if (FAIL == ret) {
		WRITE_ERROR(ERROR_LOG_PATH, "get eth name failed\n");
		return FAIL;
	}
	*/

	ret = net_get_eth_index(info->eth_name, &info->eth_index);
	if (FAIL == ret) {
		WRITE_ERROR(ERROR_LOG_PATH, "get eth index failed\n");
		return FAIL;
	}

	ret = net_get_mac_addr0(info->eth_name, info->mac);
	if (FAIL == ret) {
		WRITE_ERROR(ERROR_LOG_PATH, "get mac addr failed\n");
		return FAIL;
	}

	ret = pthread_create(&pid, NULL, net_recv_loopback_msg, info);
	if (ret < 0) {
		WRITE_ERROR(ERROR_LOG_PATH, "create thread failed\n");
		return FAIL;
	}

	g_net_info = info;

	return ret;
}

int main() {
	net_test_init();
	net_test_all();
        while (1) {
            sleep(1);
        }
	return 0;
}
