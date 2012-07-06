/*
* Copyright c                  Realtek Semiconductor Corporation, 2006
* All rights reserved.
*
* Program : Control  smi connected RTL8366
* Abstract :
* Author : Yu-Mei Pan (ympan@realtek.com.cn)
*  $Id: smi.c,v 1.1 2007-10-15 02:37:31 abelshie Exp $
*/

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <setjmp.h>
#include <syslog.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "smi.h"      /*RTL8651B file*/
#include "smi_imp.h"

#define SWITCH_RTL8366RB   8366
#define SWITCH_RTL8367R     8367

#define ack_timer     5
#define max_register  0x018A

#define IS_RTL8367R   access("/etc/.rtl8367r", F_OK)
#define IS_RTL8366RB  access("/etc/.rtl8366rb", F_OK)
#define IS_OK         0

int smi_realtek_partno = 0;

void smi_get_switch_partno(void)
{
  int ret=0;

  // realtek switch chip을 partno를 설정합니다.
  // 초기화 과정이 끝나면 flag 파일이 생성되도록 되어 있기 때문에
  // flag 파일의 존재 여부를 가지고 partno를 설정합니다.
  // default 값으로 RTL8366으로 동작되도록 합니다.
  if (smi_realtek_partno == 0) {
    if (IS_RTL8367R == IS_OK)
      smi_realtek_partno = SWITCH_RTL8367R;
    else if (SWITCH_RTL8366RB == IS_OK)
      smi_realtek_partno = SWITCH_RTL8367R;
    else {
      smi_realtek_partno = 0;
    }
  }
}

int32_t smi_read (uint32_t mAddrs, uint32_t *rData)
{
  uint32_t rawData = 0, ACK = 1;
  uint8_t  con;
  uint32_t ret = 0;

  smi_get_switch_partno();

  /* Disable CPU interrupt to ensure that the SMI operation is atomic.
   * The API is based on RTL865X, rewrite the API if porting to other platform. */
  _smi_start();               /* Start SMI */

  if(smi_realtek_partno==SWITCH_RTL8367R)
    _smi_writeBit(0x0b, 4);     /* CTRL code: 4'b1011 */
  else
    _smi_writeBit(0x0a, 4);     /* CTRL code: 4'b1010 */
  _smi_writeBit(0x4, 3);      /* CTRL code: 3'b100 */
  _smi_writeBit(0x1, 1);      /* 1: issue READ command */
  con = 0;
  do {
    con++;
    _smi_readBit(1, &ACK);    /* ACK for issuing READ command*/
  } while ((ACK != 0) && (con < ack_timer));
  if (ACK != 0)
    ret = -1;

  _smi_writeBit((mAddrs&0xff), 8);  /* Set reg_addr[7:0] */
  con = 0;
  do {
    con++;
    _smi_readBit(1, &ACK);    /* ACK for setting reg_addr[7:0] */
  } while ((ACK != 0) && (con < ack_timer));
  if (ACK != 0)
    ret = -1;

  _smi_writeBit((mAddrs>>8), 8);    /* Set reg_addr[15:8] */
  con = 0;
  do {
    con++;
    _smi_readBit(1, &ACK);    /* ACK by RTL8369 */
  } while ((ACK != 0) && (con < ack_timer));
  if (ACK != 0)
    ret = -1;

  _smi_readBit(8, &rawData);  /* Read DATA [7:0] */
  *rData = rawData&0xff;

  _smi_writeBit(0x00, 1);     /* ACK by CPU */
  _smi_readBit(8, &rawData);  /* Read DATA [15: 8] */

  _smi_writeBit(0x01, 1);     /* ACK by CPU */
  *rData |= (rawData<<8);

  _smi_stop();

  return ret;
}

int32_t smi_write (uint32_t mAddrs, uint32_t rData)
{
  int8_t con;
  uint32_t ACK = 1;
  uint32_t ret = 0;

  smi_get_switch_partno();

  /* Disable CPU interrupt to ensure that the SMI operation is atomic.
   * The API is based on RTL865X, rewrite the API if porting to other platform. */

  _smi_start();             /* Start SMI */

  if(smi_realtek_partno==SWITCH_RTL8367R)
    _smi_writeBit(0x0b, 4);   /* CTRL code: 4'b1011 */
  else
    _smi_writeBit(0x0a, 4);   /* CTRL code: 4'b1010 */
  _smi_writeBit(0x4, 3);    /* CTRL code: 3'b100 */
  _smi_writeBit(0x0, 1);    /* 0: issue WRITE command */
  con = 0;
  do {
    con++;
    _smi_readBit(1, &ACK);  /* ACK for issuing WRITE command*/
  } while ((ACK != 0) && (con < ack_timer));
  if (ACK != 0)
    ret = -1;

  _smi_writeBit((mAddrs&0xff), 8);  /* Set reg_addr[7:0] */
  con = 0;
  do {
    con++;
    _smi_readBit(1, &ACK);  /* ACK for setting reg_addr[7:0] */
  } while ((ACK != 0) && (con < ack_timer));
  if (ACK != 0)
    ret = -1;

  _smi_writeBit((mAddrs>>8), 8);  /* Set reg_addr[15:8] */
  con = 0;
  do {
    con++;
    _smi_readBit(1, &ACK);  /* ACK for setting reg_addr[15:8] */
  } while ((ACK != 0) && (con < ack_timer));
  if (ACK != 0)
    ret = -1;

  _smi_writeBit(rData&0xff, 8);  /* Write Data [7:0] out */
  con = 0;
  do {
    con++;
    _smi_readBit(1, &ACK);  /* ACK for writting data [7:0] */
  } while ((ACK != 0) && (con < ack_timer));
  if (ACK != 0)
    ret = -1;

  _smi_writeBit(rData>>8, 8);  /* Write Data [15:8] out */
  con = 0;
  do {
    con++;
    _smi_readBit(1, &ACK);  /* ACK for writting data [15:8] */
  } while ((ACK != 0) && (con < ack_timer));
  if (ACK != 0)
    ret = -1;

  _smi_stop();

  return ret;
}

/*
 * ex: ts=8 sw=2 sts=2 et
 */

