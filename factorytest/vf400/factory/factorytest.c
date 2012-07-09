#include "factorytest.h"

extern const char *ipList[];
extern const char *infNameList[];

#define  PRINT_MSG(msg)    redirectOutput(SERIAL_CONSOLE_INF, msg)
#define  PRINT_DONEMSG    PRINT_MSG("DONE\n\r")

struct factoytest_db {
  char   *name;
  int      (*func) (void);
  void    (*pass) (void);
  char   *fail;
};


#define ASCII_ESC 27

///////////////////////////////////////////////////////////////////////////////
// failure message list
///////////////////////////////////////////////////////////////////////////////
static char  USBfailmsg[] =
  " - USB \t\t: "
  "USB device test failed.\r\n";

static char  RTCfailmsg[] =
  " - RTC \t\t: "
  "RTC device test failed.\r\n";

static char  DEFAULTfailmsg[] =
  " - DEFAULT PING \t: "
  "DEFAULT ping test failed.\r\n";

static char  LEDfailmsg_orange[] =
  " - LED(ORANGE) \t: "
  "LED (ORANGE) test failed.\r\n";

static char  PINGfailmsg[] =
  " - PING \t: "
  "PING test failed.\r\n";

static char  LEDfailmsg_green[] =
  " - LED(GREEN) \t: "
  "LED (GREEN) test failed.\r\n";


///////////////////////////////////////////////////////////////////////////////
// result message list
///////////////////////////////////////////////////////////////////////////////
void display_pass_msg(void)
{
  PRINT_MSG("\n\r");
  PRINT_MSG("BBBBBBBBBBBB       BBB         BBBBBB    BBBBBB8   \r\n");
  PRINT_MSG("BBB     BBGB      BBBBB       BB    BB   BB    3B  \r\n");
  PRINT_MSG("BBB      BBB    BBBB  BBB     BB         BB         \r\n");
  PRINT_MSG("BBB     BB5B   BBBB    BBB    BB         BB         \r\n");
  PRINT_MSG("BBBBBBBBBBBB  BBBB     BBBB    BBBBBB    BBBBBB   \r\n");
  PRINT_MSG("BBBS5555      BBBB      BBBB    BBBBBB    BBBBBBB  \r\n");
  PRINT_MSG("BBB;          BBBBGQ888;BBBB         BB         BB    \r\n");
  PRINT_MSG("BBB5          BBDBBBBBBBBBBB         BB         BB  \r\n");
  PRINT_MSG("BBBO          BBBB      BBBB  BB    wBB  BB    wBB  \r\n");
  PRINT_MSG("BBBB          BBBB      BBBB   BBBBBB     BBBBBB   \r\n");
}


void display_fail_msg(void)
{
  PRINT_MSG("\n\r");
  PRINT_MSG("BBBBBBBBBBBB      BBB         BBBB    BBBB        \r\n");
  PRINT_MSG("BBBB             BBBB         BBBB    BBBB        \r\n");
  PRINT_MSG("BBBB            BBB BBB       BBBB    BBBB        \r\n");
  PRINT_MSG("BBBB           BBB   BBB      BBBB    BBBB        \r\n");
  PRINT_MSG("BBBBBBBBBB   .BBBB   BBBB     BBBB    BBBB        \r\n");
  PRINT_MSG("BBBBBBB8BB   BBBB     BBBB    BBBB    BBBB        \r\n");
  PRINT_MSG("BBBB         BBBB     BBBB    BBBB    BBBB        \r\n");
  PRINT_MSG("B5BB        BBBBBBBBBBBBBBB   BBBB    BBBB        \r\n");
  PRINT_MSG("B5BB        BBBB       BBBB   BBBB    BBBBBBBBBBBB\r\n");
  PRINT_MSG("B5BB        DBBB:      BBBB   BBBB    BBBBBBBBBBBB\r\n");
}




/*
 ** Get the yes/no answer from the user on the specified serial console interface
 */
char getAnswer(const char dev[])
{
  char c = 0;
  struct termios newtio;
  int fd;

  fd = open(dev, O_RDWR | O_NOCTTY );

  if (fd <0) {
    printf("Cannot open %s\n", dev);
    exit (-1);
  }

  tcgetattr(fd,&newtio);
  newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;
  // set input mode (non-canonical, no echo,...)
  newtio.c_lflag = 0;

  newtio.c_cc[VTIME] = 1;
  newtio.c_cc[VMIN] = 0;

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);

  // Read from the terminal
  while (1) {
    if(read(fd,&c,1) > 0)
      if( (c == 'Y') || (c=='y') || (c==13) || (c=='n') || (c=='N') )
        break;
  }

  close(fd);

  return c;
}

char getHiddenAnswer(const char dev[])
{
  char c = 0;
  struct termios newtio;
  int fd;

  fd = open(dev, O_RDWR | O_NOCTTY );

  if (fd <0) {
    printf("Cannot open %s\n", dev);
    exit (-1);
  }

  tcgetattr(fd,&newtio);
  newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;
  // set input mode (non-canonical, no echo,...)
  newtio.c_lflag = 0;

  newtio.c_cc[VTIME] = 1;
  newtio.c_cc[VMIN] = 0;

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);

  // Read from the terminal
  while (1)  {
    if(read(fd,&c,1) > 0)
      break;
  }

  close(fd);

  return c;
}

/*
 ** Redirect the output to the specified serial console interface
*/
void redirectOutput (const char dev[], const char str[])
{
  struct termios newtio;
  int fd;

  fd = open(dev, O_RDWR | O_NOCTTY );

  if (fd <0) {
    printf("Cannot open %s\n", dev);
    exit (-1);
  }

  tcgetattr(fd,&newtio);
  newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;
  // set input mode (non-canonical, no echo,...)
  newtio.c_lflag = 0;

  newtio.c_cc[VTIME] = 1;
  newtio.c_cc[VMIN] = 0;

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);


  // send it to the serial port
  write(fd, str, strlen(str));

  close(fd);

}

/*
 ** Get the process ID of the specified process name.
*/
unsigned int getProcessID(char *p_processname)
{
  DIR *dir_p;
  struct dirent *dir_entry_p;
  char dir_name[40];
  char target_name[256];
  int target_result;
  char exe_link[256];
  int errorcount;
  int result;

  errorcount=0;
  result=0;
  dir_p = opendir("/proc/");                               // Open /proc/ directory
  while(NULL != (dir_entry_p = readdir(dir_p)))                     // Reading /proc/ entries
  {
    if (strspn(dir_entry_p->d_name, "0123456789") == strlen(dir_entry_p->d_name))   // Checking for numbered directories
    {
      strcpy(dir_name, "/proc/");
      strcat(dir_name, dir_entry_p->d_name);
      strcat(dir_name, "/");                             // Obtaining the full-path eg: /proc/24657/
      exe_link[0] = 0;
      strcat(exe_link, dir_name);
      strcat(exe_link, "exe");                          // Getting the full-path of that exe link
      target_result = readlink(exe_link, target_name, sizeof(target_name)-1);    // Getting the target of the exe ie to which binary it points to
      if (target_result > 0)
      {
        target_name[target_result] = 0;
        if (strstr(target_name, p_processname) != NULL)             // Searching for process name in the target name -- ??? could be a better search !!!
        {
          result = atoi(dir_entry_p->d_name);
          // Found the process
          closedir(dir_p);
          return result;
        }
      }
    }
  }
  closedir(dir_p);

  // Process not found
  return result;
}


///////////////////////////////////////////////////////////////////////////////
// DEFAULT ping test function
///////////////////////////////////////////////////////////////////////////////
int DEFAULTtest(void)
{
  int        i,j;
  int        pingSuccess = 0;
  unsigned int  isOk = -1;            // false

  PRINT_MSG("\n\rDEFAULT PING\t\t");
  PRINT_MSG(">>>>>>");
  for( i=0; i<20; i++)  {
    if ( ((i+1)%2) ) {
      PRINT_MSG("\033[6D");
      PRINT_MSG(">>>>>>");
      sleep(1);
    }
    else {
      PRINT_MSG("\033[6D");
      PRINT_MSG("------");
      sleep(1);
    }

    pingSuccess = ping(ipList[0]);
    if( pingSuccess == MAX_PING_COUNT )     {
        isOk = 0;
        break;
    }
    else {                   // Ping failed
      isOk = -1;
    }
  }

  return isOk;
}


///////////////////////////////////////////////////////////////////////////////
// board check function
///////////////////////////////////////////////////////////////////////////////
void CHECKboard(void)
{
  FILE *fp;
  char buf[BUFSIZ], board[128];
  char path[256], *bp, *sp;

  memset (buf, 0, sizeof (buf));

  PRINT_MSG("\n\r\n\rSystem information");
  PRINT_MSG("\n\r======================================\n\r");

  if ((fp = fopen ("/proc/octeon_info", "r")) ||  (fp = fopen (path, "r"))) {
      while (fgets (buf, BUFSIZ, fp)) {
        if (!strncmp (buf, "board_serial_number", strlen ("board_serial_number"))) {
          PRINT_MSG(buf);
          PRINT_MSG("\r");
        }
        else if (!strncmp (buf, "mac_addr_base", strlen ("mac_addr_base"))) {
          PRINT_MSG(buf);
          PRINT_MSG("\r");
        }
      }
    fclose (fp);
  }
}



///////////////////////////////////////////////////////////////////////////////
// print serial number function
///////////////////////////////////////////////////////////////////////////////
void PRINTserialnumber(void)
{
  FILE *fp;
  char buf[BUFSIZ], board[128];
  char path[256], *bp, *sp;

  memset (buf, 0, sizeof (buf));
  if ((fp = fopen ("/proc/octeon_info", "r")) ||  (fp = fopen (path, "r"))) {
      while (fgets (buf, BUFSIZ, fp)) {
        if (!strncmp (buf, "board_serial_number", strlen ("board_serial_number"))) {
          PRINT_MSG("BOARD SN ");
          memcpy(buf, &buf[strlen ("board_serial_number")], BUFSIZ);
          PRINT_MSG(buf);
          PRINT_MSG("\r");
        }
      }
    fclose (fp);
  }
}



///////////////////////////////////////////////////////////////////////////////
// reboot function
///////////////////////////////////////////////////////////////////////////////
void rebootSystem()
{
  char status[256];

  /* Reboot the device */
  PRINT_MSG("\n\rSystem restart...\n\r");
  PRINT_MSG("======================================\n\r\n\r\n\r");
  system("sync");
  system("reboot -f");
  exit (0);
}


///////////////////////////////////////////////////////////////////////////////
// ping function
///////////////////////////////////////////////////////////////////////////////
int ping (const char ip[])
{
  char buf[MAX_PING_RESULT_LEN + 1];
  char cmd[256];
  int i = 0, c;
  int count = 0;
  FILE *fp;

  if (strlen(ip) == 0)
    return -1;

  sprintf(cmd, "ping -c %d -W %d %s | grep -c 'seq'", MAX_PING_COUNT, MAX_PING_WAIT, ip);
  fp=popen(cmd, "r");

  if (!fp)
    return -1;
  else {
    for(i=0; i< MAX_PING_RESULT_LEN; i++) {
      c=fgetc(fp);
      if( c==EOF || c== '\0' || c=='\n')
        break;

      buf[i]=(char) c;
    }
    buf[i]='\0';
  }

  /* Now, get the result */
  count = atoi(buf);

  /* Close the pipe */
  if (pclose(fp) == -1) {
    fprintf (stderr, "Cannot close the pipe.\n");
    return -1;
  }

  return count;
}


///////////////////////////////////////////////////////////////////////////////
// USB test function
///////////////////////////////////////////////////////////////////////////////
int USBTest()
{
  FILE *fp;
  int i, ret = 0;
  const char source[] = "/dev/sdb1";
  const char target[] = "/media/usbdisk";
  const char default_filename[] = "/media/usbdisk/testfile";
  char filename[MAX_FILENAME_LEN];
  const char basetext[] = "USB Testing. This is file #";
  char text[1024];
  char status[1024];
  struct stat sts;

  PRINT_MSG("\n\rUSB\t\t\t");
  PRINT_MSG(">>>>>>");
  sleep(1);
  PRINT_MSG("\033[6D");
  PRINT_MSG("------");
  sleep(1);

  /* Create some files, write some texts on the usb device */
  for(i=0; i<5; i++) {
    /* Make filename first */
    sprintf (filename, "%s%d", default_filename, i);

    fp = fopen(filename, "w");
    if (fp) {
      sprintf(text, "%s%d", basetext, i);
      fprintf (fp, text);
    }
    else {
      //sprintf (status, "%s\n\r", strerror(errno));
      //PRINT_MSG(status);
      return -1;
    }

    fclose(fp);
  }

  /* Sleep for some seconds before sync */
  for(i=0; i<100; i++) {
    usleep(1000);
  }
  sync();

  /* Re-mount the usb drive and verify the created files */
  ret = mount(source, target, "vfat", MS_REMOUNT, "");
  if(ret == -1)   {
    sprintf (status, "%s\n\r", strerror(errno));
    PRINT_MSG(status);
    return -1;
  }

  for(i=0; i<5; i++)   {
    sprintf(filename, "%s%d", default_filename, i);
    if (stat(target, &sts) == -1)
      return -1;
  }

  /* Delete files from the usb */
  for(i=0; i<5; i++)   {
    sprintf(filename, "%s%d", default_filename, i);
    if(remove(filename))   {
      sprintf(status, "%s\n\r", strerror(errno));
      PRINT_MSG(status);
    }
  }

  /* Sleep for some seconds and sync before un-mounting the filesystem */
  for(i=0; i<100; i++) {
    usleep(1000);
  }
  sync();

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
// RTC test function
///////////////////////////////////////////////////////////////////////////////
int RTCTest()
{
  FILE *fp;
  char pBuf[1024];

  memset(pBuf, 0, sizeof(pBuf));


  PRINT_MSG("\n\rRTC\t\t\t");
  PRINT_MSG(">>>>>>");
  sleep(1);
  PRINT_MSG("\033[6D");
  PRINT_MSG("------");
  sleep(1);

  system("date 101105022011;hwclock -w");

  usleep(1000);

  fp=popen("hwclock", "r");

  if(!fp)
    return -1;

  fgets(pBuf, 16, fp);

  pclose(fp);

  if(strncmp("Tue Oct 11 05:02:00 2011  0.000000 second", pBuf, 14))   {
    return -1;
  }

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
// LED test function (GREEN)
///////////////////////////////////////////////////////////////////////////////
int LEDTest_green()
{
  int i = 0;
  char c;
  char status[1024];

  system("/media/disk0/phy_spd100");
  system("/media/disk0/realtek_spd100");

  /* Wait a few seconds for nsm process to start up */
  PRINT_MSG("\n\rLED (GREEN)\t\t");
  PRINT_MSG(">>>>>>");
  for ( i=0; i<10; i++ ) {
    if ( ((i+1)%2) ) {
      sleep(1);
      PRINT_MSG("\033[6D");
      PRINT_MSG("------");
    }
    else {
      sleep(1);
      PRINT_MSG("\033[6D");
      PRINT_MSG(">>>>>>");
    }
  }


  /* Verify with user if all eth ports' color is correct, i.e. they should all be green color */
  sleep(1);
  PRINT_MSG("\033[6D");
  PRINT_MSG("Y/N...");

  c = getAnswer(SERIAL_CONSOLE_INF);
  PRINT_MSG("\033[6D");
  PRINT_MSG(">>>>>>");

  /* User says "Yes", go to default mode. Test is done! */
  if ( (c == 'Y') || (c == 'y') || (c == '\n')  || (c == '\r') )
    return 0;

  return -1;
}


///////////////////////////////////////////////////////////////////////////////
// LED test function (ORANGE)
///////////////////////////////////////////////////////////////////////////////
int LEDTest_orange()
{
  int i = 0;
  char c;
  char status[1024];

  PRINT_MSG("\n\rLED (ORANGE)\t\tY/N...");

  c = getAnswer(SERIAL_CONSOLE_INF);
  PRINT_MSG("\033[6D");
  PRINT_MSG(">>>>>>");

  /* User says "Yes", go to default mode. Test is done! */
  if ( (c == 'Y') || (c == 'y') || (c == '\n')  || (c == '\r') )
    return 0;

  return -1;
}


///////////////////////////////////////////////////////////////////////////////
// Ping test function
///////////////////////////////////////////////////////////////////////////////
int PINGtest(void)
{
  int i = 0;
  char c = 0;
  int ret = 0;
  char status[256];
  int isOK = 0;

  // Ping each interface now...
  for ( i=0; i<MAX_IP; i++ )  {
    sprintf(status, "\n\rPING (%s) %s\t", infNameList[i], ipList[i]);
    PRINT_MSG(status);

    if( (ret = ping(ipList[i])) == MAX_PING_COUNT ) {
      PRINT_MSG(">>>>>>\t\tP");
    }
    else {
      isOK = -1;
      PRINT_MSG(">>>>>>\t\tFAIL");
    }
  }

  PRINT_MSG("\n\rPING (TOTAL)\t\t>>>>>>");
  return isOK;
}


///////////////////////////////////////////////////////////////////////////////
// Factory test table.
///////////////////////////////////////////////////////////////////////////////
struct factoytest_db func_table[] =  {
  {"USB",           USBTest,            display_pass_msg,   USBfailmsg},
  {"RTC",           RTCTest,            display_pass_msg,   RTCfailmsg},
  {"DEFAUT",        DEFAULTtest,        display_pass_msg,   DEFAULTfailmsg},
  {"LED(ORANGE)",   LEDTest_orange,     display_pass_msg,   LEDfailmsg_orange},
  {"PING",          PINGtest,           display_pass_msg,   PINGfailmsg},
  {"LED(GREEN)",    LEDTest_green,      display_pass_msg,   LEDfailmsg_green},
  {"",              NULL,               NULL,               NULL}
};


///////////////////////////////////////////////////////////////////////////////
// Main Function
///////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[])
{
  int i = 0;
  char c;
  char status[256];
  int result[32];
  int index =0;
  int isOK = 0;

  index = 0;
  memset(&result, 0x00, sizeof(result));
  memset(&status, 0x00, sizeof(status));

  PRINT_MSG("\033[0;37;40m");   // Black
  PRINT_MSG("\033[2J");         // Clear screen
  PRINT_MSG("\n\r\n\r\n\r\n\r\tWelcome to Factory Test");

  // print board information
  CHECKboard();

  // run factorytest
  PRINT_MSG("\n\r\n\rTEST\t\t\tRunning\t\tRESULT");
  PRINT_MSG("\n\r===============================================");

  for ( index = 0; func_table[index].name[0] != '\0'; index++) {
    result[index] = func_table[index].func();
    if ( result[index] == 0 ) {
      PRINT_MSG("\033[6D");
      PRINT_MSG(">>>>>>\t\tP");
    }
    else {
      isOK = 1;
      PRINT_MSG("\033[6D");
      PRINT_MSG(">>>>>>\t\tFAIL");
    }
  }

  // print result
  PRINT_MSG("\n\r\n\r");
  PRINT_MSG("\n\r\n\r    FACTORY TEST RESULT");
  PRINT_MSG("\n\r===============================================");
  if ( isOK == 0 ) {
    display_pass_msg();
  }
  else {
    PRINT_MSG("\033[41m");  // RED
    PRINT_MSG("\n\r\n\r");
    display_fail_msg();
    PRINT_MSG("\r\n");

    PRINT_MSG("\n\r\n\r # FAILED LIST");
    PRINT_MSG("\n\r # ");
    PRINTserialnumber();
    PRINT_MSG("===============================================");
    PRINT_MSG("\r\n");
    for ( index = 0; func_table[index].name[0] != '\0'; index++) {
      if ( result[index] < 0 ) {
        PRINT_MSG(func_table[index].fail);
      }
    }
  }

  exit(0);
}

