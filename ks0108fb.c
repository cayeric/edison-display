#include "ks0108fb.h"
#include <unistd.h>

#define CHECK_GPIO if(GPIO_INIT == 0){initGPIO();}


static int GPIO_INIT = 0;


void nanoDelay(int nanos)
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = nanos;
    nanosleep(&ts, NULL);
    return;
}

void initGPIO()
{
    rs = mraa_gpio_init(GPIO_RS);
    mraa_gpio_dir(rs, MRAA_GPIO_OUT);
    rw = mraa_gpio_init(GPIO_RW);
    mraa_gpio_dir(rw, MRAA_GPIO_OUT);
    e = mraa_gpio_init(GPIO_E);
    mraa_gpio_dir(e, MRAA_GPIO_OUT);
    db0 = mraa_gpio_init(GPIO_DB0);
    mraa_gpio_dir(db0, MRAA_GPIO_OUT);
    db1 = mraa_gpio_init(GPIO_DB1);
    mraa_gpio_dir(db1, MRAA_GPIO_OUT);
    db2 = mraa_gpio_init(GPIO_DB2);
    mraa_gpio_dir(db2, MRAA_GPIO_OUT);
    db3 = mraa_gpio_init(GPIO_DB3);
    mraa_gpio_dir(db3, MRAA_GPIO_OUT);
    db4 = mraa_gpio_init(GPIO_DB4);
    mraa_gpio_dir(db4, MRAA_GPIO_OUT);
    db5 = mraa_gpio_init(GPIO_DB5);
    mraa_gpio_dir(db5, MRAA_GPIO_OUT);
    db6 = mraa_gpio_init(GPIO_DB6);
    mraa_gpio_dir(db6, MRAA_GPIO_OUT);
    db7 = mraa_gpio_init(GPIO_DB7);
    mraa_gpio_dir(db7, MRAA_GPIO_OUT);
    
    rstb = mraa_gpio_init(GPIO_RSTB);
    mraa_gpio_dir(rstb, MRAA_GPIO_OUT);
    csa = mraa_gpio_init(GPIO_CSA);
    mraa_gpio_dir(csa, MRAA_GPIO_OUT);
    csb = mraa_gpio_init(GPIO_CSB);
    mraa_gpio_dir(csb, MRAA_GPIO_OUT);
    csc = mraa_gpio_init(GPIO_CSC);
    mraa_gpio_dir(csc, MRAA_GPIO_OUT);
    
    mraa_gpio_use_mmaped(rs, 1);
    mraa_gpio_use_mmaped(rw, 1);
    mraa_gpio_use_mmaped(e, 1);
    mraa_gpio_use_mmaped(db0, 1);
    mraa_gpio_use_mmaped(db1, 1);
    mraa_gpio_use_mmaped(db2, 1);
    mraa_gpio_use_mmaped(db3, 1);
    mraa_gpio_use_mmaped(db4, 1);
    mraa_gpio_use_mmaped(db5, 1);
    mraa_gpio_use_mmaped(db6, 1);
    mraa_gpio_use_mmaped(db7, 1);
    mraa_gpio_use_mmaped(rstb, 1);
    mraa_gpio_use_mmaped(csa, 1);
    mraa_gpio_use_mmaped(csb, 1);
    mraa_gpio_use_mmaped(csc, 1);

    nanoDelay(TRESET);
    mraa_gpio_write(rstb,1);
    
    GPIO_INIT = 1;

    DISPonOff(1);
}

void DISPselectController(int abc)
{
    CHECK_GPIO;
    if (abc == 0)
    {
        mraa_gpio_write(csa, 0);
    }
    else
    {
        mraa_gpio_write(csa, 1);
    }
    
    if (abc == 1)
    {
        mraa_gpio_write(csb, 0);
    }
    else
    {
        mraa_gpio_write(csb, 1);
    }
    
    if (abc == 2)
    {
        mraa_gpio_write(csc, 0);
    }
    else
    {
        mraa_gpio_write(csc, 1);
    }
}

void DISPonOff(int command)
{
    //all displays on
    int controller=0;
    for (;controller< DISP_CONTROLLER_COUNT;controller++)
    {
        DISPselectController(controller);
        KSdisplayOnOff(KS_DISPLAY_ON);
    }
}

void KSdisplayOnOff(int command)
{
    CHECK_GPIO;
    mraa_gpio_write(rs, 0);
    mraa_gpio_write(rw, 0);
    nanoDelay(TASU);
    
    mraa_gpio_write(db7, 0);
    mraa_gpio_write(db6, 0);
    mraa_gpio_write(db5, 1);
    mraa_gpio_write(db4, 1);
    mraa_gpio_write(db3, 1);
    mraa_gpio_write(db2, 1);
    mraa_gpio_write(db1, 1);
    mraa_gpio_write(db0, command);
    
    mraa_gpio_write(e, 1);
    nanoDelay(TWH);
    
    mraa_gpio_write(e, 0);
    nanoDelay(TWL);
}

void KSsetAddress(int address)
{
    CHECK_GPIO;
    mraa_gpio_write(rs, 0);
    mraa_gpio_write(rw, 0);
    nanoDelay(TASU);
    
    mraa_gpio_write(db7, 0);
    mraa_gpio_write(db6, 1);
    
    mraa_gpio_write(db0, address & 1);
    address >>= 1;
    mraa_gpio_write(db1, address & 1);
    address >>= 1;
    mraa_gpio_write(db2, address & 1);
    address >>= 1;
    mraa_gpio_write(db3, address & 1);
    address >>= 1;
    mraa_gpio_write(db4, address & 1);
    address >>= 1;
    mraa_gpio_write(db5, address & 1);
    
    mraa_gpio_write(e, 1);
    nanoDelay(TWH);
    
    mraa_gpio_write(e, 0);
    nanoDelay(TWL);
    
}

void KSsetPage(int page)
{
    CHECK_GPIO;
    mraa_gpio_write(rs, 0);
    mraa_gpio_write(rw, 0);
    nanoDelay(TASU);
    
    mraa_gpio_write(db7, 1);
    mraa_gpio_write(db6, 0);
    mraa_gpio_write(db5, 1);
    mraa_gpio_write(db4, 1);
    mraa_gpio_write(db3, 1);
    
    mraa_gpio_write(db0, page & 1);
    page >>= 1;
    mraa_gpio_write(db1, page & 1);
    page >>= 1;
    mraa_gpio_write(db2, page & 1);
    
    mraa_gpio_write(e, 1);
    nanoDelay(TWH);
    
    mraa_gpio_write(e, 0);
    nanoDelay(TWL);
}

void KSdisplayStartLine(int line)
{
}

//not implemented because of read instruction
//int statusRead()
//{
//    return 0;
//}

void KSwriteDisplayData(int data)
{
    CHECK_GPIO;
    mraa_gpio_write(rs, 1);
    mraa_gpio_write(rw, 0);
    nanoDelay(TASU);
    
    mraa_gpio_write(db0, data & 1);
    data >>= 1;
    mraa_gpio_write(db1, data & 1);
    data >>= 1;
    mraa_gpio_write(db2, data & 1);
    data >>= 1;
    mraa_gpio_write(db3, data & 1);
    data >>= 1;
    mraa_gpio_write(db4, data & 1);
    data >>= 1;
    mraa_gpio_write(db5, data & 1);
    data >>= 1;
    mraa_gpio_write(db6, data & 1);
    data >>= 1;
    mraa_gpio_write(db7, data & 1);
    
    mraa_gpio_write(e, 1);
    nanoDelay(TWH);
    
    mraa_gpio_write(e, 0);
    nanoDelay(TWL);
}

//not implemented because of read instruction
//int readDisplayData()
//{
//    return 0;
//}

//returns pointer to new frame buffer with given dimensions (192x64)
FrameBuffer * FBcreate( x, y)
{
    FrameBuffer * fb = malloc(sizeof(FrameBuffer));
    if (!fb)
    {
        fprintf(stderr, "Unable to allocate memory for framebuffer\n");
        return NULL;
    }
    
    fb->pixel = (FrameBufferPixel*)malloc(x * y * sizeof(FrameBufferPixel));
    
    if (!fb->pixel)
    {
        fprintf(stderr, "Unable to allocate memory for frame buffer pixel\n");
        return NULL;
    }
    
    fb->x = x;
    fb->y = y;
    
    return fb;
}

//update all lcd controllers (horicontally aligned) with buffer data
int FBwrite(FrameBuffer * fb)
{
    //loop through all 3 controllers of this particular display - device dependend
    int page;
    for (page=0;page<fb->y/8;page++)
    {
        int controller = 0;
        for(;controller<(int)(fb->x/DISP_CONTROLLER_WIDTH);controller++)
        {
            DISPselectController(controller);
            
            //set page address in lcd
            KSsetPage(page);
            
            //write row to lcd
            int address = 0;
            KSsetAddress(address);
            for (;address<DISP_CONTROLLER_WIDTH;address++)
            {
                //create the data to write to the address by collecting bits
                int c = 0;
                int bit = 0;
                for (;bit<8;bit++)
                {
                    //get fb address and fetch pixel value
                    if (fb->pixel[(((page*8)+bit)*fb->x)+((controller*DISP_CONTROLLER_WIDTH)+address)] > 127)
                    {
                        c |= 1 << bit;
                    }
                }
                
                //write data to lcd
                KSwriteDisplayData(c);
            }
        }
    }
    
    return 0;
}
//update all lcd controllers (horicontally aligned) with buffer data
int FBupdateRect(FrameBuffer * fb, int x, int y, int width, int height)
{
    //loop through all 3 controllers of this particular display - device dependend
    int page = y/8;
    for (page = y/8; page <= (y+height-1) / 8; page++)
    {
        int controller = (x / DISP_CONTROLLER_WIDTH);
        for(;controller<=(int)((x+width-1) / DISP_CONTROLLER_WIDTH); controller++)
        {
            if (controller < 0)
                continue;
            if (controller >= DISP_CONTROLLER_COUNT)
                break;

            DISPselectController(controller);
            
            //set page address in lcd
            KSsetPage(page);
            
            //write row to lcd
            int address = controller * DISP_CONTROLLER_WIDTH;
            if (address < x)
            {
                address = x - (controller * DISP_CONTROLLER_WIDTH);
            }

            KSsetAddress(address);
            for (;(address<(x + width)) && (address < ((controller+1) * DISP_CONTROLLER_WIDTH));address++)
            {
                //create the data to write to the address by collecting bits
                int c = 0;
                int bit = 0;
                for (;bit<8;bit++)
                {
                    //get fb address and fetch pixel value
                    if (fb->pixel[(((page*8)+bit)*fb->x)+address] > 127)
                    {
                        c |= 1 << bit;
                    }
                }
                
                //write data to lcd
                KSwriteDisplayData(c);
            }
        }
    }

    return 0;
}

//fills fb with grid
void FBgrid(FrameBuffer * fb, int width)
{
    int x,y;
    for (x=0;x<fb->x;x++)
    {
        for (y=0;y<fb->y;y++)
        {
            if ((y%width==0)||(x%width==0))
            {
                fb->pixel[x+(y*fb->x)] = 255;
            }
            else
            {
                fb->pixel[x+(y*fb->x)] = 0;
            }
        }
    }
}
