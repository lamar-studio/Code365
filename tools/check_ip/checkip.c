
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>

#include "checkip.h"

#define bcmp(s1 ,s2, n) memcmp((s1), (s2), (size_t)(n))

#define LOG_EMERG	"EMERGENCY!"
#define LOG_ALERT	"ALERT!"
#define LOG_CRIT	"critical!"
#define LOG_WARNING	"warning"
#define LOG_ERR	    "error"
#define LOG_INFO	"info"
#define LOG_DEBUG	"debug"
#define LOG(level, str, args...) do { printf("%s, ", level); \
        printf(str, ## args); \
        printf("\n"); } while(0)

#define DBG
#ifdef DBG
#define DEBUG(level, str, args...) LOG(level, str, ## args)
#else
#define DEBUG(level, str, args...)
#endif

/* network card interface */
#define OPTION_INTERFACE    "-i"

/* print debug log */
#define OPTION_DEBUG        "-d"

/* listening arp for ip conflit checking */
#define OPTION_LISTEN       "-l"

/* print arp info all the time, no return. */
#define OPTION_LISTEN_ALL_TIME "-a"

/**
 * set: set source ip address to 0, to check ip conflict after ip address has set.
 * not set: set source ip address to local ip address, to check ip conflict before ip address has set.
 */
#define OPTION_SEND_ARP     "-s"

char is_listen = 0;
char is_debug = 0;
char listen_all_time = 0;
char is_send_arp = 0;

#define DEBUG_PRT(level, format, args...)   \
{                                           \
	if (is_debug) {                         \
        printf(format"\n", ## args);        \
    }                                       \
}

/* miscellaneous defines */
#define MAC_BCAST_ADDR	(unsigned char *) "\xff\xff\xff\xff\xff\xff"
#define ETH_INTERFACE       "eth0"
/* globals */
struct server_config_t server_config;

int read_interface(char *interface, int *ifindex, u_int32_t *addr, unsigned char *arp)
{
    int fd;
    struct ifreq ifr;
    struct sockaddr_in *our_ip;

    memset(&ifr, 0, sizeof(struct ifreq));
    if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
        ifr.ifr_addr.sa_family = AF_INET;
        strcpy(ifr.ifr_name, interface);

        /*this is not execute*/
        if (addr) {
            if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
                our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
                *addr = our_ip->sin_addr.s_addr;
                DEBUG_PRT(LOG_INFO, "%s (our ip) = %s", ifr.ifr_name, inet_ntoa(our_ip->sin_addr));
            } else {
            	DEBUG_PRT(LOG_ERR, "SIOCGIFADDR failed, is the interface up and configured?: %s",
                        strerror(errno));
                return -1;
            }
        }

        if (ioctl(fd, SIOCGIFINDEX, &ifr) == 0) {
            DEBUG_PRT(LOG_INFO, "adapter index %d", ifr.ifr_ifindex);
            *ifindex = ifr.ifr_ifindex;
        } else {
        	DEBUG_PRT(LOG_ERR, "SIOCGIFINDEX failed!: %s", strerror(errno));
            return -1;
        }
        if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) {
            memcpy(arp, ifr.ifr_hwaddr.sa_data, 6);
            DEBUG_PRT(LOG_INFO, "adapter hardware address %02x:%02x:%02x:%02x:%02x:%02x", arp[0],
                    arp[1], arp[2], arp[3], arp[4], arp[5]);
        } else {
        	DEBUG_PRT(LOG_ERR, "SIOCGIFHWADDR failed!: %s", strerror(errno));
            return -1;
        }
    } else {
    	DEBUG_PRT(LOG_ERR, "socket failed!: %s", strerror(errno));
        return -1;
    }
    close(fd);
    return 0;
}

/* args:	yiaddr - what IP to ping
 *	ip - our ip
 *	mac - our arp address
 *	interface - interface to use
 * retn: 	1 addr free
 *	0 addr used
 *	-1 error
 */

/* FIXME: match response against chaddr */
int arpping(u_int32_t yiaddr, u_int32_t ip, unsigned char *mac, char *interface)
{
    int timeout = 2;
    int optval = 1;
    int s;                                          /* socket */
    int rv = 1;                                     /* return value */
    struct sockaddr addr;                           /* for interface name */
    struct arpMsg arp;
    fd_set fdset;
    struct timeval tm;
    time_t prevTime;

    if ((s = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP))) == -1) {
    	DEBUG_PRT(LOG_ERR, "Could not open raw socket");
        return -1;
    }

    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1) {
    	DEBUG_PRT(LOG_ERR, "Could not setsocketopt on raw socket");
        close(s);
        return -1;
    }

    /* send arp request */
    memset(&arp, 0, sizeof(arp));
    memcpy(arp.ethhdr.h_dest, MAC_BCAST_ADDR, 6);   /* MAC DA */
    memcpy(arp.ethhdr.h_source, mac, 6);            /* MAC SA */
    arp.ethhdr.h_proto = htons(ETH_P_ARP);          /* protocol type (Ethernet) */
    arp.htype = htons(ARPHRD_ETHER);                /* hardware type */
    arp.ptype = htons(ETH_P_IP);                    /* protocol type (ARP message) */
    arp.hlen = 6;                                   /* hardware address length */
    arp.plen = 4;                                   /* protocol address length */
    arp.operation = htons(ARPOP_REQUEST);           /* ARP op code */
    if (is_send_arp) {
    	*((u_int *) arp.sInaddr) = 0;               /* source IP address */
    } else {
    	*((u_int *) arp.sInaddr) = ip;              /* source IP address */
    }
    memcpy(arp.sHaddr, mac, 6);                     /* source hardware address */
    *((u_int *) arp.tInaddr) = yiaddr;              /* target IP address */

    DEBUG_PRT(LOG_INFO, "arp.sInaddr=[%d.%d.%d.%d], arp.tInaddr=[%d.%d.%d.%d]", arp.sInaddr[0],
            arp.sInaddr[1], arp.sInaddr[2], arp.sInaddr[3], arp.tInaddr[0], arp.tInaddr[1],
            arp.tInaddr[2], arp.tInaddr[3]);

    memset(&addr, 0, sizeof(addr));
    strcpy(addr.sa_data, interface);
    if (sendto(s, &arp, sizeof(arp), 0, &addr, sizeof(addr)) < 0)
        rv = 0;

    /* wait arp reply, and check it */
    tm.tv_usec = 0;
    time(&prevTime);
    while (timeout > 0) {
        FD_ZERO(&fdset);
        FD_SET(s, &fdset);
        tm.tv_sec = timeout;
        if (select(s + 1, &fdset, (fd_set *) NULL, (fd_set *) NULL, &tm) < 0) {
            DEBUG_PRT(LOG_ERR, "Error on ARPING request: %s", strerror(errno));
            if (errno != EINTR)
                rv = 0;
        } else if (FD_ISSET(s, &fdset)) {
            if (recv(s, &arp, sizeof(arp), 0) < 0) {
                rv = 0;
            }
            if (arp.operation == htons(ARPOP_REPLY) && bcmp(arp.tHaddr, mac, 6) == 0
                    && *((u_int *) arp.sInaddr) == yiaddr) {
                DEBUG_PRT(LOG_INFO, "Valid arp reply receved for this address");
                printf("srcMac: %02x:%02x:%02x:%02x:%02x:%02x\ndestMac: %02x:%02x:%02x:%02x:%02x:%02x\n",
                        arp.sHaddr[0], arp.sHaddr[1], arp.sHaddr[2], arp.sHaddr[3], arp.sHaddr[4],
                        arp.sHaddr[5], arp.tHaddr[0], arp.tHaddr[1], arp.tHaddr[2], arp.tHaddr[3],
                        arp.tHaddr[4], arp.tHaddr[5]);
                rv = 0;
                break;
            }
        }
        timeout -= time(NULL) - prevTime;
        time(&prevTime);
    }
    close(s);

    DEBUG_PRT(LOG_INFO, "%salid arp replies for this address", rv ? "No v" : "V");

    return rv;
}

/* args: yiaddr - what IP to ping
 * interface - interface to use
 * return: src_ip:0.0.0.0 dst_ip:yiaddr src_mac:not me dst_mac:me
 */
int arp_listening(u_int32_t yiaddr, char *interface)
{
    int optval = 1;
    int s;                                          /* socket */
    int rv = 1;                                     /* return value */
    struct sockaddr addr;                           /* for interface name */
    struct arpMsg arp;
    fd_set fdset;
    unsigned char *mac = server_config.arp;
    u_int32_t ip = server_config.server;

    if ((s = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP))) == -1) {
    	DEBUG_PRT(LOG_ERR, "listening: Could not open raw socket");
        return -1;
    }

    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1) {
    	DEBUG_PRT(LOG_ERR, "listening: Could not setsocketopt on raw socket");
        close(s);
        return -1;
    }

    /* send arp request */
    memset(&arp, 0, sizeof(arp));
    memcpy(arp.ethhdr.h_dest, MAC_BCAST_ADDR, 6);   /* MAC DA */
    memcpy(arp.ethhdr.h_source, mac, 6);            /* MAC SA */
    arp.ethhdr.h_proto = htons(ETH_P_ARP);          /* protocol type (Ethernet) */
    arp.htype = htons(ARPHRD_ETHER);                /* hardware type */
    arp.ptype = htons(ETH_P_IP);                    /* protocol type (ARP message) */
    arp.hlen = 6;                                   /* hardware address length */
    arp.plen = 4;                                   /* protocol address length */
    arp.operation = htons(ARPOP_REQUEST);           /* ARP op code */
    *((u_int *) arp.sInaddr) = ip;                  /* source IP address */
    memcpy(arp.sHaddr, mac, 6);                     /* source hardware address */
    *((u_int *) arp.tInaddr) = yiaddr;              /* target IP address */

    DEBUG_PRT(LOG_INFO, "listening: arp.sInaddr=[%d.%d.%d.%d], arp.tInaddr=[%d.%d.%d.%d]", arp.sInaddr[0],
            arp.sInaddr[1], arp.sInaddr[2], arp.sInaddr[3], arp.tInaddr[0], arp.tInaddr[1],
            arp.tInaddr[2], arp.tInaddr[3]);

#if 1
    if (is_send_arp) {
		memset(&addr, 0, sizeof(addr));
		strcpy(addr.sa_data, interface);
		if (sendto(s, &arp, sizeof(arp), 0, &addr, sizeof(addr)) < 0)
			rv = 0;
    }
#endif

    /* wait arp reply, and check it */
    while (1) {
        FD_ZERO(&fdset);
        FD_SET(s, &fdset);
        if (select(s + 1, &fdset, (fd_set *) NULL, (fd_set *) NULL, NULL) < 0) {
            DEBUG_PRT(LOG_ERR, "Error on ARPING request: %s", strerror(errno));
            if (errno != EINTR)
                rv = 0;
        } else if (FD_ISSET(s, &fdset)) {
            if (recv(s, &arp, sizeof(arp), 0) < 0) {
                rv = 0;
            }

            if (listen_all_time) {
				printf("listening: %d (%s) src_ip:[%d.%d.%d.%d] dest_ip:[%d.%d.%d.%d] mac_src: %02x:%02x:%02x:%02x:%02x:%02x mac_dest: %02x:%02x:%02x:%02x:%02x:%02x\n",
				        arp.operation,
				        (arp.operation == htons(ARPOP_REQUEST)) ? "arp_request" :
				        (arp.operation == htons(ARPOP_REPLY)) ? "arp_reply" :
				        (arp.operation == htons(ARPOP_RREQUEST)) ? "rarp_request" : "rarp_reply",
				        arp.sInaddr[0], arp.sInaddr[1], arp.sInaddr[2], arp.sInaddr[3],
				        arp.tInaddr[0], arp.tInaddr[1], arp.tInaddr[2], arp.tInaddr[3],
				        arp.sHaddr[0], arp.sHaddr[1], arp.sHaddr[2], arp.sHaddr[3], arp.sHaddr[4],
				        arp.sHaddr[5], arp.tHaddr[0], arp.tHaddr[1], arp.tHaddr[2], arp.tHaddr[3],
				        arp.tHaddr[4], arp.tHaddr[5]);
			} else if ((((*((u_int *) arp.sInaddr) == yiaddr) && (*((u_int *) arp.tInaddr) == 0))
						|| ((*((u_int *) arp.sInaddr) == 0) && (*((u_int *) arp.tInaddr) == yiaddr)))
						&& (arp.operation == htons(ARPOP_REQUEST) || arp.operation == htons(ARPOP_REPLY)
								|| arp.operation == htons(ARPOP_RREQUEST)
								|| arp.operation == htons(ARPOP_RREPLY))) {
				if (memcmp(mac, arp.sHaddr, 6) == 0) {
					/* source mac is same with me */
					continue;
				}

				if (arp.operation == htons(ARPOP_REQUEST) && (arp.tHaddr[0] !=0 || arp.tHaddr[1] !=0 ||
						arp.tHaddr[2] !=0 || arp.tHaddr[3] !=0 ||
				        arp.tHaddr[4] !=0 || arp.tHaddr[5] !=0)) {
					/* arp request but dest mac is not 0 */
					continue;
				}

				printf("listening: %d (%s)\nsrc_ip:[%d.%d.%d.%d]\ndest_ip:[%d.%d.%d.%d]\nmac_src: %02x:%02x:%02x:%02x:%02x:%02x\nmac_dest: %02x:%02x:%02x:%02x:%02x:%02x\n\n",
				        arp.operation,
				        (arp.operation == htons(ARPOP_REQUEST)) ? "arp_request" :
				        (arp.operation == htons(ARPOP_REPLY)) ? "arp_reply" :
				        (arp.operation == htons(ARPOP_RREQUEST)) ? "rarp_request" : "rarp_reply",
				        arp.sInaddr[0], arp.sInaddr[1], arp.sInaddr[2], arp.sInaddr[3],
				        arp.tInaddr[0], arp.tInaddr[1], arp.tInaddr[2], arp.tInaddr[3],
				        arp.sHaddr[0], arp.sHaddr[1], arp.sHaddr[2], arp.sHaddr[3], arp.sHaddr[4],
				        arp.sHaddr[5], arp.tHaddr[0], arp.tHaddr[1], arp.tHaddr[2], arp.tHaddr[3],
				        arp.tHaddr[4], arp.tHaddr[5]);
				rv = 0;
				break;
			}
        }
    }
    close(s);

    return rv;
}

/* check is an IP is taken, if it is, add it to the lease table */
int check_ip(u_int32_t addr, char *interface)
{
    struct in_addr temp;

    if (arpping(addr, server_config.server, server_config.arp, interface) == 0) {
        temp.s_addr = addr;
        DEBUG_PRT(LOG_INFO, "%s belongs to someone, reserving it for %ld seconds", inet_ntoa(temp),
                server_config.conflict_time);
        return 1;
    }

    return 0;
}

int parse_option(int argc, char *argv[], char **interface)
{
	int i = 0;
	int ret = -1;

	for (i = 1; i < argc; i++) {
		if ((memcmp(argv[i], OPTION_LISTEN, strlen(OPTION_LISTEN)) == 0) &&
				(strlen(argv[i]) == strlen(OPTION_LISTEN))) {
			is_listen = 1;
			ret = 1;
		} else if ((memcmp(argv[i], OPTION_DEBUG, strlen(OPTION_DEBUG)) == 0) &&
				(strlen(argv[i]) == strlen(OPTION_DEBUG))) {
			is_debug = 1;
			ret = 1;
		} else if ((memcmp(argv[i], OPTION_LISTEN_ALL_TIME, strlen(OPTION_LISTEN_ALL_TIME)) == 0)
		        && (strlen(argv[i]) == strlen(OPTION_LISTEN_ALL_TIME))) {
			listen_all_time = 1;
			ret = 1;
		} else if ((memcmp(argv[i], OPTION_SEND_ARP, strlen(OPTION_SEND_ARP)) == 0)
		        && (strlen(argv[i]) == strlen(OPTION_SEND_ARP))) {
			is_send_arp = 1;
			ret = 1;
		} else if ((memcmp(argv[i], OPTION_INTERFACE, strlen(OPTION_INTERFACE)) == 0)
		        && (strlen(argv[i]) == strlen(OPTION_INTERFACE))) {
			if (argc >= (i + 1)) {
				*interface = argv[i + 1];
				i++;
				ret = 1;
			}
		}
	}

	return ret;
}

/* check ip format */
int check_ip_input(char *ipaddr)
{
	int ip_parm = 0;
	int i, j;
	char *p;
	p = ipaddr;
	int ret = 0;

	for (i = 0; i < 4; i++) {
		ip_parm = 0;
		for (j = 0; *p != '.' && *p != '\0'; p++, j++) {
			if (j >= 3) {
				ret = -1;
				goto end;
			}

			if (j != 0) {
				ip_parm *= 10;
			}
			if (*p < '0' || *p > '9') {
				ret = -2;
				goto end;
			}
			ip_parm += (*p - '0');
		}
		if (j == 0 || ip_parm < 0 || ip_parm > 255) {
			printf("idx:%d, jdx:%d, parm:%d\n", i, j, ip_parm);
			ret = -3;
			goto end;
		}
		if (*p == '.') {
			p++;
			if (i == 3) {
				ret = -4;
				goto end;
			}
		}
	}

	if (*p != '\0' || i != 4) {
		ret = -5;
	}

end:
	if (ret < 0) {
		printf("[%s] ip parmeter error (%d).\n", ipaddr, ret);
	}

	return ret;
}

int main(int argc, char *argv[])
{
    char *interface = NULL;

    if (argc < 2 || check_ip_input(argv[1]) < 0) {
        printf("Usage: checkip ipaddress [-i echx] [-l] [-d] [-a] [-s]\n");
		printf("-i --- network card interface.\n");
		printf("-l --- listening arp for ip conflit checking.\n");
		printf("-d --- print debug log.\n");
		printf("-a --- print arp info all the time, no return.\n");
		printf("-s --- set source ip address to 0, to check ip conflict after ip address has set;\n");
        printf("       without -s: set source ip address to local ip address, to check ip conflict before ip address has set.\n");
        exit(0);
    }

	interface = ETH_INTERFACE;

    parse_option(argc, argv, &interface);

    if (read_interface(interface, &server_config.ifindex, &server_config.server,
            server_config.arp) < 0) {
        exit(0);
    }

    if (is_listen == 1) {
    	arp_listening(inet_addr(argv[1]), interface);
		DEBUG_PRT(LOG_INFO, "IP:%s conflict", argv[1]);
		printf("0\n");
    } else {
		if (!check_ip(inet_addr(argv[1]), interface)) {
			printf("srcMac: 00:00:00:00:00:00\n");
			DEBUG_PRT(LOG_INFO, "IP:%s can use", argv[1]);
		} else {
			DEBUG_PRT(LOG_INFO, "IP:%s conflict", argv[1]);
		}
    }

    return 0;
}
