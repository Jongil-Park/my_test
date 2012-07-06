#ifndef _RTL8366RB_API_H_
#define _RTL8366RB_API_H_

#include "rtl8368s_asicdrv.h"

#define ENABLE                                  1
#define DISABLE                                 0

#ifndef NULL
#define NULL                                    0
#endif

#ifndef TRUE
#define TRUE                                    1
#endif

#ifndef FALSE
#define FALSE                                   0
#endif

#define RTL8366RB_PHY_CONTROL_REG               0
#define RTL8366RB_PHY_STATUS_REG                1
#define RTL8366RB_PHY_AN_ADVERTISEMENT_REG      4
#define RTL8366RB_PHY_1000_BASET_CONTROL_REG    9

#define RTL8366RB_PHY_NO_MAX                    4

#define RTL8366RB_LED_GROUP_MAX                 4

#define RTL8366RB_VLANMCIDXMAX                  15
#define RTL8366RB_FIDMAX                        7
#define RTL8366RB_VIDMAX                        0xFFF
#define RTL8366RB_PRIORITYMAX                   7
#define RTL8366RB_PORTMASK                      0x3F

#define RTL8366RB_PORT_MAX                      6


typedef enum RTL8366RB_FRAMETYPE
{
  RTL8366RB_FRAME_TYPE_ALL = 0,
  RTL8366RB_FRAME_TYPE_CTAG,
  RTL8366RB_FRAME_TYPE_UNTAG,
} rtl8366rb_accept_frame_t;

typedef enum RTL8366RB_LEDBLINKRATE
{
  RTL8366RB_LEDBLINKRATE_28MS=0,
  RTL8366RB_LEDBLINKRATE_56MS,
  RTL8366RB_LEDBLINKRATE_84MS,
  RTL8366RB_LEDBLINKRATE_111MS,
  RTL8366RB_LEDBLINKRATE_222MS,
  RTL8366RB_LEDBLINKRATE_446MS,
  RTL8366RB_LEDBLINKRATE_MAX,
} rtl8366rb_led_blinkrate_t;

typedef struct rtl8366rb_phyAbility_s
{
  uint16_t AutoNegotiation:1;/*PHY register 0.12 setting for auto-negotiation process*/
  uint16_t Half_10:1;        /*PHY register 4.5 setting for 10BASE-TX half duplex capable*/
  uint16_t Full_10:1;        /*PHY register 4.6 setting for 10BASE-TX full duplex capable*/
  uint16_t Half_100:1;       /*PHY register 4.7 setting for 100BASE-TX half duplex capable*/
  uint16_t Full_100:1;       /*PHY register 4.8 setting for 100BASE-TX full duplex capable*/
  uint16_t Full_1000:1;      /*PHY register 9.9 setting for 1000BASE-T full duplex capable*/
  uint16_t FC:1;             /*PHY register 4.10 setting for flow control capability*/
  uint16_t AsyFC:1;          /*PHY register 4.11 setting for  asymmetric flow control capability*/
} rtl8366rb_phyAbility_t;

typedef struct rtl8366rb_portPriority_s
{
  enum PRIORITYVALUE priPort0;
  enum PRIORITYVALUE priPort1;
  enum PRIORITYVALUE priPort2;
  enum PRIORITYVALUE priPort3;
  enum PRIORITYVALUE priPort4;
  enum PRIORITYVALUE priPort5;
} rtl8366rb_portPriority_t;

typedef struct rtl8366rb_dot1qPriority_s
{
  enum PRIORITYVALUE dot1qPri0;
  enum PRIORITYVALUE dot1qPri1;
  enum PRIORITYVALUE dot1qPri2;
  enum PRIORITYVALUE dot1qPri3;
  enum PRIORITYVALUE dot1qPri4;
  enum PRIORITYVALUE dot1qPri5;
  enum PRIORITYVALUE dot1qPri6;
  enum PRIORITYVALUE dot1qPri7;
} rtl8366rb_dot1qPriority_t;


typedef struct
{
  uint32_t vid;
  uint32_t mbrmsk;
  uint32_t untagmsk;
  uint32_t fid;
} rtl8366rb_vlanConfig_t;


int32_t rtl8366rb_set_port_priority(rtl8366rb_portPriority_t *ptr_portpriority);
int32_t rtl8366rb_set_1QMapping_priority(rtl8366rb_dot1qPriority_t *ptr_1qpriority);

int32_t rtl8366rb_set_ethernet_phy(uint32_t phy, rtl8366rb_phyAbility_t *ptr_ability);
int32_t rtl8366rb_get_ethernet_phy(uint32_t phy, rtl8366rb_phyAbility_t *ptr_ability);
int32_t rtl8366rb_get_phy_link_status(uint32_t phy, uint32_t *ptr_linkStatus);
int32_t rtl8366rb_set_led_blink_rate(rtl8366rb_led_blinkrate_t blinkRate);

int32_t rtl8366rb_init_vlan(uint32_t flag);
int32_t rtl8366rb_clear_vlan(void);
int32_t rtl8366rb_set_vlan(rtl8366rb_vlanConfig_t *ptr_vlancfg);
int32_t rtl8366rb_get_vlan(rtl8366rb_vlanConfig_t *ptr_vlancfg);
int32_t rtl8366rb_set_vlan_pvid(uint32_t port, uint32_t vid, uint32_t priority);
int32_t rtl8366rb_get_vlan_pvid(uint32_t port, uint32_t *ptr_vid, uint32_t *ptr_priority);
int32_t rtl8366rb_set_vlan_ingress_filter(uint32_t port, uint32_t enabled);
int32_t rtl8366rb_set_vlan_accept_frame_type(uint32_t port, rtl8366rb_accept_frame_t type);
int32_t rtl8366rb_set_vlan_keep_ctag_format(uint32_t ingressport, uint32_t portmask);
int32_t rtl8366rb_set_source_mac_learning(uint32_t port, uint32_t enabled);
int32_t rtl8366rb_init_chip(void);
void rtl8366rb_power_on(void);   // Power on rtl2266 chip. by jong2ry 2012.03.20
void rtl8366rb_power_down(void);   // Power down rtl2266 chip. by jong2ry 2012.03.26


#endif /*#ifndef _RTL8366RB_API_H_*/

/*
 * ex: ts=8 sw=2 sts=2 et
 */

