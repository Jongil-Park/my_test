#include "factorytest.h"

extern const char *ipList[];
extern const char *infNameList[];

#define  PRINT_MSG(msg)    redirectOutput(SERIAL_CONSOLE_INF, msg)
#define  PRINT_DONEMSG    PRINT_MSG("DONE\n\r")

#define ASCII_ESC 27

void display_pass_msg(void)
{
  PRINT_MSG("\n\r");
  PRINT_MSG("BBBBBBBBB      B         BBBBBB    BBBBBB8   \r\n");
  PRINT_MSG("B     BBG     BBB       BB    BB   BB    3B  \r\n");
  PRINT_MSG("B      BB    BB BB     BB         BB         \r\n");
  PRINT_MSG("B     BB5   BB   BB    BB         BB         \r\n");
  PRINT_MSG("BBBBBBBBB  BB     BB    BBBBBB    BBBBBB   \r\n");
  PRINT_MSG("BS5555     BB      BB    BBBBBB    BBBBBBB  \r\n");
  PRINT_MSG("B;         BBGQ888;BB         BB         BB    \r\n");
  PRINT_MSG("B5         DBBBBBBBBB         BB         BB  \r\n");
  PRINT_MSG("BO         BB      BB  BB    wBB  BB    wBB  \r\n");
  PRINT_MSG("BB         BB      BB   BBBBBB     BBBBBB   \r\n");
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
  PRINT_MSG("B5BB         BBBBBBBBBBBBB    BBBB    BBBB        \r\n");
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

/*
 ** Check if the CPU is ready for ping test
 */
int checkDefaultPing(void)
{
  int        i,j;
  int        pingSuccess = 0;
  unsigned int  isOk = -1;            // false

  for( i=0; i<20; i++)  {
    PRINT_MSG(".");
    pingSuccess = ping(ipList[0]);
    printf("pingSuccess = %d\n", pingSuccess);
    sleep(1);
    if( pingSuccess == MAX_PING_COUNT )     {
//      PRINT_MSG(".");
//      pingSuccess = ping(ipList[0]);    // Re-ping to confirm
//      printf("pingSuccess = %d\n", pingSuccess);
//      if( pingSuccess == MAX_PING_COUNT ) {   // Ping success
        isOk = 0;
        break;
//      }
    }
    else {                   // Ping failed
      isOk = -1;
    }
    sleep(1);
  }

  if (isOk == 0)
    return 0;
  else
    return -1;

  return 0;
}


/*
 ** Reboot the system
 */
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


/*
 ** Ping to a specified IP address
*/
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


/*
 **
*/
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

/*
 **
*/
int SATATest()
{
  FILE *fp;
  int i, ret = 0;
  const char source[] = "/dev/sda1";
  const char target[] = "/media/disk1";
  const char default_filename[] = "/media/disk1/testfile";
  char filename[MAX_FILENAME_LEN];
  const char basetext[] = "SATA Testing. This is file #";
  char text[1024];
  char status[1024];
  struct stat sts;


  /* Create files from the sata */
  for(i=0; i<5; i++) {
    /* Make filename first */
    sprintf (filename, "%s%d", default_filename, i);

    fp = fopen(filename, "w");
    if (fp) {
      sprintf(text, "%s%d", basetext, i);
      fprintf (fp, text);
    }
    else {
      sprintf (status, "%s\n\r", strerror(errno));
      PRINT_MSG(status);
      return -1;
    }

    fclose(fp);
  }

  /* Sleep for some seconds before sync */
  for(i=0; i<100; i++) {
    usleep(1000);
  }
  sync();

  for(i=0; i<5; i++)  {
    sprintf(filename, "%s%d", default_filename, i);
    if (stat(target, &sts) == -1)
      return -1;
  }

  /* Delete files from the sata */
  for(i=0; i<5; i++) {
    sprintf(filename, "%s%d", default_filename, i);
    if(remove(filename))   {
      sprintf(status, "%s\n\r", strerror(errno));
      PRINT_MSG(status);
    }
  }
  sync();
  return 0;
}

int RTCTest()
{
  FILE *fp;
  char pBuf[1024];

  memset(pBuf, 0, sizeof(pBuf));

  system("date 101105022011;hwclock -w");

  usleep(1000);

  fp=popen("hwclock", "r");

  if(!fp)
    return -1;

  fgets(pBuf, 16, fp);

  pclose(fp);

  //PRINT_MSG("\n\r");
  //PRINT_MSG(pBuf);
  //PRINT_MSG("\n\r");

  if(strncmp("Tue Oct 11 05:02:00 2011  0.000000 second", pBuf, 14))   {
    return -1;
  }

  return 0;
}


/*
 **
*/
int LEDTest()
{
  int i = 0;
  char c;
  char status[1024];

  system("/media/disk0/phy_spd100");
  system("/media/disk0/realtek_spd100");

  /* Wait a few seconds for nsm process to start up */
  PRINT_MSG("Please wait for 5 or 10 seconds...\n\r");
  for ( i=0; i<10; i++ ) {
    PRINT_MSG("#");
    sleep(1);
  }

  /* Verify with user if all eth ports' color is correct, i.e. they should all be green color */
  PRINT_MSG("\r\nAre all LEDs green? (Y/n): ");

  c = getAnswer(SERIAL_CONSOLE_INF);
  sprintf (status, "%c\n\r", c);
  PRINT_MSG(status);

  /* User says "Yes", go to default mode. Test is done! */
  if ( (c == 'Y') || (c == 'y') || (c == '\n')  || (c == '\r') )
    return 0;
  else
    return -1;

  return -1;
}

//exist -> return 0, no exist -> 1
int checkAutoTest(const char *pFilename)
{
  FILE *fp;

  fp=fopen(pFilename,"r");
  if(fp<=0)
    return 1;

  fclose(fp);

  return 0;
}

int usb_test(void)
{
  char c = 0;
  int ret = 0;
  char status[512];

  memset(status, 0x00, sizeof(status));

  PRINT_MSG("\033[0;37;40m");  // Black
  PRINT_MSG("\n\r\n\r============== USB Test ==============\n\r");
  //PRINT_MSG("Testing .....................\t");
  if ((ret = USBTest()) == 0)  {
    display_pass_msg();
    PRINT_MSG("\n\r======================================\n\r");
    return 0;
  }
  else
  {
//    PRINT_MSG("\033[41m");  // RED
//    PRINT_MSG("\n\r\n\r############# TEST FAILED ############\n\r");
//    PRINT_MSG("############# TEST FAILED ############\n\r");
//    PRINT_MSG("############# TEST FAILED ############\n\r");
//    PRINT_MSG("############# TEST FAILED ############\n\r");
    display_fail_msg();
//    PRINT_MSG("\n\r>>> USB Test Fail\n\r");
    PRINT_MSG("\n\r======================================\n\r");
    return -1;

//    for (;;) {
//      c = getHiddenAnswer(SERIAL_CONSOLE_INF);
//      usleep(1000);
//      if (c == 'H')
//        break;
//    }
  }

  return 0;
}

int ping_test(void)
{
  int i = 0;
  char c = 0;
  int ret = 0;
  char status[256];
  int testResult = 0;

  PRINT_MSG("\033[0;37;40m");  // Black
  PRINT_MSG("\n\r\n\r============== Ping Test =============\n\r");
  /* Check cpu idle by pinging to localhost address */

  // Ping each interface now...
  for ( i=0; i<MAX_IP; i++ )  {
    sprintf(status, "ping to (%s) %s.......", infNameList[i], ipList[i]);
    PRINT_MSG(status);

    if( (ret = ping(ipList[i])) == MAX_PING_COUNT ) {
      //PRINT_MSG("PASSED\n\r");
      PRINT_MSG("\n\r");
    }
    else {
      PRINT_MSG("FAILED\n\r");
      testResult=-1;
    }
  }

  if ( testResult == -1 ) {
//    return -1;
//    PRINT_MSG("\033[41m");  // RED
//    PRINT_MSG("\n\r\n\r############# TEST FAILED ############\n\r");
//    PRINT_MSG("############# TEST FAILED ############\n\r");
//    PRINT_MSG("############# TEST FAILED ############\n\r");
//    PRINT_MSG("############# TEST FAILED ############\n\r");
    display_fail_msg();
//    PRINT_MSG("\n\r>>> Ping Test Fail\n\r");
//    PRINT_MSG("\n\r======================================\n\r");

//    for (;;) {
//      c = getHiddenAnswer(SERIAL_CONSOLE_INF);
//      usleep(1000);
//      if (c == 'H')
//        break;
//    }
    return -1;
  }

  return 0;
}

int rtc_test(void)
{
  char c = 0;
  int ret = 0;
  char status[512];
  int testResult = 0;

  memset(status, 0x00, sizeof(status));

  PRINT_MSG("\033[0;37;40m");  // Black
  PRINT_MSG("\n\r\n\r============== RTC Test ==============\n\r");
  //PRINT_MSG("Testing .....................\t");

  if( (ret = RTCTest()) == 0 ) {
    display_pass_msg();
    return 0;
  }
  else {
//    PRINT_MSG("\033[41m");  // RED
//    PRINT_MSG("\n\r\n\r############# TEST FAILED ############\n\r");
//    PRINT_MSG("############# TEST FAILED ############\n\r");
//    PRINT_MSG("############# TEST FAILED ############\n\r");
//    PRINT_MSG("############# TEST FAILED ############\n\r");
    display_fail_msg();
//    PRINT_MSG("\n\r>>> RTC Test Fail\n\r");
    PRINT_MSG("\n\r======================================\n\r");

//    for (;;) {
//      c = getHiddenAnswer(SERIAL_CONSOLE_INF);
//      usleep(1000);
//      if (c == 'H')
//        break;
//    }
    return -1;
  }

  return 0;
}


int led_test(void)
{
  char c = 0;
  int ret = 0;
  char status[512];
  int testResult = 0;

  memset(status, 0x00, sizeof(status));

  PRINT_MSG("\033[0;37;40m");  // Black
  PRINT_MSG("\n\r\n\r============== LED Test ==============\n\r");
  if( (ret = LEDTest()) == 0 ) {
    PRINT_MSG("............................\tPASSED");
    PRINT_MSG("\n\r\n\r============= TEST PASSED ============\n\r");
    PRINT_MSG("============= TEST PASSED ============\n\r");
    PRINT_MSG("============= TEST PASSED ============\n\r");
    PRINT_MSG("============= TEST PASSED ============\n\r");

    display_pass_msg();

    PRINT_MSG("Running the same test in the next boot...\n\r");
    PRINT_MSG("Now reboot the system  ");
    //rebootSystem();
    return 0;
  }
  else {
    return -1;
    PRINT_MSG("............................\tFAILED");
    PRINT_MSG("\033[41m");  // RED
    PRINT_MSG("\n\r\n\r############# TEST FAILED ############\n\r");
    PRINT_MSG("############# TEST FAILED ############\n\r");
    PRINT_MSG("############# TEST FAILED ############\n\r");
    PRINT_MSG("############# TEST FAILED ############\n\r");

    display_fail_msg();

    PRINT_MSG("Please try the test again.\n\r");
    PRINT_MSG("Please try system reboot  again.\n\r");

    for (;;) {
      c = getHiddenAnswer(SERIAL_CONSOLE_INF);
      usleep(1000);
      if (c == 'H')
        break;
    }
  }
}


void ready_test(void)
{
  int ret = 0;
  char c;

  /*
   ** When this app is called automatically on start up, we need to pause the execution for some seconds.
   ** This is to guarantee that all services and processes have completely started.
   */
  PRINT_MSG("\n\rPlease wait for all services to be ready \n\r");
  PRINT_MSG("The Orange LEDs will go Green\n\r");

  ret = checkDefaultPing();

  // CPU is busy. We can't get accurate ping test.
  if ( ret == -1 ) {
    PRINT_MSG("\n\rSomething's wrong with the system\n\r");
    PRINT_MSG("\033[41m");  // RED
    PRINT_MSG("\n\r\n\r############# TEST FAILED ############\n\r");
    PRINT_MSG("############# TEST FAILED ############\n\r");
    PRINT_MSG("############# TEST FAILED ############\n\r");
    PRINT_MSG("############# TEST FAILED ############\n\r");

    display_fail_msg();

    PRINT_MSG("Please try the test again.\n\r");
    PRINT_MSG("Please try system reboot  again.\n\r");

    for (;;) {
      c = getHiddenAnswer(SERIAL_CONSOLE_INF);
      usleep(1000);
      if (c == 'H')
        break;
    }
  }
  PRINT_MSG("\n\r");

  PRINT_MSG("\n\rPress Any KEY to Start Testing... \n\r");
  getAnswer(SERIAL_CONSOLE_INF);
}

int main (int argc, char *argv[])
{
  int i = 0;
  int result[5];
  int index = 0;

  PRINT_MSG("\033[0;37;40m");  // Black
  PRINT_MSG("\033[2J");
  PRINT_MSG("\n\r\n\r\tWelcome to Factory Test\n\r");

  index = 0;
  memset(&result, 0x00, sizeof(result));

  ready_test();
  result[index++] = usb_test();
  result[index++] = ping_test();
  result[index++] = rtc_test();
  //result[index++] = led_test();

  for ( i = 0; i < index; i++ ) {
    if ( result[i] < 0 ) {
      PRINT_MSG("\033[41m");  // RED
      PRINT_MSG("\n\r\n\r");
      printf("\nTest Fail - Index[%d]", i);
    }
  }

  //rebootSystem();
  exit(0);
}

