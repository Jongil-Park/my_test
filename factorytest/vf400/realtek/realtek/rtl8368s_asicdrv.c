/*
* Copyright c                  Realtek Semiconductor Corporation, 2006 
* All rights reserved.
* 
* Program : RTL8366S switch low-level API
* Abstract : 
* Author :Abel Hsu(abelshie@realtek.com.tw)                
*  $Id: rtl8368s_asicdrv.c,v 1.26 2008-06-11 08:51:37 hmchung Exp $
*/
/*
   @doc RTL8368S_ASICDRIVER_API
   
   @module Rtl8366S_AsicDrv.c - RTL8366S Switch asic driver API documentation	|
   This document explains API interface of the asic. 
   @normal 

   Copyright <cp>2007 Realtek<tm> Semiconductor Cooperation, All Rights Reserved.
   
   @head3 List of Symbols | Here is a list of all functions and variables in this module.
   
   @index | RTL8368S_ASICDRIVER_API
*/

#include "rtl8368s_asicdrv.h"

/*=======================================================================
 * Asic read/write driver through SMI
 *========================================================================*/
/*
   @func int32_t | rtl8368s_set_asic_reg_bit | Set a bit value of a specified register.
   @parm uint32_t | reg | Register's address.
   @parm uint32_t | bit | Bit location. For 16-bits register only. 
         Maximun value is 15 for MSB location.
   @parm uint32 | value | Value to set. It can be value 0 or 1.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue TBLDRV_EINVALIDINPUT | Invalid input parameter.
   @comm
   Set a bit of a specified register to 1 or 0. It is 16-bits system of RTL8366s chip.
*/
int32_t rtl8368s_set_asic_reg_bit(uint32_t reg, uint32_t bit, uint32_t value)
{
  uint32_t regData;
  int32_t retVal;

  if(bit >= RTL8368S_REGBITLENGTH)
    return ERRNO_INPUT;

  retVal = smi_read(reg, &regData);
  if (retVal != 0) return ERRNO_SMI;

  if (value)
    regData = regData | (1<<bit);
  else
    regData = regData & (~(1<<bit));

  retVal = smi_write(reg, regData);
  if (retVal != 0) return ERRNO_SMI;

  return 0;
}

/*
   @func int32_t | rtl8368s_set_asic_reg_bits | Set bits value of a specified register.
   @parm uint32_t | reg | Register's address.
   @parm uint32_t | bits | Bits mask for setting. 
   @parm uint32_t | value | Bits value for setting. 
   Value of bits will be set with mapping mask bit is 1.   
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue TBLDRV_EINVALIDINPUT | Invalid input parameter. 
   @comm
   Set bits of a specified register to value. 
   Both bits and value are be treated as bit-mask.
*/
int32_t rtl8368s_set_asic_reg_bits(uint32_t reg, uint32_t bits, uint32_t value)
{
  uint32_t regData = 0;
  int32_t retVal = 0;

  if (bits >= (1 << RTL8368S_REGBITLENGTH))
    return ERRNO_INPUT;

  retVal = smi_read(reg, &regData);
  if (retVal != 0)
    return ERRNO_SMI;

  regData = regData & (~bits);
  regData = regData | (value & bits);

  retVal = smi_write(reg, regData);
  if (retVal != 0)
    return ERRNO_SMI;

  return 0;
}

/*
   @func int32_t | rtl8368s_get_asic_reg | Get content of register.
   @parm uint32_t | reg | Register's address.
   @parm uint32* | value | Value of register.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @comm
   Value 0x0000 will be returned for ASIC un-mapping address.
*/
int32_t rtl8368s_get_asic_reg(uint32_t reg, uint32_t *value)
{
  uint32_t regData = 0;
  int32_t retVal = 0;

  retVal = smi_read(reg, &regData);
  if (retVal != 0)
    return ERRNO_SMI;

  *value = regData;

  return 0;
}

/*
   @func int32_t | rtl8368s_set_asic_reg | Set content of asic register.
   @parm uint32_t | reg | Register's address.
   @parm uint32_t | value | Value setting to register.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @comm
   The value will be set to ASIC mapping address only 
   and it is always return 0 while setting un-mapping address registers.
*/
int32_t rtl8368s_set_asic_reg(uint32_t reg, uint32_t value)
{
  int32_t retVal = 0;

  retVal = smi_write(reg, value);
  if (retVal != 0)
    return ERRNO_SMI;

  return 0;
}

/*=======================================================================
 * Port property APIs
 *========================================================================*/
/*
   @func int32_t | rtl8368s_get_asic_port_link_state | Get a specific port's link state.
   @parm enum PORTID | port | Physical Port number.
   @parm enum PORTLINKSPEED  * | speed | current link speed,
   SPD_10M: 10Mbps,
   SPD_100M: 100Mbps,
   SPD_1000M: 1000Mbps.
   @parm enum PORTLINKDUPLEXMODE * | duplex | current duplex status,
   FULL_DUPLEX: full duplex,
   HALF_DUPLEX: half duplex.
   @parm uint32_t  * | link | Link status, 1: link up, 0: link down.
   @parm uint32_t  * | rxPause | The pause frame response ability, 1: active, 0: inactive.
   @parm uint32_t  * | txPause | The pause frame transmit ability, 1: active, 0: inactive.
   @parm uint32_t  * | nWay | N-way function, 1: enable, 0: disable.
   @rvalue 0 | Success.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @comm
   The API can get a specific port's link state information including link up/down,
   link speed, auto negotiation enabled/disabled, Tx in active/inactive and Rx in
   active/inactive.
*/
int32_t rtl8368s_get_asic_port_link_state(enum PORTID port, enum PORTLINKSPEED *speed, 
                                          enum PORTLINKDUPLEXMODE *duplex, uint32_t *link, 
                                          uint32_t *txPause, uint32_t *rxPause, 
                                          uint32_t *nWay)
{
  int32_t retVal = 0;
  uint32_t RegData = 0;

  if (port < PORT0 || port >= PORT_MAX)
    return ERRNO_INPUT;

  retVal = rtl8368s_get_asic_reg(RTL8368S_PORT_LINK_STATUS_BASE + (port >> 1), &RegData);
  if (0 != retVal)
    return -1;

  if (port & 0x1)
    RegData = RegData >> 8;

  *speed = (RegData & RTL8368S_PORT_STATUS_SPEED_MSK) >> RTL8368S_PORT_STATUS_SPEED_OFF;
  *duplex = (RegData & RTL8368S_PORT_STATUS_DUPLEX_MSK) >> RTL8368S_PORT_STATUS_DUPLEX_OFF;
  *link = (RegData & RTL8368S_PORT_STATUS_LINK_MSK) >> RTL8368S_PORT_STATUS_LINK_OFF;
  *txPause = (RegData & RTL8368S_PORT_STATUS_TXPAUSE_MSK) >> RTL8368S_PORT_STATUS_TXPAUSE_OFF;
  *rxPause = (RegData & RTL8368S_PORT_STATUS_RXPAUSE_MSK) >> RTL8368S_PORT_STATUS_RXPAUSE_OFF;
  *nWay = (RegData & RTL8368S_PORT_STATUS_AN_MSK) >> RTL8368S_PORT_STATUS_AN_OFF;

  return 0;
}


/*=======================================================================
 *  C-VLAN APIs
 *========================================================================*/
static void _rtl8368s_VlanStUser2Smi(rtl8368s_user_vlan4kentry *VlanUser, 
                                     rtl8368s_vlan4kentry *VlanSmi)
{
  VlanSmi->vid    = VlanUser->vid;
  VlanSmi->untag  = VlanUser->untag;
  VlanSmi->member = VlanUser->member;
  VlanSmi->fid    = VlanUser->fid;
}

static void _rtl8368s_VlanStSmi2User(rtl8368s_user_vlan4kentry *VlanUser, 
                                     rtl8368s_vlan4kentry *VlanSmi)
{
  VlanUser->vid   = VlanSmi->vid;
  VlanUser->untag = VlanSmi->untag;
  VlanUser->member= VlanSmi->member;
  VlanUser->fid   = VlanSmi->fid;
}

/*
   @func int32_t | rtl8368s_set_asic_vlan | Set VLAN enable function.
   @parm uint32_t | enabled | VLAN enable function usage 1: enabled, 0: disabled.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @comm
   ASIC will parse frame type/length if VLAN function usage is enabled. 
   In 802.1q spec. ,the Type/Length of C-tag is 0x8100. System will decide
   802.1q VID of received frame from C-tag, Protocol-and-Port based VLAN and Port based VLAN. 
   This setting will impact on VLAN ingress, VLAN egress
   and 802.1q priority selection.
*/
int32_t rtl8368s_set_asic_vlan(uint32_t enabled)
{
  uint32_t retVal;

  retVal = rtl8368s_set_asic_reg_bit(RTL8368S_SWITCH_GLOBAL_CTRL_REG, RTL8368S_EN_VLAN_OFF,
                                     enabled);

  return retVal;
}

/*
   @func int32_t | rtl8368s_get_asic_vlan | Get VLAN enable function configuration.
   @parm uint32_t* | enabled | VLAN enable function usage 1: enabled, 0: disabled.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @comm
   The API can get usage of asic VLAN enable configuration.
*/
int32_t rtl8368s_get_asic_vlan(uint32_t *enabled)
{
  uint32_t retVal;
  uint32_t regData;

  retVal = rtl8368s_get_asic_reg(RTL8368S_SWITCH_GLOBAL_CTRL_REG, &regData);
  if(retVal !=  0)
    return retVal;

  regData = regData & (1 << RTL8368S_EN_VLAN_OFF);
  if(regData)
    *enabled = 1;
  else
    *enabled = 0;

  return 0;
}

/*
   @func int32_t | rtl8368s_set_asic_vlan_4k_tb_usage | 
         Set 4k VLAN table usage configuration.
   @parm uint32_t | enabled | 4k VLAN table usage 1: enabled, 0: disabled.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @comm
   Each VLAN entry contains member port, untag set and FID (support 8 SVL/IVL filtering database) 
   information. While VLAN function of system is enabled and 4k VLAN table is enabled, 
   system will decide each receiving frame's VID. VLAN ingress and VLAN egress function will
   reference member port of mapped VID entry in 4k table. Without 4k VLAN table usage, 
   there are 16 VLAN memeber configurations to support VLAN enabled reference.
*/
int32_t rtl8368s_set_asic_vlan_4k_tb_usage(uint32_t enabled)
{
  uint32_t retVal;

  retVal = rtl8368s_set_asic_reg_bit(RTL8368S_SWITCH_GLOBAL_CTRL_REG, 
                                     RTL8368S_EN_VLAN_4KTB_OFF, enabled);

  return retVal;
}

/*
   @func int32_t | rtl8368s_get_asic_vlan_4k_tb_usage | 
         Get 4k VLAN table usage configuration.
   @parm uint32_t* | enabled | 4k VLAN table usage 1: enabled, 0: disabled.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @comm
   The API can get 4K VLAN table usage.
*/
int32_t rtl8368s_get_asic_vlan_4k_tb_usage(uint32_t *enabled)
{
  uint32_t retVal;
  uint32_t regData;

  retVal = rtl8368s_get_asic_reg(RTL8368S_SWITCH_GLOBAL_CTRL_REG, &regData);

  if(retVal !=  0)
    return retVal;

  if(regData & (1 << RTL8368S_EN_VLAN_4KTB_OFF))
    *enabled = 1;
  else
    *enabled = 0;

  return 0;
}


/*
   @func int32_t | rtl8368s_set_asic_vlan_member_config | 
         Set 16 VLAN member configurations.
   @parm uint32_t | index | VLAN member configuration index (0~15).
   @parm rtl8368s_vlanconfig* | vlanmemberconf | VLAN member configuration. 
         It contained VID, priority, member set, untag set and FID fields.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @rvalue ERRNO_VLAN_INVALIDFID | Invalid FID (0~7).
   @rvalue ERRNO_VLAN_INVALIDPRIORITY | Invalid VLAN priority (0~7).
   @rvalue ERRNO_VLAN_INVALIDPORTMSK | Invalid port mask (0x00~0x3F).
   @rvalue ERRNO_VLAN_INVALIDVID | Invalid VID parameter (0~4095).
   @rvalue ERRNO_VLAN_VIDX | Invalid VLAN member configuration index (0~15).
   @comm
   VLAN ingress and egress will reference these 16 configurations 
   while system VLAN function is enabled without 4k VLAN table usage. 
   Port based, Protocol-and-Port based VLAN and 802.1x guest VLAN functions retrieved 
   VLAN information from these 16 member configurations too. 
   Only VID will be referenced while 4k VLAN table is enabled. 
   It means that member set, untag set and FID need to be retrieved from 4k mapped VID entry.
*/
int32_t rtl8368s_set_asic_vlan_member_config(uint32_t index, rtl8368s_vlanconfig vlanmconf)
{
  uint32_t retVal;
  uint32_t regAddr;
  uint32_t regData;
  uint16_t *tableAddr;

  if(index > RTL8368S_VLANMCIDXMAX)
    return ERRNO_CVIDX;

  regAddr = RTL8368S_VLAN_MEMCONF_BASE + (index*3);

  tableAddr = (uint16_t*)&vlanmconf;

  regData = *tableAddr;
  retVal = rtl8368s_set_asic_reg(regAddr, regData);
  if(retVal != 0)
    return retVal;

  regAddr = RTL8368S_VLAN_MEMCONF_BASE + 1 + (index*3);

  tableAddr ++;
  regData = *tableAddr;
  retVal = rtl8368s_set_asic_reg(regAddr, regData);
  if(retVal != 0)
    return retVal;

  return 0;
}

/*
   @func int32_t | rtl8368s_get_asic_vlan_member_config | 
         Get 16 VLAN member configurations.
   @parm uint32_t | index | VLAN member configuration index (0~15).
   @parm rtl8368s_vlanconfig* | vlanmemberconf | VLAN member configuration. 
         It contained VID, priority, member set, untag set and FID fields.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @rvalue ERRNO_VLAN_VIDX | Invalid VLAN member configuration index (0~15).
   @comm
   The API can get 16 VLAN member configuration.
*/
int32_t rtl8368s_get_asic_vlan_member_config(uint32_t index, rtl8368s_vlanconfig *vlanmconf)
{
  uint32_t retVal;
  uint32_t regAddr;
  uint32_t regData;
  uint16_t* tableAddr;

  if(index > RTL8368S_VLANMCIDXMAX)
    return ERRNO_CVIDX;

  tableAddr = (uint16_t*)vlanmconf;

  regAddr = RTL8368S_VLAN_MEMCONF_BASE + (index*3);

  retVal = rtl8368s_get_asic_reg(regAddr, &regData);
  if(retVal != 0)
    return retVal;

  *tableAddr = regData;
  tableAddr ++;

  regAddr = RTL8368S_VLAN_MEMCONF_BASE + 1 + (index*3);
  retVal = rtl8368s_get_asic_reg(regAddr, &regData);
  if(retVal != 0)
    return retVal;

  *tableAddr = regData;
  tableAddr ++;

  regAddr = RTL8368S_VLAN_MEMCONF_BASE + 2 + (index*3);
  retVal = rtl8368s_get_asic_reg(regAddr, &regData);
  if(retVal != 0)
    return retVal;

  *tableAddr = regData;

  return 0;
}

/*
   @func int32_t | rtl8368s_set_asic_vlan_4k_entry | Set VID mapped entry to 4K VLAN table.
   @parm rtl8368s_vlan4kentry* | vlan4kEntry | VLAN entry seting for 4K table. 
         There is VID field in entry structure and entry is directly mapping to 
         4K table location(1 to 1).
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @rvalue ERRNO_VLAN_INVALIDFID | Invalid FID (0~7).
   @rvalue ERRNO_VLAN_INVALIDPORTMSK | Invalid port mask (0x00~0x3F).
   @rvalue ERRNO_VLAN_INVALIDVID | Invalid VID parameter (0~4095).
   @comm
   VID field of C-tag is 12-bits and available VID range is 0~4095. 
   In 802.1q spec. , null VID (0x000) means tag header contain priority information only and 
   VID 0xFFF is reserved for implementtation usage. 
   But ASIC still retrieved these VID entries in 4K VLAN table if VID is decided from 16
   member configurations. It has no available VID 0x000 and 0xFFF from C-tag. 
   ASIC will retrieve these two non-standard VIDs (0x000 and 0xFFF) from member configuration 
   indirectly referenced by Port based, Protocol-and-Port based VLAN and 802.1x functions.
*/
int32_t rtl8368s_set_asic_vlan_4k_entry(rtl8368s_user_vlan4kentry vlan4kEntry)
{
  uint32_t retVal;
  uint32_t regData;
  uint16_t *tableAddr;
  uint32_t i;
  rtl8368s_vlan4kentry smiVlan4kentry;

  memset(&smiVlan4kentry, 0x0, sizeof(rtl8368s_vlan4kentry));

  _rtl8368s_VlanStUser2Smi(&vlan4kEntry, &smiVlan4kentry);

  tableAddr = (uint16_t*)&smiVlan4kentry;
  regData = *tableAddr;
  retVal = rtl8368s_set_asic_reg(RTL8368S_VLAN_TABLE_WRITE_BASE, regData);
  if(retVal != 0)
    return retVal;

  tableAddr ++;
  regData = *tableAddr;
  retVal = rtl8368s_set_asic_reg(RTL8368S_VLAN_TABLE_WRITE_BASE+1, regData);
  if(retVal != 0)
    return retVal;

  tableAddr ++;
  regData = *tableAddr;
  retVal = rtl8368s_set_asic_reg(RTL8368S_VLAN_TABLE_WRITE_BASE+2, regData);
  if(retVal != 0)
    return retVal;

  /*write table access Control word*/
  retVal = rtl8368s_set_asic_reg(RTL8368S_TABLE_ACCESS_CTRL_REG,
                                 RTL8368S_TABLE_VLAN_WRITE_CTRL);
  if(retVal != 0)
    return retVal;

  /*check ASIC command*/
  i=0;
  while(i < RTL8368S_TBL_CMD_CHECK_COUNTER)
  {
    retVal = rtl8368s_get_asic_reg(RTL8368S_TABLE_ACCESS_CTRL_REG, &regData);
    if(retVal != 0)
      return retVal;

    if(regData & RTL8368S_TABLE_ACCESS_CMD_MSK)
      {
        i++;
        if(i == RTL8368S_TBL_CMD_CHECK_COUNTER)
          return -1;
      }
    else
      break;
  }

  return 0;
}

/*
   @func int32 | rtl8368s_get_asic_vlan_4k_entry | Get VID mapped entry to 4K VLAN table.
   @parm rtl8368s_user_vlan4kentry* | vlan4kEntry | VLAN entry seting for 4K table. 
         There is VID field in entry structure and entry is directly mapping to 
         4K table location(1 to 1).
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @rvalue ERRNO_VLAN_INVALIDVID | Invalid VID parameter (0~4095).
   @comm
   The API can get entry of 4k VLAN table. Software must prepare the retrieving VID first 
   at writing data and used control word to access desired VLAN entry.
*/
int32_t rtl8368s_get_asic_vlan_4k_entry(rtl8368s_user_vlan4kentry *vlan4kEntry)
{
  uint32_t retVal;
  uint32_t regData;
  uint32_t vid;
  uint32_t i;
  uint16_t *tableAddr;
  rtl8368s_vlan4kentry smiVlan4kentry;

  memset(&smiVlan4kentry, 0x0, sizeof(rtl8368s_vlan4kentry));

  _rtl8368s_VlanStUser2Smi(vlan4kEntry, &smiVlan4kentry);

  vid = smiVlan4kentry.vid;
  if(vid > RTL8368S_VIDMAX)
    return ERRNO_VID;

  tableAddr = (uint16_t*)&smiVlan4kentry;

  /*write VID first*/
  regData = *tableAddr;
  retVal = rtl8368s_set_asic_reg(RTL8368S_VLAN_TABLE_WRITE_BASE, regData);
  if(retVal != 0)
    return retVal;

  /*write table access Control word*/
  retVal = rtl8368s_set_asic_reg(RTL8368S_TABLE_ACCESS_CTRL_REG, 
                                 RTL8368S_TABLE_VLAN_READ_CTRL);
  if(retVal != 0)
    return retVal;

  /*check ASIC command*/
  i=0;
  while(i < RTL8368S_TBL_CMD_CHECK_COUNTER)
  {
    retVal = rtl8368s_get_asic_reg(RTL8368S_TABLE_ACCESS_CTRL_REG, &regData);
    if(retVal != 0)
      return retVal;

    if(regData & RTL8368S_TABLE_ACCESS_CMD_MSK)
      {
        i++;
        if(i == RTL8368S_TBL_CMD_CHECK_COUNTER)
          return -1;
      }
    else
      break;
  }

  retVal = rtl8368s_get_asic_reg(RTL8368S_VLAN_TABLE_READ_BASE, &regData);
  if(retVal != 0)
    return retVal;

  *tableAddr = regData;
  tableAddr ++;

  retVal = rtl8368s_get_asic_reg(RTL8368S_VLAN_TABLE_READ_BASE+1, &regData);
  if(retVal != 0)
    return retVal;

  *tableAddr = regData;
  tableAddr ++;

  retVal = rtl8368s_get_asic_reg(RTL8368S_VLAN_TABLE_READ_BASE+2, &regData);
  if(retVal != 0)
    return retVal;

  *tableAddr = regData;

  _rtl8368s_VlanStSmi2User(vlan4kEntry, &smiVlan4kentry);

  vlan4kEntry->vid = vid;

  return 0;
}

/*
   @func int32_t | rtl8368s_set_asic_vlan_accept_tagged_only | 
         Set ASIC permit C-tagged frames only.
   @parm enum PORTID | port | Physical port number (0~5).
   @parm uint32_t | enabled | ASIC permit C-tagged frame only 1: enabled, 0: disabled
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_PORT_NUM | Invalid port number.
   @comm
   Ingress rule for acceptable frame types control. If the input parameter is set to 'enabled', 
   any received frames carrying no VID(i.e., Untagged
   frames or Priority-Tagged Frames) will be dropped. If the parameter is set to 'disabled' 
   all incoming Priority-Tagged and Untagged frames are associated with a VLAN by 
   the ingress rule on the received port. 
   Filtering function is actived while system VLAN function is enabled.
*/
int32_t rtl8368s_set_asic_vlan_accept_tagged_only(enum PORTID port, uint32_t enabled)
{
  int32_t retVal;

  if(port >= PORT_MAX)
    return ERRNO_PORT_NUM;

  retVal = rtl8368s_set_asic_reg_bit(RTL8368S_VLAN_INGRESS_CTRL_1_REG, port, enabled);

  return retVal;
}

/*
   @func int32_t | rtl8368s_get_asic_vlan_accept_tagged_only | 
         Get ASIC permit C-tagged frames setting.
   @parm enum PORTID | port | Physical port number (0~5).
   @parm uint32* | enabled | ASIC permit C-tagged frame only 1: enabled, 0: disabled
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_PORT_NUM | Invalid port number.
   @comm
   The API can get C-tagged frame control setting.
*/
int32_t rtl8368s_get_asic_vlan_accept_tagged_only(enum PORTID port, uint32_t* enabled)
{
  int32_t retVal;
  uint32_t regData;

  if(port >= PORT_MAX)
    return ERRNO_PORT_NUM;

  retVal = rtl8368s_get_asic_reg(RTL8368S_VLAN_INGRESS_CTRL_1_REG, &regData);
  if(retVal != 0)
    return retVal;

  if(regData & (1 << port))
    *enabled = 1;
  else
    *enabled = 0;

  return 0;
}

/*
   @func int32_t | rtl8368s_set_asic_vlan_drop_tagged_packets | 
         Set dropping C-tagged frame ability.
   @parm enum PORTID | port | Physical port number (0~5).
   @parm uint32_t | enabled | ASIC permit  un-tagged frame only 1: enabled, 0: disabled.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_PORT_NUM | Invalid port number.
   @common
   Ingress rule for acceptable frame types control. If the input parameter is set to 'enabled', 
   any received frames carrying C-tag (with VID is not 0x000)
   will be dropped. Filtering function is actived while system VLAN function is enabled.
*/
int32_t rtl8368s_set_asic_vlan_drop_tagged_packets(enum PORTID port, uint32_t enabled)
{
  int32_t retVal;
  uint32_t bitNo;

  if(port >= PORT_MAX)
    return ERRNO_PORT_NUM;

  bitNo = port + PORT_MAX;
  retVal = rtl8368s_set_asic_reg_bit(RTL8368S_VLAN_INGRESS_CTRL_1_REG, bitNo, enabled);

  return retVal;
}

/*
   @func int32_t | rtl8368s_set_asic_vlan_ingress_filtering | Set VLAN ingress function.
   @parm enum PORTID | port | Physical port number (0~7).
   @parm uint32_t | enabled | VLAN ingress function setting 1: enabled, 0: disabled
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_PORT_NUM | Invalid port number.
   @common
   If enable VLAN ingress function, all frames received on a port whose 
   VLAN classification does not include that port in its member set will be discarded.
   Filtering function is actived while system VLAN function is enabled.
*/
int32_t rtl8368s_set_asic_vlan_ingress_filtering(enum PORTID port, uint32_t enabled)
{
  int32_t retVal;

  if(port >= PORT_MAX)
    return ERRNO_PORT_NUM;

  retVal = rtl8368s_set_asic_reg_bit(RTL8368S_VLAN_INGRESS_CTRL_2_REG, port, enabled);

  return retVal;
}

/*
   @func int32_t | rtl8368s_set_asic_vlan_port_based_vid | 
         Set port based VID which is indexed to 16 VLAN member configurations.
   @parm enum PORTID | port | Physical port number (0~5).
   @parm uint32_t | index | Index to VLAN member configuration (0~15).
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_CVIDX | Invalid VLAN member configuration index (0~15).
   @rvalue ERRNO_PORT_NUM | Invalid port number.
   @comm
   In port based VLAN, untagged packets recieved by port N are forwarded to a VLAN according 
   to the setting VID of port N. Usage of VLAN 4k table is enabled
   and there are only VID and 802.1q priority retrieved from 16 member configurations. 
   Member set, untag set and FID of port based VLAN are be retrieved from 4K mapped VLAN entry.
*/
int32_t rtl8368s_set_asic_vlan_port_based_vid(enum PORTID port, uint32_t index)
{
  int32_t  retVal;
  uint32_t regAddr;
  uint32_t regData;
  uint32_t regBits;

  /* bits mapping to port vlan control register of port n */
  const uint16_t bits[8] = { 0x000F,0x00F0,0x0F00,0xF000,0x000F,0x00F0,0x0F00,0xF000 };
  /* bits offset to port vlan control register of port n */
  const uint16_t bitOff[8] = { 0,4,8,12,0,4,8,12 };
  /* address offset to port vlan control register of port n */
  const uint16_t addrOff[8] = { 0,0,0,0,1,1,1,1 };

  if(port >= PORT_MAX)
    return ERRNO_PORT_NUM;

  if(index > RTL8368S_VLANMCIDXMAX)
    return ERRNO_CVIDX;

  regAddr = RTL8368S_PORT_VLAN_CTRL_BASE + addrOff[port];

  regBits = bits[port];

  regData = (index << bitOff[port]) & regBits;

  retVal = rtl8368s_set_asic_reg_bits(regAddr, regBits, regData);

  return retVal;
}

/*
   @func int32_t | rtl8368s_get_asic_vlan_port_based_vid | 
         Get port based VID which is indexed to 16 VLAN member configurations.
   @parm enum PORTID | port | Physical port number (0~5).
   @parm uint32_t* | index | Index to VLAN member configuration (0~15).
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_PORT_NUM | Invalid port number.
   @comm
   The API can access port based VLAN index indirectly retrieving VID 
   and priority from 16 member configuration for a specific port.
*/
int32_t rtl8368s_get_asic_vlan_port_based_vid(enum PORTID port, uint32_t* index)
{
  int32_t  retVal;
  uint32_t regAddr;
  uint32_t regData;

  /* bits mapping to port vlan control register of port n */
  const uint16_t bits[8] = { 0x000F,0x00F0,0x0F00,0xF000,0x000F,0x00F0,0x0F00,0xF000 };
  /* bits offset to port vlan control register of port n */
  const uint16_t bitOff[8] = { 0,4,8,12,0,4,8,12 };
  /* address offset to port vlan control register of port n */
  const uint16_t addrOff[8] = { 0,0,0,0,1,1,1,1 };

  if(port >= PORT_MAX)
    return ERRNO_PORT_NUM;

  regAddr = RTL8368S_PORT_VLAN_CTRL_BASE + addrOff[port];

  retVal = rtl8368s_get_asic_reg(regAddr, &regData);
  if(retVal != 0)
    return retVal;

  *index = (regData & bits[port]) >> bitOff[port];
  return retVal;
}

/*
   @func int32_t | rtl8368s_set_asic_vlan_keep_ctag_format | 
         Set ASIC keep receiving tag/untag format in ingress port..
   @parm enum PORTID | ingressport | Ingress port number (0~7).
   @parm uint32_t | portmask | Egress port mask (0x00-0xFF).
   @rvalue 0 | Success.
   @rvalue ERRNO_SMIERROR | SMI access error.
   @rvalue ERRNO_INPUT | Invalid port number.
   @comm
   Output frame from ASIC will not be tagged C-tag or untagged C-tag 
   if ingress port was set enable keep-Ctag-format function.
   Receiving frame with untag format will be output with untag format, 
   priority-tag frame will be output as priority-tag
   frame and c-tagged frame will be output as original c-tagged frame. 
   But if 802.1q remarking function was enabled in the
   egress port, then the original priroity of frame will be impacted.
*/
int32_t rtl8368s_set_asic_vlan_keep_ctag_format(enum PORTID ingressport, uint32_t portmask)
{
  int32_t retVal;
  uint32_t regData;

  if(ingressport >= PORT_MAX)
    return ERRNO_INPUT;
  if (portmask > RTL8368S_PORTMASK)
    return ERRNO_PORT_MSK;

  retVal = rtl8368s_get_asic_reg(RTL8368S_KEEP_FORMAT_BASE+(ingressport>>1), &regData);
  regData = (regData&(RTL8368S_KEEP_FORMAT_MSK << (RTL8368S_KEEP_PORT_OFF*((ingressport+1)%2)))) |
            ((portmask&RTL8368S_KEEP_FORMAT_MSK) << (RTL8368S_KEEP_PORT_OFF*(ingressport%2)));
  retVal = rtl8368s_set_asic_reg(RTL8368S_KEEP_FORMAT_BASE+(ingressport>>1), regData);

  return retVal;
}

/*
   @func int32_t | rtl8368s_get_asic_vlan_keep_ctag_format | 
         Set ASIC keep receiving tag/untag format in ingress port..
   @parm enum PORTID | ingressport | Ingress port number (0~7).
   @parm uint32_t* | portmask | Egress port mask with original tag/untag format
   @rvalue 0 | Success.
   @rvalue ERRNO_SMIERROR | SMI access error.
   @rvalue ERRNO_VLAN_INVALIDPORT | Invalid port number.
   @comm
   The API can be used to get keep format function of receiving port.
*/
int32_t rtl8368s_get_asic_vlan_keep_ctag_format(enum PORTID ingressport, uint32_t *portmask)
{
  int32_t retVal;
  uint32_t regData;

  if(ingressport >= PORT_MAX)
    return ERRNO_INPUT;

  retVal = rtl8368s_get_asic_reg(RTL8368S_KEEP_FORMAT_BASE + (ingressport >> 1), &regData);
  if(retVal != 0)
    return retVal;

  if (ingressport%2 == 1)
    *portmask = (regData & 0xFF00) >> 8;
  else
    *portmask = (regData & 0x00FF);

  return 0;
}


/*=======================================================================
 *PHY APIs 
 *========================================================================*/
/*
   @func int32_t | rtl8368s_setAsicPHYReg | Set PHY registers .
   @parm uint32_t | phyNo | PHY number (0~3).
   @parm uint32_t | page | PHY page (0~7).
   @parm uint32_t | addr | PHY address (0~31).
   @parm uint32_t | data | Writing data.
   @rvalue 0 | Success.
   @rvalue -1 | invalid parameter.
   @comm
*/
int32_t rtl8368s_set_asic_phy_regs(uint32_t phyNo, uint32_t page, uint32_t addr, 
                                   uint32_t data)
{
  int32_t   retVal = 0, phySmiAddr = 0;
  uint32_t  dummy = 0;

  if (phyNo > RTL8368S_PHY_NO_MAX)
    return ERROR_PHY_INVALIDPHYNO;

  if (page > RTL8368S_PHY_PAGE_MAX)
    return ERROR_PHY_INVALIDPHYPAGE;

  if (addr > RTL8368S_PHY_REG_MAX)
    return ERROR_PHY_INVALIDREG;

  /* Dummy read before write PHY register */
  retVal = rtl8368s_get_asic_phy_regs(phyNo, page, addr, &dummy);
  if (retVal != 0) 
    return retVal;

  retVal = rtl8368s_set_asic_reg(RTL8368S_PHY_ACCESS_CTRL_REG, RTL8368S_PHY_CTRL_WRITE);
  if (retVal !=  0) 
    return retVal;

  phySmiAddr = 0x8000 |
               (1 << (phyNo +RTL8368S_PHY_NO_OFFSET)) |
               ((page << RTL8368S_PHY_PAGE_OFFSET) & RTL8368S_PHY_PAGE_MASK) |
               (addr & RTL8368S_PHY_REG_MASK);

  retVal = rtl8368s_set_asic_reg(phySmiAddr, data);
  if (retVal != 0) 
    return retVal;

  return 0;	
}

/*
   @func int32_t | rtl8368s_getAsicPHYReg | get PHY registers .
   @parm uint32_t | phyNo | PHY number (0~3).
   @parm uint32_t | page | PHY page (0~7).
   @parm uint32_t | addr | PHY address (0~31).
   @parm uint32* | data | Read data.
   @rvalue 0 | Success.
   @rvalue -1 | invalid parameter.
   @comm
*/
int32_t rtl8368s_get_asic_phy_regs(uint32_t phyNo, uint32_t page, uint32_t addr, 
                                   uint32_t *data)
{
  int32_t retVal = 0, phySmiAddr = 0;

  if (phyNo > RTL8368S_PHY_NO_MAX)
    return ERROR_PHY_INVALIDPHYNO;

  if (page > RTL8368S_PHY_PAGE_MAX)
    return ERROR_PHY_INVALIDPHYPAGE;

  if (addr > RTL8368S_PHY_REG_MAX)
    return ERROR_PHY_INVALIDREG;


  retVal = rtl8368s_set_asic_reg(RTL8368S_PHY_ACCESS_CTRL_REG, RTL8368S_PHY_CTRL_READ);
  if (retVal !=  0) 
    return retVal;

  phySmiAddr = 0x8000 | 
               (1 << (phyNo +RTL8368S_PHY_NO_OFFSET)) |
               ((page << RTL8368S_PHY_PAGE_OFFSET) & RTL8368S_PHY_PAGE_MASK) |
               (addr & RTL8368S_PHY_REG_MASK);

  retVal = rtl8368s_set_asic_reg(phySmiAddr, 0);
  if (retVal != 0) 
    return retVal;

  //For Verify PHY Read issue, mark this place
  retVal = rtl8368s_set_asic_reg(phySmiAddr, 0);
  if (retVal != 0) 
    return retVal;

  retVal = rtl8368s_get_asic_reg(RTL8368S_PHY_ACCESS_DATA_REG,data);
  if (retVal != 0)
    return retVal;

  return 0;	
}

/*=======================================================================
 *  Port Mirroring APIs
 *========================================================================*/
/*
   @func int32_t | rtl8368s_set_asic_port_learn_disable | Disable source learning per port
   @parm enum PORTID | port | Physical port number.
   @parm uint32_t | disable | disable port mask
   @rvalue 0 | Success.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @comm
   This API is setted to disable SA learning function per port
*/
int32_t rtl8368s_set_asic_port_learn_disable(enum PORTID port, uint32_t disable)
{
  uint32_t retVal;

  if(port >= PORT_MAX)
    return ERRNO_PORT_NUM;

  retVal = rtl8368s_set_asic_reg_bit(RTL8368S_PORT_LEARNDIS_CTRL_REG, port, disable);

  return retVal;
}

/*
   @func int32_t | rtl8368s_get_asic_port_learn_disable | Disable source learning per port
   @parm enum PORTID | port | Physical port number.
   @parm uint32_t* | disable | disable port mask
   @rvalue 0 | Success.
   @rvalue ERRNO_INPUT | Invalid input parameter.
   @comm
   If the API can be used to get status of SA learning function usage of ports
*/
int32_t rtl8368s_get_asic_port_learn_disable(enum PORTID port, uint32_t* disable)
{
  uint32_t retVal;
  uint32_t regData;

  retVal = rtl8368s_get_asic_reg(RTL8368S_PORT_LEARNDIS_CTRL_REG, &regData);
  if(retVal !=  0)
    return retVal;

  if(regData & (1 << port))
    *disable = 1;
  else
    *disable = 0;

  return 0;
}

/*=======================================================================
 * ASIC DRIVER API: Priority Assignment Control Register
 *========================================================================*/
/*
   @func int32 | rtl8368s_setAsicPortPriority | Set port based priority.
   @parm enum PORTID | port | The port number.
   @parm enum PRIORITYVALUE | priority | Priority value.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_PORT_NUM | Invalid port number.
   @rvalue ERRNO_QOS_INVALIDPRIORITY | Invalid priority.
   @comm
   The API can set a 3-bit priority value of the specified port.
*/
int32_t rtl8368s_set_asic_port_priority(enum PORTID port, enum PRIORITYVALUE priority)
{
  uint32_t retVal;
  uint32_t regAddr;
  uint32_t regData;
  uint32_t regBits;

  /* Invalid input parameter */
  if (port >= PORT_MAX)
    return ERRNO_PORT_NUM;

  /* Invalid input parameter */
  if ((priority < PRI0) || (priority > PRI7))
    return ERRNO_PRIORITY;

  if(port < PORT5)
    {
      regAddr = RTL8368S_PBP_BASE;
      regBits = RTL8368S_PBP_PRIORITY_MSK<<(RTL8368S_PBP_PRIORITY_BITS*port);
      regData = (priority << (RTL8368S_PBP_PRIORITY_BITS*port)) & regBits;
    }
  else
    {
      regAddr = RTL8368S_PBP_BASE + 1;
      regBits = RTL8368S_PBP_PRIORITY_MSK<<(RTL8368S_PBP_PRIORITY_BITS*(port-5));
      regData = (priority << (RTL8368S_PBP_PRIORITY_BITS*(port-5))) & regBits;
    }

  retVal = rtl8368s_set_asic_reg_bits(regAddr,regBits,regData);

  return retVal;
}

/*
   @func int32 | rtl8368s_set_asic_dot1q_absolutely_priority | Set 802.1Q absolutely priority.
   @parm enum PRIORITYVALUE | srcpriority | Priority value.
   @parm enum PRIORITYVALUE | priority | Absolute priority value.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_PRIORITY | Invalid priority.
   @comm
   The API can set a 3-bit absolutely priority of the specified 802.1Q priority.
*/
int32_t rtl8368s_set_asic_dot1q_absolutely_priority(enum PRIORITYVALUE srcpriority, enum PRIORITYVALUE priority)
{
  uint32_t retVal, regValue;

  /* Invalid input parameter */
  if ((srcpriority < PRI0) || (srcpriority > PRI7) || (priority < PRI0) || (priority > PRI7))
    return ERRNO_PRIORITY;

  /* Get register value */
  retVal = rtl8368s_get_asic_reg(RTL8368S_1QMCR_REG(srcpriority), &regValue);
  if (retVal != 0)
    return retVal;

  /* Set register value */
  retVal = rtl8368s_set_asic_reg(RTL8368S_1QMCR_REG(srcpriority), 
      (regValue & ~(RTL8368S_1QMCR_MSK(srcpriority))) | (priority << RTL8368S_1QMCR_OFFSET(srcpriority)));
  if (retVal != 0)
    return retVal;

  return 0;
}

/*
   @func int32 | rtl8368s_get_asic_dot1q_absolutely_priority | Get 802.1Q absolutely priority.
   @parm enum PRIORITYVALUE | srcpriority | Priority value.
   @parm enum PRIORITYVALUE* | pPriority | It will return the absolute priority value.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @rvalue ERRNO_QOS_INVALIDPRIORITY | Invalid priority.
   @comm
   The API will return the absolutely priority value of the specified 802.1Q priority.
*/
int32_t rtl8368s_get_asic_dot1q_absolutely_priority(enum PRIORITYVALUE srcpriority, enum PRIORITYVALUE *pPriority)
{
  int32_t retVal, regValue;

  /* Invalid input parameter */
  if ((srcpriority < PRI0) || (srcpriority > PRI7))
    return ERRNO_PRIORITY;

  /* Get register value */
  retVal = rtl8368s_get_asic_reg(RTL8368S_1QMCR_REG(srcpriority), &regValue);
  if (retVal != 0)
    return retVal;

  if (pPriority != NULL)
    *pPriority = (regValue & RTL8368S_1QMCR_MSK(srcpriority)) >> RTL8368S_1QMCR_OFFSET(srcpriority);

  return 0;
}


/*=======================================================================
 * ASIC DRIVER API: FLOW CONTROL
 *========================================================================*/
/*
   @func int32_t | rtl8368s_set_asic_led_indicate_info_config | 
         Set Leds indicated information mode
   @parm uint32_t | ledNo | LED group number. 
         There are 1 to 1 led mapping to each port in each led group.
   @parm enum RTL8368S_LEDCONF | config | Support 16 types configuration.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @comm
   The API can set LED indicated information configuration for each LED group 
   with 1 to 1 led mapping to each port.
   Definition            LED Statuses                    Description
         0000            LED_Off                         LED pin Tri-State.
         0001            Dup/Col                         Collision, Full duplex Indicator. Blinking every 43ms when collision happens. Low for full duplex, and high for half duplex mode.
         0010            Link/Act                                Link, Activity Indicator. Low for link established. Link/Act Blinks every 43ms when the corresponding port is transmitting or receiving.
         0011            Spd1000                         1000Mb/s Speed Indicator. Low for 1000Mb/s.
         0100            Spd100                          100Mb/s Speed Indicator. Low for 100Mb/s.
         0101            Spd10                           10Mb/s Speed Indicator. Low for 10Mb/s.
         0110            Spd1000/Act                     1000Mb/s Speed/Activity Indicator. Low for 1000Mb/s. Blinks every 43ms when the corresponding port is transmitting or receiving.
         0111            Spd100/Act                      100Mb/s Speed/Activity Indicator. Low for 100Mb/s. Blinks every 43ms when the corresponding port is transmitting or receiving.
         1000            Spd10/Act                       10Mb/s Speed/Activity Indicator. Low for 10Mb/s. Blinks every 43ms when the corresponding port is transmitting or receiving.
         1001            Spd100 (10)/Act         10/100Mb/s Speed/Activity Indicator. Low for 10/100Mb/s. Blinks every 43ms when the corresponding port is transmitting or receiving.
         1010            Fiber                           Fiber link Indicator. Low for Fiber.
         1011            Fault   Auto-negotiation        Fault Indicator. Low for Fault.
         1100            Link/Rx                         Link, Activity Indicator. Low for link established. Link/Rx Blinks every 43ms when the corresponding port is transmitting.
         1101            Link/Tx                         Link, Activity Indicator. Low for link established. Link/Tx Blinks every 43ms when the corresponding port is receiving.
         1110            Master                          Link on Master Indicator. Low for link Master established.
         1111            LED_Force                       Force LED output, LED output value reference
*/
int32_t rtl8368s_set_asic_led_indicate_info_config(uint32_t ledNo, 
                                                   enum RTL8368S_LEDCONF config)
{
  uint32_t retVal, regValue;

  if(ledNo >= RTL8368S_LED_GROUP_MAX)
    return ERRNO_INPUT;

  if(config > LEDCONF_LEDFORCE)
    return ERRNO_INPUT;

  /* Get register value */
  retVal = rtl8368s_get_asic_reg(RTL8368S_LED_INDICATED_CONF_REG, &regValue);
  if (retVal != 0)
    return retVal;

  regValue = (regValue & (~(0xF<<(ledNo*4)))) | (config<<(ledNo*4));

  retVal = rtl8368s_set_asic_reg(RTL8368S_LED_INDICATED_CONF_REG, regValue);

  return retVal;
}

/*
   @func int32_t | rtl8368s_get_asic_led_indicate_info_config | 
         Get Leds indicated information mode
   @parm uint32_t | ledNo | LED group number. 
         There are 1 to 1 led mapping to each port in each led group.
   @parm enum RTL8368S_LEDCONF* | config | Support 16 types configuration.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @comm
   The API can get LED indicated information configuration for each LED group.
*/
int32_t rtl8368s_get_asic_led_indicate_info_config(uint32_t ledNo, 
                                                   enum RTL8368S_LEDCONF *config)
{
  uint32_t retVal, regValue;

  if(ledNo >= RTL8368S_LED_GROUP_MAX)
    return ERRNO_INPUT;

  /* Get register value */
  retVal = rtl8368s_get_asic_reg(RTL8368S_LED_INDICATED_CONF_REG, &regValue);
  if (retVal != 0)
    return retVal;

  *config = (regValue >> (ledNo*4)) & 0x000F;

  return 0;
}

/*
   @func int32_t | rtl8368s_set_asic_force_leds | Turn on/off Led of dedicated port
   @parm uint32_t | ledG0Msk | Turn on or turn off Leds of group 0, 1:on 0:off.
   @parm uint32_t | ledG1Msk | Turn on or turn off Leds of group 1, 1:on 0:off.
   @parm uint32_t | ledG2Msk | Turn on or turn off Leds of group 2, 1:on 0:off.
   @parm uint32_t | ledG3Msk | Turn on or turn off Leds of group 3, 1:on 0:off.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @comm
   The API can turn on/off desired Led group of dedicated port 
   while indicated information configuration of LED group is set to force mode.
*/
int32_t rtl8368s_set_asic_force_leds(uint32_t ledG0Msk, uint32_t ledG1Msk, 
                                     uint32_t ledG2Msk, uint32_t ledG3Msk)
{
  uint32_t retVal, regValue;

  regValue = (ledG0Msk & RTL8368S_LED_0_FORCE_MASK) | 
             ((ledG1Msk << RTL8368S_LED_1_FORCE_OFF)&RTL8368S_LED_1_FORCE_MASK);

  retVal = rtl8368s_set_asic_reg(RTL8368S_LED_0_1_FORCE_REG, regValue);
  if(retVal != 0)
    return retVal;

  regValue = (ledG2Msk & RTL8368S_LED_2_FORCE_MASK) | 
             ((ledG3Msk << RTL8368S_LED_3_FORCE_OFF)&RTL8368S_LED_3_FORCE_MASK);
  retVal = rtl8368s_set_asic_reg(RTL8368S_LED_2_3_FORCE_REG, regValue);

  return retVal;
}

/*
   @func int32_t | rtl8368s_set_asic_led_blink_rate | Set led blinking rate ate mode 0 to mode 3
   @parm enum RTL8368S_LEDBLINKRATE | blinkRate | Support 6 blink rates.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @comm
   The API can set LED blink rate at 43ms, 84ms, 120ms, 170ms, 340ms and 670ms.
*/
int32_t rtl8368s_set_asic_led_blink_rate(enum RTL8368S_LEDBLINKRATE blinkRate)
{
  uint32_t retVal = 0;
  uint32_t regData = 0;

  if (blinkRate >= LEDBLINKRATE_MAX)
    return ERRNO_INPUT;

  regData = blinkRate << RTL8368S_LED_BLINKRATE_OFF;

  retVal = rtl8368s_set_asic_reg_bits(RTL8368S_LED_BLINK_REG, 
                                      RTL8368S_LED_BLINKRATE_MSK,
                                      regData);

  return retVal;
}

/*
   @func int32_t | rtl8368s_get_asic_led_blink_rate | 
         Get led blinking rate ate mode 0 to mode 3
   @parm enum RTL8368S_LEDBLINKRATE* | blinkRate | Support 6 blink rates.
   @rvalue 0 | Success.
   @rvalue ERRNO_SMI | SMI access error.
   @comm
   The API can set LED blink rate at 43ms, 84ms, 120ms, 170ms, 340ms and 670ms.
*/
int32_t rtl8368s_get_asic_led_blink_rate(enum RTL8368S_LEDBLINKRATE *blinkRate)
{
  uint32_t retVal;
  uint32_t regData;

  retVal = rtl8368s_get_asic_reg(RTL8368S_LED_BLINK_REG, &regData);
  if(retVal != 0)
    return retVal;

  *blinkRate = (regData&RTL8368S_LED_BLINKRATE_MSK) >> RTL8368S_LED_BLINKRATE_OFF;

  return 0;
}

/*
 * ex: ts=8 sw=2 sts=2 et
 */

