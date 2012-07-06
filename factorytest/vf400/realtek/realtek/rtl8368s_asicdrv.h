#ifndef _RTL8368S_ASICDRV_H_
#define _RTL8368S_ASICDRV_H_

#include <string.h>

#include "smi.h"
#include "rtl8368s_reg.h"
#include "rtl8368s_errno.h"

#define RTL8368S_REGBITLENGTH		        16
#define RTL8368S_VLANMCIDXMAX                   15
#define RTL8368S_FIDMAX                         7
#define RTL8368S_FIDMASK                        0xFF
#define RTL8368S_VIDMAX                         0xFFF
#define RTL8368S_PRIORITYMAX                    7
#define RTL8368S_PORTMASK                       0xFF

#define RTL8368S_PHY_NO_MAX		        (5-1)
#define RTL8368S_PHY_PAGE_MAX		        7
#define RTL8368S_PHY_REG_MAX		        31

#define RTL8368S_LED_GROUP_MAX                  4

#define RTL8368S_TBL_CMD_CHECK_COUNTER          1000

/* enum for port ID */
enum PORTID
{
  PORT0 =  0,
  PORT1,
  PORT2,
  PORT3,
  PORT4,
  PORT5,
  PORT6,	
  PORT7,	
  PORT_MAX
};

/* enum for priority value type */
enum PRIORITYVALUE
{
  PRI0 = 0,
  PRI1,
  PRI2,
  PRI3,
  PRI4,
  PRI5,
  PRI6,
  PRI7,
  PRI_MAX,
};

/* enum for port current link speed */
enum PORTLINKSPEED
{
  SPD_10M = 0,
  SPD_100M,
  SPD_1000M
};

/* enum for port current link duplex mode */
enum PORTLINKDUPLEXMODE
{
  HALF_DUPLEX = 0,
  FULL_DUPLEX
};

typedef struct VLANCONFIG
{
#ifdef _LITTLE_ENDIAN
  uint16_t  vid:12;
  uint16_t  priority:3;
  uint16_t  reserved1:1;

  uint16_t  member:8;
  uint16_t  untag:8;

  uint16_t  fid:3;
  uint16_t  reserved2:2;
  uint16_t  stag_idx:3;
  uint16_t  stag_mbr:8;
#else
  uint16_t  reserved2:1;
  uint16_t  priority:3;
  uint16_t  vid:12;

  uint16_t  untag:8;
  uint16_t  member:8;

  uint16_t  stag_mbr:8;
  uint16_t  stag_idx:3;
  uint16_t  reserved1:2;
  uint16_t  fid:3;
#endif
} rtl8368s_vlanconfig;

typedef struct VLANTABLE
{
#ifdef _LITTLE_ENDIAN
  uint16_t  vid:12;
  uint16_t  reserved1:4;

  uint16_t  member:8;
  uint16_t  untag:8;

  uint16_t  fid:3;
  uint16_t  reserved2:13;
#else
  uint16_t  reserved1:4;
  uint16_t  vid:12;

  uint16_t  untag:8;
  uint16_t  member:8;

  uint16_t  reserved2:13;
  uint16_t  fid:3;
#endif
} rtl8368s_vlan4kentry;

typedef struct USER_VLANTABLE
{
  uint16_t  vid:12;
  uint16_t  member:8;
  uint16_t  untag:8;
  uint16_t  fid:3;
} rtl8368s_user_vlan4kentry;

enum RTL8368S_LEDCONF
{
  LEDCONF_LEDOFF = 0,
  LEDCONF_DUPCOL,
  LEDCONF_LINK_ACT,
  LEDCONF_SPD1000,
  LEDCONF_SPD100,
  LEDCONF_SPD10,
  LEDCONF_SPD1000ACT,
  LEDCONF_SPD100ACT,
  LEDCONF_SPD10ACT,
  LEDCONF_SPD10010ACT,
  LEDCONF_FIBER,
  LEDCONF_FAULT,
  LEDCONF_LINKRX,
  LEDCONF_LINKTX,
  LEDCONF_MASTER,
  LEDCONF_LEDFORCE,
};

enum RTL8368S_LEDBLINKRATE
{
  LEDBLINKRATE_28MS = 0,
  LEDBLINKRATE_56MS,		
  LEDBLINKRATE_84MS,
  LEDBLINKRATE_111MS,
  LEDBLINKRATE_222MS,
  LEDBLINKRATE_446MS,
  LEDBLINKRATE_MAX,
};


int32_t rtl8368s_set_asic_reg_bit(uint32_t reg, uint32_t bit, uint32_t value);
int32_t rtl8368s_set_asic_reg_bits(uint32_t reg, uint32_t bits, uint32_t value);

int32_t rtl8368s_get_asic_reg(uint32_t reg, uint32_t *val);
int32_t rtl8368s_set_asic_reg(uint32_t reg, uint32_t value);

int32_t rtl8368s_get_asic_port_link_state(enum PORTID port, enum PORTLINKSPEED *speed, 
                                          enum PORTLINKDUPLEXMODE *duplex, uint32_t *link,
                                          uint32_t *txPause, uint32_t *rxPause, 
                                          uint32_t *nWay);

int32_t rtl8368s_set_asic_vlan(uint32_t enabled);
int32_t rtl8368s_get_asic_vlan(uint32_t *enabled);

int32_t rtl8368s_set_asic_vlan_4k_tb_usage(uint32_t enabled);
int32_t rtl8368s_get_asic_vlan_4k_tb_usage(uint32_t *enabled);

int32_t rtl8368s_set_asic_vlan_member_config(uint32_t index, rtl8368s_vlanconfig vlanmconf);
int32_t rtl8368s_get_asic_vlan_member_config(uint32_t index, rtl8368s_vlanconfig *vlanmconf);

int32_t rtl8368s_set_asic_vlan_4k_entry(rtl8368s_user_vlan4kentry vlan4kEntry);
int32_t rtl8368s_get_asic_vlan_4k_entry(rtl8368s_user_vlan4kentry *vlan4kEntry);

int32_t rtl8368s_set_asic_vlan_accept_tagged_only(enum PORTID port, uint32_t enabled);
int32_t rtl8368s_get_asic_vlan_accept_tagged_only(enum PORTID port, uint32_t* enabled);

int32_t rtl8368s_set_asic_vlan_drop_tagged_packets(enum PORTID port, uint32_t enabled);
int32_t rtl8368s_set_asic_vlan_ingress_filtering(enum PORTID port, uint32_t enabled);

int32_t rtl8368s_set_asic_vlan_port_based_vid(enum PORTID port, uint32_t index);
int32_t rtl8368s_get_asic_vlan_port_based_vid(enum PORTID port, uint32_t* index);

int32_t rtl8368s_set_asic_vlan_keep_ctag_format(enum PORTID ingressport, uint32_t portmask);
int32_t rtl8368s_get_asic_vlan_keep_ctag_format(enum PORTID ingressport, uint32_t *portmask);

int32_t rtl8368s_set_asic_phy_regs(uint32_t phyNo, uint32_t page, 
                                   uint32_t addr, uint32_t data);
int32_t rtl8368s_get_asic_phy_regs(uint32_t phyNo, uint32_t page, 
                                   uint32_t addr, uint32_t *data);

int32_t rtl8368s_set_asic_port_learn_disable(enum PORTID port, uint32_t disable);
int32_t rtl8368s_get_asic_port_learn_disable(enum PORTID port, uint32_t* disable);

int32_t rtl8368s_set_asic_port_priority(enum PORTID port, enum PRIORITYVALUE priority);
int32_t rtl8368s_set_asic_dot1q_absolutely_priority(enum PRIORITYVALUE srcpriority, 
                                                    enum PRIORITYVALUE priority);
int32_t rtl8368s_get_asic_dot1q_absolutely_priority(enum PRIORITYVALUE srcpriority, 
                                                    enum PRIORITYVALUE *pPriority);

int32_t rtl8368s_set_asic_led_indicate_info_config(uint32_t ledNo, 
                                                   enum RTL8368S_LEDCONF config);
int32_t rtl8368s_get_asic_led_indicate_info_config(uint32_t ledNo, 
                                                   enum RTL8368S_LEDCONF *config);
int32_t rtl8368s_set_asic_force_leds(uint32_t ledG0Msk, uint32_t ledG1Msk,
                                     uint32_t ledG2Msk, uint32_t ledG3Msk);

int32_t rtl8368s_set_asic_led_blink_rate(enum RTL8368S_LEDBLINKRATE blinkRate);
int32_t rtl8368s_get_asic_led_blink_rate(enum RTL8368S_LEDBLINKRATE *blinkRate);

#endif /*#ifndef _RTL8368S_ASICDRV_H_*/

/*
 * ex: ts=8 sw=2 sts=2 et
 */

