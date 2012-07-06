

void rtl8367_RGMII_set(void);
// Initialize RealTek Chip ( Support RTL8367R & RTL8366RB)
// Set Global Var 'realtek_switch_partno' here
int realtek_init_chip(int model);
void realtek_power_on();       // add power on function by jong2ry. 2012.03.21
void realtek_power_down();     // add power down function by jong2ry. 2012.03.26
int32_t realtek_get_vlan(rtl8366rb_vlanConfig_t *ptr_vlancfg);
int32_t realtek_set_vlan(rtl8366rb_vlanConfig_t *ptr_vlancfg);
int32_t realtek_set_vlan_pvid(uint32_t port, uint32_t vid, uint32_t priority);
int32_t realtek_init_vlan(uint32_t flag);
int32_t rtk_set_1QMapping_priority(rtl8366rb_dot1qPriority_t *ptr_1qpriority);
int32_t realtek_set_1QMapping_priority(rtl8366rb_dot1qPriority_t *ptr_1qpriority);
int32_t realtek_set_source_mac_learning(uint32_t port, uint32_t enabled);
int32_t realtek_clear_vlan(void);
int32_t realtek_get_ethernet_phy(uint32_t phy, rtl8366rb_phyAbility_t *ptr_ability);
int32_t realtek_set_ethernet_phy(uint32_t phy, rtl8366rb_phyAbility_t *ptr_ability);
int32_t realtek_get_asic_port_link_state(enum PORTID port, enum PORTLINKSPEED *speed,
                                          enum PORTLINKDUPLEXMODE *duplex, uint32_t *link,
                                          uint32_t *txPause, uint32_t *rxPause,
                                          uint32_t *nWay);
int get_realtek_switch_partno(void);

#define SWITCH_RTL8366RB	8366
#define SWITCH_RTL8367R		8367



