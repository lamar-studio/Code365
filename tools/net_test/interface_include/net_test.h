#ifndef _NET_TEST_H_
#define _NET_TEST_H_

#include "base.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define ETH_NAME_LEN     (16)

#define ETH_STATUS_UP    (0)
#define ETH_STATUS_DOWN  (1)

typedef struct tagNetInfo {

	uint8_t mac[MAC_ADDR_LEN];
	uint8_t eth_name[ETH_NAME_LEN];
	uint32_t eth_index;
	uint32_t eth_status;
	uint32_t eth_link;
    uint32_t eth_speed;
    uint8_t eth_duplex;
	uint32 seq;
	uint32 recv_num;
} NET_INFO_T;

/* The forced speed, 10Mb, 100Mb, gigabit, 10GbE. */
#define SPEED_10		10
#define SPEED_100		100
#define SPEED_1000		1000
#define SPEED_10000		10000

/* Duplex, half or full. */
#define DUPLEX_HALF		0x00
#define DUPLEX_FULL		0x01

extern int net_test_init();
extern int net_test_all();
extern int net_get_mac_addr(uint8_t *mac, uint8 *hw_buf);


#ifdef __cplusplus
}
#endif

#endif
