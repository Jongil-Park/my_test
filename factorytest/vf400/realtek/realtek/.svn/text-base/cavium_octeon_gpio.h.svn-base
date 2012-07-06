#ifndef __CAVIUM_OCTEON_GPIO_H__
#define __CAVIUM_OCTEON_GPIO_H__

#define BIT_0                           0x01
#define BIT_1                           0x02
#define BIT_2                           0x04
#define BIT_3                           0x08
#define BIT_4                           0x10
#define BIT_5                           0x20
#define BIT_6                           0x40
#define BIT_7                           0x80
#define BIT_8                           0x0100
#define BIT_9                           0x0200
#define BIT_10                          0x0400
#define BIT_11                          0x0800
#define BIT_12                          0x1000
#define BIT_13                          0x2000
#define BIT_14                          0x4000
#define BIT_15                          0x8000

/* ioctl cmd definition */
#define OCTEON_GPIO_CFG_RD              0xCA00
#define OCTEON_GPIO_CFG_WR              0xCA01
#define OCTEON_GPIO_TX_SET              0xCA02
#define OCTEON_GPIO_DAT_RD              0xCA03
#define OCTEON_GPIO_TX_CLR              0xCA04

/*the structure of gpio*/
struct octeon_gpio
{
  int port;                       /* GPIO pin number */
  unsigned long reg_val;
};

#define GPIO_DIR_OUT	                1
#define GPIO_DIR_IN	                0

#define SMI_SCK	                        4
#define SMI_SDA	                        5

#endif /* __CAVIUM_OCTEON_GPIO_H__ */

/*
 * ex: ts=8 sw=2 sts=2 et
 */
