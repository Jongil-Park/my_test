/*
* Copyright c                  Realtek Semiconductor Corporation, 2008
* All rights reserved.
*
* Program : RTL8366RB switch high-level API
* Abstract :
* Author : H.M Chung (hmchung@realtek.com.tw), Kobe Wu (kobe_wu@realtek.com.tw)
* $Id: rtl8366rb_api.c,v 1.18 2008-06-25 06:22:44 kobe_wu Exp $
*/
/*
   @doc rtl8366rb_api
   @module rtl8366rb_api.c - RTL8366RB switch high-level API documentation	|
   This document explains high-level API interface of the asic.
   @normal
   Copyright <cp>2008 Realtek<tm> Semiconductor Cooperation, All Rights Reserved.
   @head3 List of Symbols | Here is a list of all functions and variables in this module.
   @index | rtl8366rb_api
*/

#include "rtl8366rb_api.h"

/*
   @func int32_t | rtl8366rb_set_port_priority | Configure priority usage to each port.
   @parm rtl8366rb_portPriority_t *| ptr_portpriority | Priorities assigment for each port.
   @rvalue 0 | Success.
   @rvalue -1 | Failure.
   @comm
   Priorityof ports assigment for queue usage and packet scheduling.
*/
int32_t rtl8366rb_set_port_priority(rtl8366rb_portPriority_t *ptr_portpriority)
{
  if (0 != rtl8368s_set_asic_port_priority(0, ptr_portpriority->priPort0))
    return -1;

  if (0 != rtl8368s_set_asic_port_priority(1, ptr_portpriority->priPort1))
    return -1;

  if (0 != rtl8368s_set_asic_port_priority(2, ptr_portpriority->priPort2))
    return -1;

  if (0 != rtl8368s_set_asic_port_priority(3, ptr_portpriority->priPort3))
    return -1;

  if (0 != rtl8368s_set_asic_port_priority(4, ptr_portpriority->priPort4))
    return -1;

  if (0 != rtl8368s_set_asic_port_priority(5, ptr_portpriority->priPort5))
    return -1;

  return 0;
}

/*
   @func int32 | rtl8366rb_set1QMappingPriority | Configure 1Q priority mapping asic internal absolute priority.
   @parm dot1qPriority_t *| ptr_1qpriority | Priorities assigment for receiving 802.1Q prioirty.
   @rvalue 0 | Success.
   @rvalue -1 | Failure.
   @comm
   Priorityof 802.1Q assigment for queue usage and packet scheduling.
*/
int32_t rtl8366rb_set_1QMapping_priority(rtl8366rb_dot1qPriority_t *ptr_1qpriority)
{
  if(0 != rtl8368s_set_asic_dot1q_absolutely_priority(PRI0, ptr_1qpriority->dot1qPri0))
    return -1;

  if(0 != rtl8368s_set_asic_dot1q_absolutely_priority(PRI1, ptr_1qpriority->dot1qPri1))
    return -1;

  if(0 != rtl8368s_set_asic_dot1q_absolutely_priority(PRI2, ptr_1qpriority->dot1qPri2))
    return -1;

  if(0 != rtl8368s_set_asic_dot1q_absolutely_priority(PRI3, ptr_1qpriority->dot1qPri3))
    return -1;

  if(0 != rtl8368s_set_asic_dot1q_absolutely_priority(PRI4, ptr_1qpriority->dot1qPri4))
    return -1;

  if(0 != rtl8368s_set_asic_dot1q_absolutely_priority(PRI5, ptr_1qpriority->dot1qPri5))
    return -1;

  if(0 != rtl8368s_set_asic_dot1q_absolutely_priority(PRI6, ptr_1qpriority->dot1qPri6))
    return -1;

  if(0 != rtl8368s_set_asic_dot1q_absolutely_priority(PRI7, ptr_1qpriority->dot1qPri7))
    return -1;

  return 0;
}

/*
   @func int32_t | rtl8366rb_set_ethernet_phy | Set ethernet PHY registers for desired ability.
   @parm uint32_t | phy | PHY number (0~4).
   @parm phyAbility_t | ability | Ability structure
   @rvalue 0 | Success.
   @rvalue -1 | Failure.
   @comm
   If Full_1000 bit is set to 1, the AutoNegotiation will be automatic
   set to 1. While both AutoNegotiation and Full_1000 are set to 0, the
   PHY speed and duplex selection will be set as following 100F > 100H
   > 10F > 10H priority sequence.
*/
int32_t rtl8366rb_set_ethernet_phy(uint32_t phy, rtl8366rb_phyAbility_t *ptr_ability)
{
  uint32_t phyData;

  uint32_t phyEnMsk0;
  uint32_t phyEnMsk4;
  uint32_t phyEnMsk9;

  if (phy > RTL8366RB_PHY_NO_MAX)
    return -1;

  phyEnMsk0 = 0;
  phyEnMsk4 = 0;
  phyEnMsk9 = 0;

  /* 10BASE-TX half duplex capable in reg 4.5 */
  if (1 == ptr_ability->Half_10)
    {
      phyEnMsk4 = phyEnMsk4 | (1<<5);
      /* Speed selection [1:0] : 11=Reserved, 10=1000Mpbs, 01=100Mpbs, 00=10Mpbs */
      phyEnMsk0 = phyEnMsk0 & (~(1<<6));
      phyEnMsk0 = phyEnMsk0 & (~(1<<13));
    }

  /* 10BASE-TX full duplex capable in reg 4.6 */
  if (1 == ptr_ability->Full_10)
    {
      phyEnMsk4 = phyEnMsk4 | (1<<6);
      /* Speed selection [1:0] : 11=Reserved, 10=1000Mpbs, 01=100Mpbs, 00=10Mpbs */
      phyEnMsk0 = phyEnMsk0 & (~(1<<6));
      phyEnMsk0 = phyEnMsk0 & (~(1<<13));
      /* Full duplex mode in reg 0.8 */
      phyEnMsk0 = phyEnMsk0 | (1<<8);
    }

  /* 100BASE-TX half duplex capable in reg 4.7 */
  if (1 == ptr_ability->Half_100)
    {
      phyEnMsk4 = phyEnMsk4 | (1<<7);
      /* Speed selection [1:0] : 11=Reserved, 10=1000Mpbs, 01=100Mpbs, 00=10Mpbs */
      phyEnMsk0 = phyEnMsk0 & (~(1<<6));
      phyEnMsk0 = phyEnMsk0 | (1<<13);
    }

  /* 100BASE-TX full duplex capable in reg 4.8 */
  if (1 == ptr_ability->Full_100)
    {
      phyEnMsk4 = phyEnMsk4 | (1<<8);
      /*Speed selection [1:0] : 11=Reserved, 10=1000Mpbs, 01=100Mpbs, 00=10Mpbs */
      phyEnMsk0 = phyEnMsk0 & (~(1<<6));
      phyEnMsk0 = phyEnMsk0 | (1<<13);
      /*Full duplex mode in reg 0.8*/
      phyEnMsk0 = phyEnMsk0 | (1<<8);
    }

  /* 1000 BASE-T FULL duplex capable setting in reg 9.9 */
  if (1 == ptr_ability->Full_1000)
    {
      phyEnMsk9 = phyEnMsk9 | (1<<9);
      /* Speed selection [1:0] : 11=Reserved, 10=1000Mpbs, 01=100Mpbs, 00=10Mpbs */
      phyEnMsk0 = phyEnMsk0 | (1<<6);
      phyEnMsk0 = phyEnMsk0 & (~(1<<13));
#if 0
      if (ptr_ability->AutoNegotiation != 1)
        return -1;
#endif
    }

  /*Auto-Negotiation setting in reg 0.12*/
  if (1 == ptr_ability->AutoNegotiation)
      phyEnMsk0 = phyEnMsk0 | (1<<12);

  /*Asymetric flow control in reg 4.11*/
  if (1 == ptr_ability->AsyFC)
    phyEnMsk4 = phyEnMsk4 | (1<<11);

  /*Flow control in reg 4.10*/
  if (1 == ptr_ability->FC)
    phyEnMsk4 = phyEnMsk4 | (1<<10);

  /*1000 BASE-T control register setting*/
  if (0 != rtl8368s_get_asic_phy_regs(phy, 0, RTL8366RB_PHY_1000_BASET_CONTROL_REG, &phyData))
    return -1;

  phyData = (phyData & (~0x0200)) | phyEnMsk9 ;

  if (0 != rtl8368s_set_asic_phy_regs(phy, 0, RTL8366RB_PHY_1000_BASET_CONTROL_REG, phyData))
    return -1;

  /*Auto-Negotiation control register setting*/
  if (0 != rtl8368s_get_asic_phy_regs(phy, 0, RTL8366RB_PHY_AN_ADVERTISEMENT_REG, &phyData))
    return -1;

  phyData = (phyData & (~0x0DE0)) | phyEnMsk4;

  if (0 != rtl8368s_set_asic_phy_regs(phy, 0, RTL8366RB_PHY_AN_ADVERTISEMENT_REG, phyData))
    return -1;

  /*Control register setting and restart auto*/
  if (0 != rtl8368s_get_asic_phy_regs(phy, 0, RTL8366RB_PHY_CONTROL_REG, &phyData))
    return -1;

  phyData = (phyData & (~0x3140)) | phyEnMsk0;
  /*If have auto-negotiation capable, then restart auto negotiation*/
  if (1 == ptr_ability->AutoNegotiation)
    phyData = phyData | (1 << 9);

  if (0 != rtl8368s_set_asic_phy_regs(phy, 0, RTL8366RB_PHY_CONTROL_REG, phyData))
    return -1;

  return 0;
}

/*
   @func int32_t | rtl8366rb_get_ethernet_phy | Get PHY ability through PHY registers.
   @parm uint32_t | phy | PHY number (0~4).
   @parm phyAbility_t* | ptr_ability | Ability structure
   @rvalue 0 | Success.
   @rvalue -1 | Failure.
   @comm
   Get the capablity of specified PHY.
*/
int32_t rtl8366rb_get_ethernet_phy(uint32_t phy, rtl8366rb_phyAbility_t *ptr_ability)
{
  uint32_t phyData0;
  uint32_t phyData4;
  uint32_t phyData9;

  if (phy > RTL8366RB_PHY_NO_MAX)
    return -1;

  /*Control register setting and restart auto*/
  if (0 != rtl8368s_get_asic_phy_regs(phy, 0, RTL8366RB_PHY_CONTROL_REG, &phyData0))
    return -1;

  /*Auto-Negotiation control register setting*/
  if (0 != rtl8368s_get_asic_phy_regs(phy, 0, RTL8366RB_PHY_AN_ADVERTISEMENT_REG, &phyData4))
    return -1;

  /*1000 BASE-T control register setting*/
  if (0 != rtl8368s_get_asic_phy_regs(phy, 0, RTL8366RB_PHY_1000_BASET_CONTROL_REG, &phyData9))
    return -1;

  if (phyData9 & (1 << 9))
    ptr_ability->Full_1000 = 1;
  else
    ptr_ability->Full_1000 = 0;

  if (phyData4 & (1 << 11))
    ptr_ability->AsyFC = 1;
  else
    ptr_ability->AsyFC = 0;

  if (phyData4 & (1 << 10))
    ptr_ability->FC = 1;
  else
    ptr_ability->FC = 0;

  if (phyData4 & (1 << 8))
    ptr_ability->Full_100 = 1;
  else
    ptr_ability->Full_100 = 0;

  if (phyData4 & (1 << 7))
    ptr_ability->Half_100 = 1;
  else
    ptr_ability->Half_100 = 0;

  if (phyData4 & (1 << 6))
    ptr_ability->Full_10 = 1;
  else
    ptr_ability->Full_10 = 0;

  if (phyData4 & (1<<5))
    ptr_ability->Half_10 = 1;
  else
    ptr_ability->Half_10 = 0;

  if (phyData0 & (1 << 12))
    ptr_ability->AutoNegotiation = 1;
  else
    ptr_ability->AutoNegotiation = 0;

  return 0;
}

/*
   @func int32_t | rtl8366rb_get_phy_link_status | Get ethernet PHY linking status
   @parm uint32_t | phy | PHY number (0~4).
   @parm uint32_t* | ptr_linkStatus | PHY link status 1:link up 0:link down
   @rvalue 0 | Success.
   @rvalue -1 | Failure.
   @comm
   Output link status of bit 2 in PHY register 1. API will return
   status is link up under both auto negotiation complete and link
   status are set to 1.
*/
int32_t rtl8366rb_get_phy_link_status(uint32_t phy, uint32_t *ptr_linkStatus)
{
  uint32_t phyData;

  if (phy > RTL8366RB_PHY_NO_MAX)
    return -1;

  /*Get PHY status register*/
  if (0 != rtl8368s_get_asic_phy_regs(phy, 0, RTL8366RB_PHY_STATUS_REG, &phyData))
    return -1;

  /*check link status*/
  if (phyData & (1 << 2))
    *ptr_linkStatus = 1;
  else
    *ptr_linkStatus = 0;

  return 0;
}

/*
   @func int32_t | rtl8366rb_set_led_blink_rate | Set LED blinking rate at mode 0 to mode 3
   @parm rtl8366rb_led_blinkrate_t | blinkRate | Support 6 blinking rate.
   @rvalue 0 | Success.
   @rvalue -1 | Failure.
   @comm
   ASIC support 6 types of LED blinking rates at 28ms, 56ms, 84ms, 111ms, 222ms and 446ms.
*/
int32_t rtl8366rb_set_led_blink_rate(rtl8366rb_led_blinkrate_t blinkRate)
{
  if (blinkRate >= RTL8366RB_LEDBLINKRATE_MAX)
    return -1;

  if (0 != rtl8368s_set_asic_led_blink_rate(blinkRate))
    return -1;

  return 0;
}

/*
   @func int32_t | rtl8366rb_init_vlan | Initialize VLAN.
   @parm uint32_t | flag | default VLAN(vid 1) ON/OFF flag.
   @rvalue  0 | Success.
   @rvalue -1 | Failure.
   @comm
   VLAN is disabled by default. User has to call this API to enable VLAN before
   using it. And It will set a default VLAN(vid 1) including all ports and set
   all ports PVID to the default VLAN by flag.
*/
int32_t rtl8366rb_init_vlan(uint32_t flag)
{
  uint32_t i;
  rtl8368s_user_vlan4kentry vlan4K;
  rtl8368s_vlanconfig       vlanMC;

  /* clear 16 VLAN member configuration */
  for (i = 0 ; i <= RTL8366RB_VLANMCIDXMAX ; i++)
    {
      vlanMC.vid = 0;
      vlanMC.priority = 0;
      vlanMC.member = 0;
      vlanMC.untag = 0;
      vlanMC.fid = 0;
      if (rtl8368s_set_asic_vlan_member_config(i, vlanMC) != 0)
        return -1;
    }

  /* Set a default VLAN with vid 1 to 4K table for all ports */
  vlan4K.vid = 1;
  if (flag)
    {
      vlan4K.member = RTL8366RB_PORTMASK;
      vlan4K.untag = RTL8366RB_PORTMASK;
    }
  else
    {
      vlan4K.member = 0;
      vlan4K.untag = 0;
    }
  vlan4K.fid = 0;
  if (rtl8368s_set_asic_vlan_4k_entry(vlan4K) != 0)
    return -1;

  /* Also set the default VLAN to 16 member configuration index 0 */
  vlanMC.vid = 1;
  vlanMC.priority = 0;
  if (flag)
    {
      vlanMC.member = RTL8366RB_PORTMASK;
      vlanMC.untag = RTL8366RB_PORTMASK;
    }
  else
    {
      vlanMC.member = 0;
      vlanMC.untag = 0;
    }
  vlanMC.fid = 0;
  if (rtl8368s_set_asic_vlan_member_config(0, vlanMC) != 0)
    return -1;

  /* Set all ports PVID to default VLAN */
  for (i = 0; i < PORT_MAX; i++)
    {
      if (rtl8368s_set_asic_vlan_port_based_vid(i, 0) != 0)
        return -1;
    }

  /* enable VLAN and 4K VLAN */
  if (rtl8368s_set_asic_vlan(TRUE)!= 0)
    return -1;
  if (rtl8368s_set_asic_vlan_4k_tb_usage(TRUE)!= 0)
    return -1;

  return 0;
}

/*
   @func int32_t | rtl8366rb_clear_vlan | clear VLAN.
   @rvalue  0 | Success.
   @rvalue -1 | Failure.
   @comm
   VLAN is disabled. User has to call this API to disable VLAN.
   And It will unset a default VLAN(vid 1) and unset PVID to the default VLAN.
*/
int32_t rtl8366rb_clear_vlan(void)
{
  uint32_t i;
  rtl8368s_user_vlan4kentry vlan4K;
  rtl8368s_vlanconfig       vlanMC;

  /* clear 16 VLAN member configuration */
  for (i = 0 ; i <= RTL8366RB_VLANMCIDXMAX ; i++)
    {
      vlanMC.vid = 0;
      vlanMC.priority = 0;
      vlanMC.member = 0;
      vlanMC.untag = 0;
      vlanMC.fid = 0;
      if (rtl8368s_set_asic_vlan_member_config(i, vlanMC) != 0)
        return -1;
    }

  /* clear vid 1 of VLAN to 4K table for all ports */
  vlan4K.vid = 1;
  vlan4K.member = 0;
  vlan4K.untag = 0;
  vlan4K.fid = 0;
  if (rtl8368s_set_asic_vlan_4k_entry(vlan4K) != 0)
    return -1;

  /* Also clear the default VLAN to 16 member configuration index 0 */
  vlanMC.vid = 1;
  vlanMC.priority = 0;
  vlanMC.member = 0;
  vlanMC.untag = 0;
  vlanMC.fid = 0;
  if (rtl8368s_set_asic_vlan_member_config(0, vlanMC) != 0)
    return -1;

  /* Set all ports PVID to default VLAN */
  for (i = 0; i < PORT_MAX; i++)
    {
      if (rtl8368s_set_asic_vlan_port_based_vid(i, 0) != 0)
        return -1;
    }

  /* disable VLAN and 4K VLAN */
  if (rtl8368s_set_asic_vlan(FALSE)!= 0)
    return -1;
  if (rtl8368s_set_asic_vlan_4k_tb_usage(FALSE)!= 0)
    return -1;

  return 0;
}

/*
   @func int32_t | rtl8366rb_set_vlan | Set a VLAN entry.
   @parm rtl8366rb_vlanConfig_t* | ptr_vlancfg | The pointer of VLAN configuration.
   @rvalue ERRNO_API_INPUT | invalid input parameters.
   @rvalue  0 | Success.
   @rvalue -1 | Failure.
   @comm
   There are 4K VLAN entry supported. Member set and untag set of 4K VLAN entry
   are all zero by default. User could configure the member set and untag set
   for specified vid through this API. The portmask's bit N means port N.
   For example, mbrmask 23=0x17=010111 means port 0,1,2,4 in the member set.
*/
int32_t rtl8366rb_set_vlan(rtl8366rb_vlanConfig_t *ptr_vlancfg)
{
  uint32_t i;
  rtl8368s_user_vlan4kentry vlan4K;
  rtl8368s_vlanconfig       vlanMC;

  if (ptr_vlancfg == NULL)
    return ERRNO_INPUT;

  /* vid must be 0~4095 */
  if (ptr_vlancfg->vid > RTL8366RB_VIDMAX)
    return ERRNO_VID;

  if (ptr_vlancfg->mbrmsk > RTL8366RB_PORTMASK)
    return ERRNO_MBR;

  if (ptr_vlancfg->untagmsk > RTL8366RB_PORTMASK)
    return ERRNO_UTAG;

  /* fid must be 0~7 */
  if (ptr_vlancfg->fid > RTL8366RB_FIDMAX)
    return ERRNO_FID;

  /* update 4K table */
  vlan4K.vid    = ptr_vlancfg->vid;
  vlan4K.member = ptr_vlancfg->mbrmsk;
  vlan4K.untag  = ptr_vlancfg->untagmsk;
  vlan4K.fid    = ptr_vlancfg->fid;
  if (rtl8368s_set_asic_vlan_4k_entry(vlan4K) != 0)
    return -1;

  /*
   * Since VLAN entry would be copied from 4K to 16 member configuration while
   * setting Port-based VLAN. So also update the entry in 16 member configuration
   * if it existed.
   */
  for (i = 0; i <= RTL8366RB_VLANMCIDXMAX; i++)
    {
      if (rtl8368s_get_asic_vlan_member_config(i, &vlanMC) != 0)
        return -1;

      if (ptr_vlancfg->vid == vlanMC.vid)
        {
          vlanMC.member = ptr_vlancfg->mbrmsk;
          vlanMC.untag  = ptr_vlancfg->untagmsk;
          vlanMC.fid    = ptr_vlancfg->fid;
          if (rtl8368s_set_asic_vlan_member_config(i, vlanMC) != 0)
            return -1;

          return 0;
        }
    }

  return 0;
}

/*
   @func int32_t | rtl8366rb_get_vlan | Get a VLAN entry.
   @parm rtl8366rb_vlanConfig_t* | ptr_vlancfg | The pointer of VLAN configuration.
   @rvalue ERRNO_API_INPUT | invalid input parameters.
   @rvalue  0 | Success.
   @rvalue -1 | Failure.
   @comm
   The API can get the member set and untag set settings for specified vid.
*/
int32_t rtl8366rb_get_vlan(rtl8366rb_vlanConfig_t *ptr_vlancfg)
{
  rtl8368s_user_vlan4kentry vlan4K;

  if (ptr_vlancfg == NULL)
    return ERRNO_INPUT;

  /* vid must be 0~4095 */
  if (ptr_vlancfg->vid > RTL8366RB_VIDMAX)
    return ERRNO_VID;

  vlan4K.vid = ptr_vlancfg->vid;
  if (rtl8368s_get_asic_vlan_4k_entry(&vlan4K) != 0)
    return -1;

  ptr_vlancfg->mbrmsk   = vlan4K.member;
  ptr_vlancfg->untagmsk = vlan4K.untag;
  ptr_vlancfg->fid      = vlan4K.fid;
  return 0;
}

/*
   @func int32_t  | rtl8366rb_set_vlan_pvid | Set port to specified VLAN ID(PVID).
   @parm uint32_t | port | Port number (0~5).
   @parm uint32_t | vid | Specified VLAN ID (0~4095).
   @parm uint32_t | priority | 802.1p priority for the PVID (0~7).
   @rvalue ERRNO_API_INPUT | invalid input parameters.
   @rvalue  0 | Success.
   @rvalue -1 | Failure.
   @comm
   The API is used for Port-based VLAN. The untagged frame received from the
   port will be classified to the specified VLAN and assigned to the specified priority.
   Make sure you have configured the VLAN by 'rtl8366rb_set_vlan' before using the API.
*/
int32_t rtl8366rb_set_vlan_pvid(uint32_t port, uint32_t vid, uint32_t priority)
{
  uint32_t i;
  uint32_t j;
  uint32_t index;
  uint8_t  bUsed;
  rtl8368s_user_vlan4kentry vlan4K;
  rtl8368s_vlanconfig       vlanMC;

  if (port >= PORT_MAX)
    return ERRNO_PORT_NUM;

  /* vid must be 0~4095 */
  if (vid > RTL8366RB_VIDMAX)
    return ERRNO_VID;

  /* priority must be 0~7 */
  if (priority > RTL8366RB_PRIORITYMAX)
    return ERRNO_PRIORITY;

  /*
   * Search 16 member configuration to see if the entry already existed.
   * If existed, update the priority and assign the index to the port.
   */
  for (i = 0; i <= RTL8366RB_VLANMCIDXMAX; i++)
    {
      if (rtl8368s_get_asic_vlan_member_config(i, &vlanMC) != 0)
        return -1;

      if (vid == vlanMC.vid)
        {
          vlanMC.priority = priority;
          if (rtl8368s_set_asic_vlan_member_config(i, vlanMC) != 0)
            return -1;
          if (rtl8368s_set_asic_vlan_port_based_vid(port, i) != 0)
            return -1;

          return 0;
        }
    }

  /*
   * vid doesn't exist in 16 member configuration. Find an empty entry in
   * 16 member configuration, then copy entry from 4K. If 16 member configuration
   * are all full, then find an entry which not used by Port-based VLAN and
   * then replace it with 4K. Finally, assign the index to the port.
   */
  for (i = 0; i <= RTL8366RB_VLANMCIDXMAX; i++)
    {
      if (rtl8368s_get_asic_vlan_member_config(i, &vlanMC) != 0)
        return -1;

      if (vlanMC.vid == 0 && vlanMC.member == 0)
        {
          vlan4K.vid = vid;
          if (rtl8368s_get_asic_vlan_4k_entry(&vlan4K) != 0)
            return -1;

          vlanMC.vid = vid;
          vlanMC.priority = priority;
          vlanMC.member = vlan4K.member;
          vlanMC.untag = vlan4K.untag;
          vlanMC.fid = vlan4K.fid;
          if (rtl8368s_set_asic_vlan_member_config(i, vlanMC) != 0)
            return -1;

      if (rtl8368s_set_asic_vlan_port_based_vid(port, i) != 0)
        return -1;

      return 0;
    }
  }

  /* 16 member configuration is full, found a unused entry to replace */
  for (i = 0; i <= RTL8366RB_VLANMCIDXMAX; i++)
    {
      bUsed = FALSE;

      for (j = 0; j < PORT_MAX; j++)
        {
          if(rtl8368s_get_asic_vlan_port_based_vid(j, &index) != 0)
            return -1;

          if (i == index) /* index i is in use by port j */
            {
              bUsed = TRUE;
              break;
            }
        }

      if (bUsed == FALSE) /* found a unused index, replace it */
        {
          vlan4K.vid = vid;
          if (rtl8368s_get_asic_vlan_4k_entry(&vlan4K) != 0)
            return -1;

          vlanMC.vid = vid;
          vlanMC.priority = priority;
          vlanMC.member = vlan4K.member;
          vlanMC.untag = vlan4K.untag;
          vlanMC.fid = vlan4K.fid;
          vlanMC.stag_idx = 0;
          vlanMC.stag_mbr = 0;
          if (rtl8368s_set_asic_vlan_member_config(i, vlanMC) != 0)
            return -1;

          if (rtl8368s_set_asic_vlan_port_based_vid(port, i) != 0)
            return -1;

          return 0;
        }
    }

  return -1;
}

/*
   @func int32_t | rtl8366rb_get_vlan_pvid | Get VLAN ID(PVID) on specified port.
   @parm uint32_t | port | Port number (0~5).
   @parm uint32_t* | ptr_vid | pointer to returned VLAN ID.
   @parm uint32_t* | ptr_priority | pointer to returned 802.1p priority.
   @rvalue ERRNO_API_INPUT | invalid input parameters.
   @rvalue  0 | Success.
   @rvalue -1 | Failure.
   @comm
   The API can get the PVID and 802.1p priority for the PVID of Port-based VLAN.
*/
int32_t rtl8366rb_get_vlan_pvid(uint32_t port, uint32_t *ptr_vid, uint32_t *ptr_priority)
{
  uint32_t index;
  rtl8368s_vlanconfig vlanMC;

  if (port >= RTL8366RB_PORT_MAX)
    return ERRNO_PORT_NUM;

  if (rtl8368s_get_asic_vlan_port_based_vid(port, &index) != 0)
    return -1;

  if (rtl8368s_get_asic_vlan_member_config(index, &vlanMC) != 0)
    return -1;

  *ptr_vid = vlanMC.vid;
  *ptr_priority = vlanMC.priority;
  return 0;
}

/*
   @func int32_t | rtl8366rb_set_vlan_ingress_filter | Set VLAN ingress for each port.
   @parm uint32_t | port | Physical port number (0~5).
   @parm uint32_t | enabled | VLAN ingress function 1:enable 0:disable.
   @rvalue   0 | Success.
   @rvalue  -1 | Failure.
   @comm
   Length/type of 802.1q VLAN tag is 0x8100. While VLAN function is enabled,
   ASIC will decide VLAN ID for each received frame and get belonged member
   ports from VLAN table. If received port is not belonged to VLAN member ports,
   ASIC will drop received frame if VLAN ingress function is enabled.
*/
int32_t rtl8366rb_set_vlan_ingress_filter(uint32_t port, uint32_t enabled)
{
  if (port >= RTL8366RB_PORT_MAX)
    return ERRNO_PORT_NUM;

  if (0 != rtl8368s_set_asic_vlan_ingress_filtering(port,enabled))
    return -1;

  return 0;
}

/*
   @func int32_t | rtl8366rb_set_vlan_accept_frame_type | Set VLAN support frame type
   @parm uint32_t | port | Physical port number (0~5).
   @parm rtl8366rb_accept_frame_t | type |
         Supported frame types 0:all 1:C-tage frame 2:un-tag frame
   @rvalue  0 | Success.
   @rvalue -1 | Failure.
   @comm
   Length/type of 802.1q VLAN tag is 0x8100. While VLAN function is enabled,
   ASIC will parse length/type of reveived frame to check if there is a existed
   C-tag to permit frame or not. This function will *NOT* work while ASIC VLAN
   function is disabled(ASIC will accept all types of frame). Function of accept
   un-tag frame setting is not adapted to uplink port because only S-tag
   (the first length/type field is not C-tag) frame will be permitted at uplink port.
   Function of accept C-tag frame setting is not adapted to uplink port and CPU port
   because frame contained S-tag(0x88a8) or CPU tag (0x8899) will also be dropped.
   User sould take care the side effect of VLAN tag filtering function in the port
   belongs to CPU port or uplink port.
*/
int32_t rtl8366rb_set_vlan_accept_frame_type(uint32_t port, rtl8366rb_accept_frame_t type)
{
  if (port >= RTL8366RB_PORT_MAX)
    return ERRNO_PORT_NUM;

  switch(type)
    {
    case RTL8366RB_FRAME_TYPE_ALL:
      if (0 != rtl8368s_set_asic_vlan_drop_tagged_packets(port, DISABLE))
        return -1;

      if (0 != rtl8368s_set_asic_vlan_accept_tagged_only(port, DISABLE))
        return -1;
      break;
    case RTL8366RB_FRAME_TYPE_CTAG:
      if (0 != rtl8368s_set_asic_vlan_drop_tagged_packets(port, DISABLE))
        return -1;

      if (0 != rtl8368s_set_asic_vlan_accept_tagged_only(port, ENABLE))
        return -1;
      break;
    case RTL8366RB_FRAME_TYPE_UNTAG:
      if (0 != rtl8368s_set_asic_vlan_drop_tagged_packets(port, ENABLE))
        return -1;
      break;
    default:
      return -1;
    }

  return 0;
}

/*
   @func int32_t | rtl8366rb_set_vlan_keep_ctag_format |
         Set VLAN keep receiving tag/untag format in ingress port.
   @parm uint32_t | ingressport | Ingress port number (0~5).
   @uint32_t | portmask | Igress port mask (0x00-0x3F).
   @rvalue 0 | Success.
   @rvalue ERRNO_SMIERROR | SMI access error.
   @rvalue ERRNO_INPUT | Invalid port number.
   @comm
   Output frame from ASIC will not be tagged C-tag or untagged
   C-tag if egress port was set enable keep-Ctag-format function.
   Receiving frame with untag format will be output with untag
   format, priority-tag frame will be output as priority-tag
   frame and c-tagged frame will be output as original c-tagged
   frame. This configuration is configured per-ingress-port basis.
   Users can specify engress port mask for a ingress port.
*/
int32_t rtl8366rb_set_vlan_keep_ctag_format(uint32_t ingressport, uint32_t portmask)
{
  if (0 != rtl8368s_set_asic_vlan_keep_ctag_format(ingressport, portmask))
    return -1;

  return 0;
}

/*
   @func int32_t | rtl8366rb_set_source_mac_learning | Set per-port source mac learning function.
   @parm uint32_t | port | Physical port number (0~5).
   @parm uint32_t | enabled | Source mac learning function 1:enable 0:disable.
   @rvalue 0 | Success.
   @rvalue -1 | Failure.
   @comm
   Like CPU port usage, the source mac of frame from CPU port
   will be learned if this frame is just traped to CPU port
   with frame modification only and re-send from CPU. This frame
   will be learned twiced and the source port information of
   L2 entry about this frame maybe is wrong.
*/
int32_t rtl8366rb_set_source_mac_learning(uint32_t port, uint32_t enabled)
{
  if(port >= RTL8366RB_PORT_MAX)
    return -1;

  if(0 != rtl8368s_set_asic_port_learn_disable(port, enabled?FALSE:TRUE))
    return -1;

  return 0;
}

/*
   @func int32_t | rtl8366rb_get_source_mac_learning | Get per-port source mac learning function.
   @parm uint32_t | port | Physical port number (0~5).
   @parm uint32_t* | ptr_enabled | Source mac learning function 1:enable 0:disable.
   @rvalue 0 | Success.
   @rvalue -1 | Failure.
   @comm
   Like CPU port usage, the source mac of frame from CPU port
   will be learned if this frame is just traped to CPU port
   with frame modification only and re-send from CPU. This frame
   will be learned twiced and the source port information of L2
   entry about this frame maybe is wrong.
*/
int32_t rtl8366rb_get_source_mac_learning(uint32_t port, uint32_t *ptr_enabled)
{
  uint32_t disabled;

  if(port >= RTL8366RB_PORT_MAX)
    return -1;

  if(0 != rtl8368s_get_asic_port_learn_disable(port, &disabled))
    return -1;

  *ptr_enabled = (disabled ? 0 : 1);
  return 0;
}

/*
   @func int32_t | rtl8366rb_init_chip | Set chip to default configuration enviroment
   @rvalue  0 | Success.
   @rvalue -1 | Failure.
   @comm
   The API can set chip registers to default configuration for different release
   chip model.
*/
int32_t rtl8366rb_init_chip(void)
{
  uint32_t index;
  uint32_t ledGroup;
  enum RTL8368S_LEDCONF ledCfg[RTL8366RB_LED_GROUP_MAX];
  uint32_t verid;

  const uint32_t chipDefault[][2] =
        {
          {0x000B, 0x0001},{0x03A6, 0x0100},{0x3A7, 0x0001},{0x02D1, 0x3FFF},
          {0x02D2, 0x3FFF},{0x02D3, 0x3FFF},{0x02D4, 0x3FFF},{0x02D5, 0x3FFF},
          {0x02D6, 0x3FFF},{0x02D7, 0x3FFF},{0x02D8, 0x3FFF},{0x022B, 0x0688},
          {0x022C, 0x0FAC},{0x03D0, 0x4688},{0x03D1, 0x01F5},{0x0000, 0x0830},
          {0x02F9, 0x0200},{0x02F7, 0x7FFF},{0x02F8, 0x03FF},{0x0080, 0x03E8},
          {0x0081, 0x00CE},{0x0082, 0x00DA},{0x0083, 0x0230},{0xBE0F, 0x2000},
          {0x0231, 0x422A},{0x0232, 0x422A},{0x0233, 0x422A},{0x0234, 0x422A},
          {0x0235, 0x422A},{0x0236, 0x422A},{0x0237, 0x422A},{0x0238, 0x422A},
          {0x0239, 0x422A},{0x023A, 0x422A},{0x023B, 0x422A},{0x023C, 0x422A},
          {0x023D, 0x422A},{0x023E, 0x422A},{0x023F, 0x422A},{0x0240, 0x422A},
          {0x0241, 0x422A},{0x0242, 0x422A},{0x0243, 0x422A},{0x0244, 0x422A},
          {0x0245, 0x422A},{0x0246, 0x422A},{0x0247, 0x422A},{0x0248, 0x422A},
          {0x0249, 0x0146},{0x024A, 0x0146},{0x024B, 0x0146},{0xBE03, 0xC961},
          {0x024D, 0x0146},{0x024E, 0x0146},{0x024F, 0x0146},{0x0250, 0x0146},
          {0xBE64, 0x0226},{0x0252, 0x0146},{0x0253, 0x0146},{0x024C, 0x0146},
          {0x0251, 0x0146},{0x0254, 0x0146},{0xBE62, 0x3FD0},{0x0084, 0x0320},
          {0x0255, 0x0146},{0x0256, 0x0146},{0x0257, 0x0146},{0x0258, 0x0146},
          {0x0259, 0x0146},{0x025A, 0x0146},{0x025B, 0x0146},{0x025C, 0x0146},
          {0x025D, 0x0146},{0x025E, 0x0146},{0x025F, 0x0146},{0x0260, 0x0146},
          {0x0261, 0xA23F},{0x0262, 0x0294},{0x0263, 0xA23F},{0x0264, 0x0294},
          {0x0265, 0xA23F},{0x0266, 0x0294},{0x0267, 0xA23F},{0x0268, 0x0294},
          {0x0269, 0xA23F},{0x026A, 0x0294},{0x026B, 0xA23F},{0x026C, 0x0294},
          {0x026D, 0xA23F},{0x026E, 0x0294},{0x026F, 0xA23F},{0x0270, 0x0294},
          {0x02F5, 0x0048},{0xBE09, 0x0E00},{0xBE1E, 0x0FA0},{0xBE14, 0x8448},
          {0xBE15, 0x1007},{0xBE4A, 0xA284},{0xC454, 0x3F0B},{0xC474, 0x3F0B},
          {0xBE48, 0x3672},{0xBE4B, 0x17A7},{0xBE4C, 0x0B15},{0xBE52, 0x0EDD},
          {0xBE49, 0x8C00},{0xBE5B, 0x785C},{0xBE5C, 0x785C},{0xBE5D, 0x785C},
          {0xBE61, 0x368A},{0xBE63, 0x9B84},{0xC456, 0xCC13},{0xC476, 0xCC13},
          {0xBE65, 0x307D},{0xBE6D, 0x0005},{0xBE6E, 0xE120},{0xBE2E, 0x7BAF},
          {0xFFFF, 0xABCD}
        };

  const uint32_t chipDefault2[][2] =
        {
          {0x0000, 0x0830},{0x0400, 0x8130},{0x000A, 0x83ED},{0x0431, 0x5432},
          {0x0F51, 0x0017},{0x02F5, 0x0048},{0x02FA, 0xFFDF},{0x02FB, 0xFFE0},
          {0xC456, 0x0C14},{0xC476, 0x0C14},{0xC454, 0x3F8B},{0xC474, 0x3F8B},
          {0xC450, 0x2071},{0xC470, 0x2071},{0xC451, 0x226B},{0xC471, 0x226B},
          {0xC452, 0xA293},{0xC472, 0xA293},{0xC44C, 0x1585},{0xC44C, 0x1185},
          {0xC44C, 0x1585},{0xC46C, 0x1585},{0xC46C, 0x1185},{0xC46C, 0x1585},
          {0xC44C, 0x0185},{0xC44C, 0x0181},{0xC44C, 0x0185},{0xC46C, 0x0185},
          {0xC46C, 0x0181},{0xC46C, 0x0185},{0xBE24, 0xB000},{0xBE23, 0xFF51},
          {0xBE22, 0xDF20},{0xBE21, 0x0140},{0xBE20, 0x00BB},{0xBE24, 0xB800},
          {0xBE24, 0x0000},{0xBE24, 0x7000},{0xBE23, 0xFF51},{0xBE22, 0xDF60},
          {0xBE21, 0x0140},{0xBE20, 0x0077},{0xBE24, 0x7800},{0xBE24, 0x0000},
          {0xBE2E, 0x7BA7},{0xBE36, 0x1000},{0xBE37, 0x1000},{0x8000, 0x0001},
          {0xBE69, 0xD50F},{0x8000, 0x0000},{0xBE69, 0xD50F},{0xBE6B, 0x0320},
          {0xBE77, 0x2800},{0xBE78, 0x3C3C},{0xBE79, 0x3C3C},{0xBE6E, 0xE120},
          {0x8000, 0x0001},{0xBE10, 0x8140},{0x8000, 0x0000},{0xBE10, 0x8140},
          {0xBE15, 0x1007},{0xBE14, 0x0448},{0xBE1E, 0x00A0},{0xBE10, 0x8160},
          //Power off rtl8366 chip. by jong2ry 2012.03.20
          //{0xBE10, 0x8140},{0xBE00, 0x1340},{0x0450, 0x0000},{0x0401, 0x0000},
          {0xBE10, 0x8140},{0xBE00, 0x1B40},{0x0450, 0x0000},{0x0401, 0x0000},
          {0xFFFF, 0xABCD}
        };

  for (ledGroup = 0 ; ledGroup < RTL8366RB_LED_GROUP_MAX ; ledGroup++)
    {
      if(0 != rtl8368s_get_asic_led_indicate_info_config(ledGroup, &ledCfg[ledGroup]))
        return -1;

      if(0 != rtl8368s_set_asic_led_indicate_info_config(ledGroup, LEDCONF_LEDFORCE))
        return -1;
    }

  if (0 != rtl8368s_set_asic_force_leds(0x3F, 0x3F, 0x3F, 0x3F))
    return -1;

  if(0 != rtl8368s_get_asic_reg(RTL8368S_REVISION_ID_REG, &verid))
    return -1;

  index = 0;
  switch (verid)
    {
    case 0x0000:
      while (chipDefault[index][0] != 0xFFFF && chipDefault[index][1] != 0xABCD)
        {
          /*PHY registers setting*/
          if (0xBE00 == (chipDefault[index][0] & 0xBE00))
            {
              if(0 != rtl8368s_set_asic_reg(RTL8368S_PHY_ACCESS_CTRL_REG,
                                            RTL8368S_PHY_CTRL_WRITE))
                return -1;
            }

          if(0 != rtl8368s_set_asic_reg(chipDefault[index][0], chipDefault[index][1])){
            return -1;
          }

          index ++;
        }
      break;

    case 0x0001:
    case 0x0002:
    case 0x0003:
    default:
      while (chipDefault2[index][0] != 0xFFFF && chipDefault2[index][1] != 0xABCD)
        {
          /*PHY registers setting*/
          if (0xBE00 == (chipDefault2[index][0] & 0xBE00))
            {
              if(0 != rtl8368s_set_asic_reg(RTL8368S_PHY_ACCESS_CTRL_REG,
                                            RTL8368S_PHY_CTRL_WRITE))
                return -1;
            }

          if(0 != rtl8368s_set_asic_reg(chipDefault2[index][0], chipDefault2[index][1])){
            return -1;
          }

          index ++;
        }
      break;
    }

  for (ledGroup = 0 ; ledGroup < RTL8366RB_LED_GROUP_MAX ; ledGroup++)
    {
      if(0 != rtl8368s_set_asic_led_indicate_info_config(ledGroup, ledCfg[ledGroup]))
        return -1;
    }

  return 0;
}

void rtl8366rb_power_on(void)
{
  int port = 0;
#if 0
  // 초기화 하는 부분에서 복사해 온 코드.
  // 각각 port로 제어하는 것이 아니고 레지스터에 씀으로 모든 port를 제어한다.
  const uint32_t addr = 0xBE00;
  const uint32_t data = 0x1340;


  if(0 != rtl8368s_set_asic_reg(RTL8368S_PHY_ACCESS_CTRL_REG,
                                RTL8368S_PHY_CTRL_WRITE)) {
  }

  if(0 != rtl8368s_set_asic_reg(addr, data) ){
  }
#endif

  // port별로 power-down 모드로 해제한다.
  for ( port = 0; port < RTL8366RB_PHY_NO_MAX; port++) {
    if (0 != rtl8368s_set_asic_phy_regs(port, 0, RTL8366RB_PHY_CONTROL_REG, 0x1340))
      return;
  }
}

void rtl8366rb_power_down(void)
{
  int port = 0;
#if 0
  // 초기화 하는 부분에서 복사해 온 코드.
  // 각각 port로 제어하는 것이 아니고 레지스터에 씀으로 모든 port를 제어한다.
  const uint32_t addr = 0xBE00;
  const uint32_t data = 0x1940;

  if(0 != rtl8368s_set_asic_reg(RTL8368S_PHY_ACCESS_CTRL_REG,
                                RTL8368S_PHY_CTRL_WRITE)) {
  }

  if(0 != rtl8368s_set_asic_reg(addr, data) ){
  }
#endif

  // port별로 power-down 모드로 설정한다.
  for ( port = 0; port < RTL8366RB_PHY_NO_MAX; port++) {
    if (0 != rtl8368s_set_asic_phy_regs(port, 0, RTL8366RB_PHY_CONTROL_REG, 0x1940))
      return;
  }
}

/*
 * ex: ts=8 sw=2 sts=2 et
 */
