#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* Serial Console */
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
/* Working with file */
#include <sys/stat.h>
#include <dirent.h>
/* Mount and unmount the filesystem */
#include <sys/mount.h>
//#include <linux/fs.h>
#include <signal.h>
#include <errno.h>
//#include "realtekpwrdn.h"

#include "./rtl8366rb_api.h"
#include "./rtk_api.h"
#include "./rtk_api_ext.h"
#include "./rtl8367b_asicdrv_port.h"
#include "./realtek_wrap_func.h"

#include <unistd.h>
#include <fcntl.h>

#define IS_OK         0
#define RTL8367B_PHY_PAGE_ADDRESS 31

void normalize_string(char *str)
{
    char *r_ptr;
    char *w_ptr;
    int skip_sp = 0;

    if (!str) return;

    r_ptr = w_ptr = str;

    while(*r_ptr)
    {
        if (*r_ptr == 0x20) {
            // 중복된 space 면 skip
            if (skip_sp) {
                r_ptr++;
                continue;
            } else {
                skip_sp = 1;
            }
        } else {
            skip_sp = 0;
        }
        *w_ptr = *r_ptr;
        w_ptr++;
        r_ptr++;
    }

    *w_ptr = 0x00;
}


int check_board(void)
{
  FILE *fp;
  char buf[BUFSIZ], board[128];
  char path[256], *bp, *sp;

  memset (buf, 0, sizeof (buf));

  if ((fp = fopen ("/proc/octeon_info", "r")) ||  (fp = fopen (path, "r"))) {
      while (fgets (buf, BUFSIZ, fp)) {
          if (!strncmp (buf, "board_type", strlen ("board_type"))) {
            //printf(">>> %s\n", buf);
              bp = buf;
              if((sp = strstr(bp, ":")) != NULL) {
                  sprintf(board, "%s", sp+2);
                  board[strlen(board) -1] = '\0';
                  normalize_string(board);                           // 중복된 space 제거
                  memcpy(board, &board[1], strlen(board));   // space 제거
                  //printf(">>> get board : %s\n", board);
              }
          }
      }
    fclose (fp);
  }

  // VF1406 / VF400 (1406)
  if (!strncmp (board, "12004", strlen ("12004")))
    return IS_OK;
  // VF2408(2401)
  else if (!strncmp (board, "12006", strlen ("12006")))
    return IS_OK;
  else
    return -1;
}

int main (int argc, char *argv[])
{
  int ret = 0;
  int retVal = 0;
	int port = 0;
	rtk_uint32          phyData;
	rtk_uint32          phyEnMsk0;
	rtk_uint32          phyEnMsk4;
	rtk_uint32          phyEnMsk9;

  FILE *chkrtl = NULL;

   // VF1406(1406) / VF400(1406) / VF2408(2401)에만 동작되도록 합니다.
  if ( check_board() != IS_OK )
    return 0;

  // Be careful!!!!! VF400 support 4 port.
  for ( port = 0; port < 4; port++ ){
      /*100BASE-TX full duplex capable in reg 4.8*/
      phyEnMsk4 = phyEnMsk4 | (1 << 8);
      /*Speed selection [1:0] */
      /* 11=Reserved*/
      /* 10= 1000Mpbs*/
      /* 01= 100Mpbs*/
      /* 00= 10Mpbs*/
      phyEnMsk0 = phyEnMsk0 & (~(1 << 6));
      phyEnMsk0 = phyEnMsk0 | (1 << 13);
      /*Full duplex mode in reg 0.8*/
      phyEnMsk0 = phyEnMsk0 | (1 << 8);

     if ((retVal = rtl8367b_setAsicPHYReg(port, RTL8367B_PHY_PAGE_ADDRESS, 0)) != RT_ERR_OK)
         return retVal;

     /*1000 BASE-T control register setting*/
     if ((retVal = rtl8367b_getAsicPHYReg(port, PHY_1000_BASET_CONTROL_REG, &phyData)) != RT_ERR_OK)
         return retVal;

     phyData = (phyData & (~0x0200)) | phyEnMsk9 ;
     printf("phyData = 0x%x\n", phyData);

     if ((retVal = rtl8367b_setAsicPHYReg(port, PHY_1000_BASET_CONTROL_REG, phyData)) != RT_ERR_OK)
         return retVal;

     /*Auto-Negotiation control register setting*/
     if ((retVal = rtl8367b_getAsicPHYReg(port, PHY_AN_ADVERTISEMENT_REG, &phyData)) != RT_ERR_OK)
         return retVal;

     phyData = (phyData & (~0x0DE0)) | phyEnMsk4;
     if ((retVal = rtl8367b_setAsicPHYReg(port, PHY_AN_ADVERTISEMENT_REG, phyData)) != RT_ERR_OK)
         return retVal;

     /*Control register setting and power off/on*/
     phyData = phyEnMsk0 & (~(1 << 12));

     printf("phyData = 0x%x\n", phyData);

     phyData |= (1 << 11);   /* power down PHY, bit 11 should be set to 1 */
     if ((retVal = rtl8367b_setAsicPHYReg(port, PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
         return retVal;

     phyData = phyData & (~(1 << 11));   /* power on PHY, bit 11 should be set to 0*/
     if ((retVal = rtl8367b_setAsicPHYReg(port, PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
         return retVal;

  }



  return 0;
}



