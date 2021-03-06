

#include "./rtl8366rb_api.h"
#include "./rtk_api.h"
#include "./rtk_api_ext.h"
#include "./rtl8367b_asicdrv_port.h"
#include "./realtek_wrap_func.h"

#include <unistd.h>
#include <fcntl.h>

//#define dbgprint console_print
#define dbgprint(x,y...)

//RTL8367R Check macro
#define IS_RTL8367R     access("/etc/.rtl8367r", F_OK)
#define IS_RTL8366RB   access("/etc/.rtl8366rb", F_OK)
#define IS_OK         0

extern void console_print(const char *fmt, ...);

ret_t rtl8367b_getAsicReg(rtk_uint32 reg, rtk_uint32 *pValue);
int32_t rtl8368s_get_asic_reg(uint32_t reg, uint32_t *value);

int get_realtek_switch_partno(void)
{
  if (IS_RTL8367R == IS_OK)
    return SWITCH_RTL8367R;
  else
    return SWITCH_RTL8366RB;
}

void rtl8367_RGMII_set(void)
{
  rtk_mode_ext_t mode0=0, mode1=0;
  rtk_port_mac_ability_t pPortability0, pPortability1;
  int ret;

	printf("rtl8367_RGMII_set\r\n");

  //Driver Bug : dummy read
  rtl8367b_getAsicReg(0, (uint32_t *)&ret);

  //Set the Ports on RGMII mode0
  memset(&pPortability0, 0, sizeof(rtk_port_mac_ability_t));
  mode0=MODE_EXT_RGMII;
  pPortability0.forcemode=MAC_FORCE;
	pPortability0.speed=SPD_1000M;
  pPortability0.duplex=FULL_DUPLEX;
  pPortability0.link=PORT_LINKUP;
  pPortability0.nway=DISABLED;
  pPortability0.txpause=ENABLED;
  pPortability0.rxpause=ENABLED;

  ret=rtk_port_macForceLinkExt0_set(mode0, &pPortability0);
  ret=rtk_port_macForceLinkExt1_set(mode0, &pPortability0);

  if(ret!=RT_ERR_OK){
    dbgprint("Fail to set the RTL8367 port !(%d)\n\r", ret);
  }
  else {
    memset(&pPortability0, 0, sizeof(rtk_port_mac_ability_t));
    mode0=mode1=0;
    ret=rtk_port_macForceLinkExt0_get(&mode0, &pPortability0);
    ret=rtk_port_macForceLinkExt1_get(&mode1, &pPortability1);
    if(ret==RT_ERR_OK)
      dbgprint("Complete to set the RTL8367 on RGMII P0&P1(m=%d|%d,f=%d|%d,s=%d|%d,l=%d|%d)\n\r",
        mode0, mode1, pPortability0.forcemode, pPortability1.forcemode,
        pPortability0.speed, pPortability1.speed, pPortability0.link, pPortability1.link);
  }

}

// realtek_power_on fucntion. by jong2ry. 2012.03.21
void realtek_power_on(void)
{
  if (IS_RTL8367R == IS_OK){
    rtl8367r_power_on();
  }
  else{
    rtl8366rb_power_on();
  }
}


// realtek_power_down funcgion. by jong2ry. 2012.03.26
void realtek_power_down(void)
{
  if (IS_RTL8367R == IS_OK) {
    rtl8367r_power_down();
  }
  else{
    rtl8366rb_power_down();
  }
}


// Initialize RealTek Chip ( Support RTL8367R & RTL8366RB)
// Set Global Var 'realtek_switch_partno' here
// Automatically detect switch IC .... RTL8367 or RTL8366
int realtek_init_chip(int model)
{
  int ret=0;
  FILE *chkrtl = NULL;

  //Detect which switch is used
  //dummy read
  ret=rtl8368s_get_asic_reg(0, (uint32_t *)&ret);
  ret=rtl8368s_get_asic_reg(0, (uint32_t *)&ret);

  // when switch chip is rtl8366rb, create "rtl8366rb" flag file. by jong2ry. 2012.03.26
  if (chkrtl = fopen("/etc/.rtl8366rb", "w"))
    fclose(chkrtl);

  // when switch chip is rtl8366rb, delete  "rtl8367r" flag file. by jong2ry. 2012.03.26
  if (IS_RTL8367R == IS_OK)
    unlink("/etc/.rtl8367r");

  if(ret != 0){
    //dummy read
    ret = rtl8367b_getAsicReg(0, (uint32_t *)&ret);
    ret = rtl8367b_getAsicReg(0, (uint32_t *)&ret);

    // when switch chip is rtl8367r, create "rtl8367r" flag file. by jong2ry. 2012.03.26
    if (chkrtl = fopen("/etc/.rtl8367r", "w"))
      fclose(chkrtl);

    // when switch chip is rtl8367r, delete  "rtl8366rb" flag file. by jong2ry. 2012.03.26
    if (IS_RTL8366RB == IS_OK)
      unlink("/etc/.rtl8366rb");
  }

  //dbgprint("Detected Switch IC RTL%d\n\r", realtek_switch_partno);

  if (IS_RTL8367R == IS_OK)
  {
    //rtk_switch_init()은 Vlan을 초기화 못함
    rtk_vlan_init();

    //switch mode 초기화
    ret = rtk_switch_init();
    //RGMII 모드 사용됨
    rtl8367_RGMII_set();
    dbgprint("###ERCYI : RTL8367 Init (RET=%d) board_type=%d\n\r", ret, model);
  }
  else
  {

    ret = rtl8366rb_init_chip();

    dbgprint("###ERCYI : RTL8366 Init (RET=%d) board_type=%d\n\r", ret, model);
  }

  return ret;
}

int32_t realtek_get_vlan(rtl8366rb_vlanConfig_t *ptr_vlancfg)
{
  int ret;
  rtk_vlan_t vid;
  rtk_portmask_t mbrmsk;
  rtk_portmask_t untagmsk;
  rtk_fid_t fid;

  dbgprint("%s:%d\n\r" , __func__, __LINE__);

  if (IS_RTL8367R == IS_OK)
  {
    vid               = ptr_vlancfg->vid;
    mbrmsk.bits[0]    = ptr_vlancfg->mbrmsk;
    untagmsk.bits[0]  = ptr_vlancfg->untagmsk;
    fid               = ptr_vlancfg->fid;

    ret=rtk_vlan_get( vid, &mbrmsk, &untagmsk, &fid);

    ptr_vlancfg->vid      =vid;
    ptr_vlancfg->mbrmsk   =mbrmsk.bits[0];
    ptr_vlancfg->untagmsk =untagmsk.bits[0];
    ptr_vlancfg->fid      =fid;

  }
  else
  {
    ret=rtl8366rb_get_vlan(ptr_vlancfg);
  }

  return ret;
}

int32_t realtek_set_vlan(rtl8366rb_vlanConfig_t *ptr_vlancfg)
{
  int ret;
  rtk_vlan_t vid;
  rtk_portmask_t mbrmsk;
  rtk_portmask_t untagmsk;
  rtk_fid_t fid;

  dbgprint("%s:%d\n\r" , __func__, __LINE__);

  if (IS_RTL8367R == IS_OK)
  {
    vid               = ptr_vlancfg->vid;
    mbrmsk.bits[0]    = ptr_vlancfg->mbrmsk;
    untagmsk.bits[0]  = ptr_vlancfg->untagmsk;
    fid               = ptr_vlancfg->fid;

    ret=rtk_vlan_set(vid, mbrmsk, untagmsk, fid);

    ptr_vlancfg->vid      =vid;
    ptr_vlancfg->mbrmsk   =mbrmsk.bits[0];
    ptr_vlancfg->untagmsk =untagmsk.bits[0];
    ptr_vlancfg->fid      =fid;
  }
  else
  {
    ret=rtl8366rb_set_vlan(ptr_vlancfg);
  }

  dbgprint("vid=0x%x, mb=0x%x, ut=0x%x, fid=0x%x\n\r",
    ptr_vlancfg->vid, ptr_vlancfg->mbrmsk, ptr_vlancfg->untagmsk, ptr_vlancfg->fid);

  return ret;
}

int32_t realtek_set_vlan_pvid(uint32_t port, uint32_t vid, uint32_t priority)
{
  int ret;

  dbgprint("%s:%d\n\r" , __func__, __LINE__);

  if (IS_RTL8367R == IS_OK)
    ret=rtk_vlan_portPvid_set(port, vid, priority);
  else
    ret=rtl8366rb_set_vlan_pvid(port, vid, priority);

  dbgprint("port=0x%x, vid=0x%x, pri=0x%x\n\r", port, vid, priority);

  return ret;
}

int32_t realtek_init_vlan(uint32_t flag)
{
  int ret;

  dbgprint("%s:%d\n\r" , __func__, __LINE__);

  if (IS_RTL8367R == IS_OK)
    ret=rtk_vlan_init();
  else
    ret=rtl8366rb_init_vlan(flag);

  return ret;
}

int32_t rtk_set_1QMapping_priority(rtl8366rb_dot1qPriority_t *ptr_1qpriority)
{
  if(0 != rtk_qos_1pPriRemap_set(PRI0, ptr_1qpriority->dot1qPri0))
    return -1;

  if(0 != rtk_qos_1pPriRemap_set(PRI1, ptr_1qpriority->dot1qPri1))
    return -1;

  if(0 != rtk_qos_1pPriRemap_set(PRI2, ptr_1qpriority->dot1qPri2))
    return -1;

  if(0 != rtk_qos_1pPriRemap_set(PRI3, ptr_1qpriority->dot1qPri3))
    return -1;

  if(0 != rtk_qos_1pPriRemap_set(PRI4, ptr_1qpriority->dot1qPri4))
    return -1;

  if(0 != rtk_qos_1pPriRemap_set(PRI5, ptr_1qpriority->dot1qPri5))
    return -1;

  if(0 != rtk_qos_1pPriRemap_set(PRI6, ptr_1qpriority->dot1qPri6))
    return -1;

  if(0 != rtk_qos_1pPriRemap_set(PRI7, ptr_1qpriority->dot1qPri7))
    return -1;

  return 0;
}

int32_t realtek_set_1QMapping_priority(rtl8366rb_dot1qPriority_t *ptr_1qpriority)
{
  int ret;

  dbgprint("%s:%d\n\r" , __func__, __LINE__);

  if (IS_RTL8367R == IS_OK)
    ret = rtk_set_1QMapping_priority(ptr_1qpriority);
  else
    ret = rtl8366rb_set_1QMapping_priority(ptr_1qpriority);

  return ret;
}

int32_t realtek_set_source_mac_learning(uint32_t port, uint32_t enabled)
{
  int ret;

  dbgprint("%s:%d\n\r" , __func__, __LINE__);

  if (IS_RTL8367R == IS_OK)
    ret=rtk_l2_limitLearningCnt_set(port, (enabled ? RTK_MAX_NUM_OF_LEARN_LIMIT : 0) );
  else
    ret=rtl8366rb_set_source_mac_learning(port, enabled);

  return ret;
}

int32_t realtek_clear_vlan(void)
{
  int ret;

  dbgprint("%s:%d\n\r" , __func__, __LINE__);

  if (IS_RTL8367R == IS_OK)
    ret=rtk_vlan_init();
  else
    ret=rtl8366rb_clear_vlan();

  return ret;
}

int32_t realtek_get_ethernet_phy(uint32_t phy, rtl8366rb_phyAbility_t *ptr_ability)
{
  int ret;
  rtk_port_phy_ability_t ability;

  dbgprint("%s:%d\n\r" , __func__, __LINE__);

  if (IS_RTL8367R == IS_OK)
  {
    ability.AutoNegotiation = ptr_ability->AutoNegotiation;
    ability.Half_10         = ptr_ability->Half_10;
    ability.Full_10         = ptr_ability->Full_10;
    ability.Half_100        = ptr_ability->Half_100;
    ability.Full_100        = ptr_ability->Full_100;
    ability.Full_1000       = ptr_ability->Full_1000;
    ability.FC              = ptr_ability->FC;
    ability.AsyFC           = ptr_ability->AsyFC;

    ret=rtk_port_phyAutoNegoAbility_get(phy, &ability);

    ptr_ability->AutoNegotiation  = ability.AutoNegotiation;
    ptr_ability->Half_10          = ability.Half_10;
    ptr_ability->Full_10          = ability.Full_10;
    ptr_ability->Half_100         = ability.Half_100;
    ptr_ability->Full_100         = ability.Full_100;
    ptr_ability->Full_1000        = ability.Full_1000;
    ptr_ability->FC               = ability.FC;
    ptr_ability->AsyFC            = ability.AsyFC;
  }
  else
  {
    ret=rtl8366rb_get_ethernet_phy(phy, ptr_ability);
  }

  return ret;
}

int32_t realtek_set_ethernet_phy(uint32_t phy, rtl8366rb_phyAbility_t *ptr_ability)
{
  int ret;
  rtk_port_phy_ability_t ability;

	printf("### realtek_set_ethernet_phy\n");
  dbgprint("%s:%d\n\r" , __func__, __LINE__);

  if (IS_RTL8367R == IS_OK)
  {
    ability.AutoNegotiation = ptr_ability->AutoNegotiation;
    ability.Half_10         = ptr_ability->Half_10;
    ability.Full_10         = ptr_ability->Full_10;
    ability.Half_100        = ptr_ability->Half_100;
    ability.Full_100        = ptr_ability->Full_100;
    ability.Full_1000       = ptr_ability->Full_1000;
    ability.FC              = ptr_ability->FC;
    ability.AsyFC           = ptr_ability->AsyFC;

    ret = rtk_port_phyAutoNegoAbility_set(phy, &ability);

    ptr_ability->AutoNegotiation  = ability.AutoNegotiation;
    ptr_ability->Half_10          = ability.Half_10;
    ptr_ability->Full_10          = ability.Full_10;
    ptr_ability->Half_100         = ability.Half_100;
    ptr_ability->Full_100         = ability.Full_100;
    ptr_ability->Full_1000        = ability.Full_1000;
    ptr_ability->FC               = ability.FC;
    ptr_ability->AsyFC            = ability.AsyFC;
  }
  else
  {
    ret = rtl8366rb_set_ethernet_phy(phy, ptr_ability);
  }

  return ret;
}


int32_t realtek_get_asic_port_link_state(enum PORTID port, enum PORTLINKSPEED *speed,
                                          enum PORTLINKDUPLEXMODE *duplex, uint32_t *link,
                                          uint32_t *txPause, uint32_t *rxPause,
                                          uint32_t *nWay)
{
  int ret;
  rtk_port_mac_ability_t portstatus;

  dbgprint("%s:%d\n\r" , __func__, __LINE__);

  if (IS_RTL8367R == IS_OK)
  {
    portstatus.speed      = *speed;
    portstatus.duplex     = *duplex;
    portstatus.link       = *link;
    portstatus.txpause    = *txPause;
    portstatus.rxpause    = *rxPause;
    portstatus.nway       = *nWay;

    ret=rtk_port_macStatus_get(port, &portstatus);

    *speed    = portstatus.speed;
    *duplex   = portstatus.duplex;
    *link     = portstatus.link;
    *txPause  = portstatus.txpause;
    *rxPause  = portstatus.rxpause;
    *nWay     = portstatus.nway;
  }
  else
  {
    ret=rtl8368s_get_asic_port_link_state(port, speed, duplex, link, txPause, rxPause, nWay);
  }

  return ret;
}



