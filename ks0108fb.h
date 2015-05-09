#include <stdlib.h>
#include "mraa.h"

//======================
//
// Gpio assignment
//
#define GPIO_RS	0
#define GPIO_RW 1
#define GPIO_E 2

#define GPIO_DB0 3
#define GPIO_DB1 4
#define GPIO_DB2 5
#define GPIO_DB3 6
#define GPIO_DB4 19	//because IO7 aka GPIO48 is broken when using WiFi: https://communities.intel.com/thread/59485
#define GPIO_DB5 8
#define GPIO_DB6 9
#define GPIO_DB7 10

#define GPIO_CSA 11
#define GPIO_RSTB 12
#define GPIO_CSB 13
#define GPIO_CSC 18

//======================
//
// Timing
//
#define TWH 500
#define TWL 500
#define TASU 140
#define TRESET 100000

//======================
//
// Signals & commands
//
#define KS_DISPLAY_ON 0x01
#define KS_DISPLAY_OFF 0

#define STATUS_BUSY 0x80
#define STATUS_ONOFF 0x20
#define STATUS_RESET 0x10

#define DISP_CONTROLLER_WIDTH 64
#define DISP_CONTROLLER_COUNT 3

mraa_gpio_context rs, rw, e, db0, db1, db2, db3, db4, db5, db6, db7, csa, csb, csc, rstb;

typedef unsigned char FrameBufferPixel;

typedef struct
{
    int x;
    int y;
    FrameBufferPixel * pixel;
} FrameBuffer;

//display dependend functions
void DISPselectController(int abc);
void DISPonOff(int command);

//ks0108 functions
void KSwriteDisplayData(int data);
void KSdisplayStartLine(int line);
void KSsetPage(int page);
void KSsetAddress(int address);
void KSdisplayOnOff(int command);

//framebuffer functions
FrameBuffer * FBcreate(int x, int y);
int FBwrite(FrameBuffer * fb);
int FBupdateRect(FrameBuffer * fb, int x, int y, int width, int height);
void FBgrid(FrameBuffer * fb, int width);
