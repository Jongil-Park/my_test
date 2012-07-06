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

#include "def.h"



// Model : VF2400(2408)
//#define	MAX_IP				8
//const char *ipList[] = {"17.17.17.2", "16.16.16.2", "15.15.15.2","10.10.10.2", "11.11.11.2", "12.12.12.2", "13.13.13.2", "14.14.14.2"};
//const char *infNameList[] = {"WAN3", "WAN2", "WAN1", "LAN1", "LAN2", "LAN3", "LAN4","LAN5"};

// Model : VF400
#define	MAX_IP				6
const char *ipList[] = {"15.15.15.2", "14.14.14.2","10.10.10.2", "11.11.11.2", "12.12.12.2", "13.13.13.2"};
const char *infNameList[] = {"WAN2", "WAN1", "LAN1", "LAN2", "LAN3", "LAN4"};

//const char *ipList[] = {"15.15.15.2", "14.14.14.2","10.10.10.2", "10.100.9.47", "12.12.12.2", "13.13.13.2"};
//const char *ipList[] = {"17.17.17.2", "15.15.15.2", "14.14.14.2","10.10.10.2", "11.11.11.2", "12.12.12.2", "13.13.13.2", "16.16.16.2"};
//const char *infNameList[] = {"WAN2", "WAN1", "LAN1", "LAN2", "LAN3", "LAN4"};
//const char *infNameList[] = {"WAN3", "WAN2", "WAN1", "LAN1", "LAN2", "LAN3", "LAN4","LAN5"};


extern char getAnswer(const char dev[]);
extern int ping(const char[]);
extern void redirectOutput(const char dev [ ], const char str [ ]);
extern int USBTest();
extern int LEDTest();






