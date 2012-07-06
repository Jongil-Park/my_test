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
#include "cavium_octeon_gpio.h"
#include "smi.h"

#define DELAY                 10
#define CLK_DURATION(clk)     { int i; for(i=0; i<clk; i++); }

int _rtl865x_initGpioPin(int pin, int direction)
{
  int fd = -1;
  struct octeon_gpio gpio;

  fd = open("/proc/octeon_gpio" ,O_RDWR);
  if (fd < 0)
    return -1;

  memset(&gpio, 0x0, sizeof(struct octeon_gpio));

  gpio.port = pin;
  if (ioctl(fd, OCTEON_GPIO_CFG_RD, &gpio) < 0)
    {
      close(fd);
      return -1;
    }

  if (direction == 1)
    gpio.reg_val |= 0x0001;
  else
    gpio.reg_val &= ~0x0001;

  if (ioctl(fd, OCTEON_GPIO_CFG_WR, &gpio) < 0)
    {
      close(fd);
      return -1;
    }

  close(fd);

  return 0;
}

int _rtl865x_setGpioDataBit(uint32_t pin, uint32_t value)
{
  int fd = -1;
  struct octeon_gpio gpio;

  fd = open("/proc/octeon_gpio", O_RDWR);
  if (fd < 0)
    return -1;

  memset(&gpio, 0x0, sizeof(struct octeon_gpio));

  gpio.port = pin;
  if (value == 1)
    {
      gpio.reg_val = (1 << pin);
      if (ioctl(fd, OCTEON_GPIO_TX_SET, &gpio) < 0)
        {
          close(fd);
          return -1;
        }
    }
  else
    {
      gpio.reg_val = (1 << pin);
      if (ioctl(fd, OCTEON_GPIO_TX_CLR, &gpio) < 0)
        {
          close(fd);
          return -1;
        }
    }

  close(fd);

  return 0;
}

uint32_t _rtl865x_getGpioDataBit(uint32_t pin)
{
  int fd = -1;
  struct octeon_gpio gpio;
  uint32_t value = 0;

  fd = open("/proc/octeon_gpio", O_RDWR);
  if (fd < 0)
    return value;

  memset(&gpio, 0x0, sizeof(struct octeon_gpio));

  gpio.port = pin;

  if (ioctl(fd, OCTEON_GPIO_DAT_RD, &gpio) < 0)
    {
      close(fd);
      return value;
    }

  value = (gpio.reg_val & (1 << pin)) ? 1 : 0;

  close(fd);

  return value;
}

void _smi_start(void)
{
  /* change GPIO pin to Output only */
  _rtl865x_initGpioPin(SMI_SDA, GPIO_DIR_OUT);
  _rtl865x_initGpioPin(SMI_SCK, GPIO_DIR_OUT);

  /* Initial state: SCK: 0, SDA: 1 */
  _rtl865x_setGpioDataBit(SMI_SCK, 0);
  _rtl865x_setGpioDataBit(SMI_SDA, 1);
  CLK_DURATION(DELAY);

  /* CLK 1: 0 -> 1, 1 -> 0 */
  _rtl865x_setGpioDataBit(SMI_SCK, 1);
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SCK, 0);
  CLK_DURATION(DELAY);

  /* CLK 2: */
  _rtl865x_setGpioDataBit(SMI_SCK, 1);
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SDA, 0);
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SCK, 0);
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SDA, 1);

}

void _smi_writeBit(uint16_t signal, uint32_t bitLen)
{
  for( ; bitLen > 0; bitLen--)
    {
      CLK_DURATION(DELAY);

      /* prepare data */
      if (signal & (1 << (bitLen - 1)) )
        _rtl865x_setGpioDataBit(SMI_SDA, 1);
      else
        _rtl865x_setGpioDataBit(SMI_SDA, 0);
      CLK_DURATION(DELAY);

      /* clocking */
      _rtl865x_setGpioDataBit(SMI_SCK, 1);
      CLK_DURATION(DELAY);
      _rtl865x_setGpioDataBit(SMI_SCK, 0);
    }
}

void _smi_readBit(uint32_t bitLen, uint32_t *rData)
{
  uint32_t u;

  /* change GPIO pin to Input only */
  _rtl865x_initGpioPin(SMI_SDA, GPIO_DIR_IN);

  for (*rData = 0; bitLen > 0; bitLen--)
    {
      CLK_DURATION(DELAY);

      /* clocking */
      _rtl865x_setGpioDataBit(SMI_SCK, 1);
      CLK_DURATION(DELAY);
      u = _rtl865x_getGpioDataBit(SMI_SDA);
      _rtl865x_setGpioDataBit(SMI_SCK, 0);

      *rData |= (u << (bitLen - 1));
    }

  /* change GPIO pin to Output only */
  _rtl865x_initGpioPin(SMI_SDA, GPIO_DIR_OUT);

}

void _smi_stop(void)
{
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SDA, 0);
  _rtl865x_setGpioDataBit(SMI_SCK, 1);
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SDA, 1);
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SCK, 1);
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SCK, 0);
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SCK, 1);

  /* add a click */
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SCK, 0);
  CLK_DURATION(DELAY);
  _rtl865x_setGpioDataBit(SMI_SCK, 1);

  /* change GPIO pin to Output only */
  _rtl865x_initGpioPin(SMI_SDA, GPIO_DIR_IN);
  _rtl865x_initGpioPin(SMI_SCK, GPIO_DIR_IN);

}

/*
 * ex: ts=8 sw=2 sts=2 et
 */

