#include "bcm2835.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>


#include "ascii hex(8x16).h"
#include "GB2312.h"

#define RGB565(r,g,b)((r >> 3) << 11 | (g >> 2) << 5 | ( b >> 3))
#define BCM2708SPI
#define ROTATE90


#define RGB565_MASK_RED        0xF800
#define RGB565_MASK_GREEN    0x07E0
#define RGB565_MASK_BLUE       0x001F


#ifdef    ROTATE90
#define XP    0x0201
#define YP    0x0200
#define XS    0x0212
#define YS    0x0210
#define XE    0x0213
#define YE    0x0211
#define MAX_X    319
#define MAX_Y    239
#else
#define XP    0x0200
#define YP    0x0201
#define XS    0x0210
#define YS    0x0212
#define XE    0x0211
#define YE    0x0213
#define MAX_X    239
#define MAX_Y    319
#endif

#define	SPICS	RPI_GPIO_P1_26	//7
#define	SPIRS	RPI_GPIO_P1_21	//8
#define	SPIRST	RPI_GPIO_P1_22	//25
#define	SPISCL	RPI_GPIO_P1_23	//GPIO11
#define	SPISCI	RPI_GPIO_P1_19	//GPIO10
#if 0
#define LCD_CS_CLR	bcm2835_gpio_write(SPICS,0)
#define LCD_RS_CLR	bcm2835_gpio_write(SPIRS,0)
#define LCD_RST_CLR	bcm2835_gpio_write(SPIRST,0)
#define LCD_SCL_CLR	bcm2835_gpio_write(SPISCL,0)
#define LCD_SCI_CLR	bcm2835_gpio_write(SPISCI,0)

#define LCD_CS_SET	bcm2835_gpio_write(SPICS,1)
#define LCD_RS_SET	bcm2835_gpio_write(SPIRS,1)
#define LCD_RST_SET	bcm2835_gpio_write(SPIRST,1)
#define LCD_SCL_SET	bcm2835_gpio_write(SPISCL,1)
#define LCD_SCI_SET	bcm2835_gpio_write(SPISCI,1)
#else
#define LCD_CS_CLR	bcm2835_gpio_clr(SPICS)
#define LCD_RS_CLR	bcm2835_gpio_clr(SPIRS)
#define LCD_RST_CLR	bcm2835_gpio_clr(SPIRST)
#define LCD_SCL_CLR	bcm2835_gpio_clr(SPISCL)
#define LCD_SCI_CLR	bcm2835_gpio_clr(SPISCI)

#define LCD_CS_SET	bcm2835_gpio_set(SPICS)
#define LCD_RS_SET	bcm2835_gpio_set(SPIRS)
#define LCD_RST_SET	bcm2835_gpio_set(SPIRST)
#define LCD_SCL_SET	bcm2835_gpio_set(SPISCL)
#define LCD_SCI_SET	bcm2835_gpio_set(SPISCI)
#endif

short color[]={0xf800,0x07e0,0x001f,0xffe0,0x0000,0xffff,0x07ff,0xf81f};


void LCD_WR_REG(int index)
{
	char i;
	LCD_CS_CLR;
	LCD_RS_CLR;
#ifdef BCM2708SPI
	bcm2835_spi_transfer(index>>8);
	bcm2835_spi_transfer(index);
#else
    
	for(i=0;i<16;i++)
	{
		LCD_SCL_CLR;
		if(index&0x8000)
		{
			LCD_SCI_SET;
		}
		else
		{
			LCD_SCI_CLR;
		}
		index=index<<1;
		LCD_SCL_SET;
	}
#endif
	LCD_CS_SET;
    
}


void LCD_WR_CMD(int index,int val)
{
	char i;
	LCD_CS_CLR;
	LCD_RS_CLR;
#ifdef BCM2708SPI
	bcm2835_spi_transfer(index>>8);
	bcm2835_spi_transfer(index);
#else
	for(i=0;i<16;i++)
	{
		LCD_SCL_CLR;
		if(index&0x8000)
		{
			LCD_SCI_SET;
		}
		else
		{
			LCD_SCI_CLR;
		}
		index=index<<1;
		LCD_SCL_SET;
	}
#endif
	LCD_RS_SET;
#ifdef BCM2708SPI
	bcm2835_spi_transfer(val>>8);
	bcm2835_spi_transfer(val);
#else
	for(i=0;i<16;i++)
	{
		LCD_SCL_CLR;
		if(val&0x8000)
		{
			LCD_SCI_SET;
		}
		else
		{
			LCD_SCI_CLR;
		}
		val=val<<1;
		LCD_SCL_SET;
	}
#endif
	LCD_CS_SET;
}

void inline LCD_WR_Data(int val)
{
	char i;
#ifdef BCM2708SPI
	bcm2835_spi_transfer(val>>8);
	bcm2835_spi_transfer(val);
#else
	for(i=0;i<16;i++)
	{
		LCD_SCL_CLR;
		if(val&0x8000)
		{
			LCD_SCI_SET;
		}
		else
		{
			LCD_SCI_CLR;
		}
		val=val<<1;
		LCD_SCL_SET;
	}
#endif
}



void LCD_Init()
{
	LCD_RST_CLR;
	delay (100);
	LCD_RST_SET;
	delay (100);
    
	LCD_WR_CMD( 0x000, 0x0001 ); /* oschilliation start */
	delay ( 1 );
	/* Power settings */
	LCD_WR_CMD( 0x100, 0x0000 ); /*power supply setup*/
	LCD_WR_CMD( 0x101, 0x0000 );
	LCD_WR_CMD( 0x102, 0x3110 );
	LCD_WR_CMD( 0x103, 0xe200 );
	LCD_WR_CMD( 0x110, 0x009d );
	LCD_WR_CMD( 0x111, 0x0022 );
	LCD_WR_CMD( 0x100, 0x0120 );
	delay ( 2 );
    
	LCD_WR_CMD( 0x100, 0x3120 );
	delay ( 8 );
	/* Display control */
    
	LCD_WR_CMD( 0x002, 0x0000 );
#ifdef	ROTATE90
	LCD_WR_CMD( 0x001, 0x0000 );
	LCD_WR_CMD( 0x003, 0x12B8 );
#else
	LCD_WR_CMD( 0x001, 0x0100 );
	LCD_WR_CMD( 0x003, 0x1230 );
#endif
	LCD_WR_CMD( 0x006, 0x0000 );
	LCD_WR_CMD( 0x007, 0x0101 );
	LCD_WR_CMD( 0x008, 0x0808 );
	LCD_WR_CMD( 0x009, 0x0000 );
	LCD_WR_CMD( 0x00b, 0x0000 );
	LCD_WR_CMD( 0x00c, 0x0000 );
	LCD_WR_CMD( 0x00d, 0x0018 );
	/* LTPS control settings */
	LCD_WR_CMD( 0x012, 0x0000 );
	LCD_WR_CMD( 0x013, 0x0000 );
	LCD_WR_CMD( 0x018, 0x0000 );
	LCD_WR_CMD( 0x019, 0x0000 );
    
	LCD_WR_CMD( 0x203, 0x0000 );
	LCD_WR_CMD( 0x204, 0x0000 );
    
	LCD_WR_CMD( 0x210, 0x0000 );
	LCD_WR_CMD( 0x211, 0x00ef );
	LCD_WR_CMD( 0x212, 0x0000 );
	LCD_WR_CMD( 0x213, 0x013f );
	LCD_WR_CMD( 0x214, 0x0000 );
	LCD_WR_CMD( 0x215, 0x0000 );
	LCD_WR_CMD( 0x216, 0x0000 );
	LCD_WR_CMD( 0x217, 0x0000 );
    
	// Gray scale settings
	LCD_WR_CMD( 0x300, 0x5343);
	LCD_WR_CMD( 0x301, 0x1021);
	LCD_WR_CMD( 0x302, 0x0003);
	LCD_WR_CMD( 0x303, 0x0011);
	LCD_WR_CMD( 0x304, 0x050a);
	LCD_WR_CMD( 0x305, 0x4342);
	LCD_WR_CMD( 0x306, 0x1100);
	LCD_WR_CMD( 0x307, 0x0003);
	LCD_WR_CMD( 0x308, 0x1201);
	LCD_WR_CMD( 0x309, 0x050a);
    
	/* RAM access settings */
	LCD_WR_CMD( 0x400, 0x4027 );
	LCD_WR_CMD( 0x401, 0x0000 );
	LCD_WR_CMD( 0x402, 0x0000 );	/* First screen drive position (1) */
	LCD_WR_CMD( 0x403, 0x013f );	/* First screen drive position (2) */
	LCD_WR_CMD( 0x404, 0x0000 );
    
	LCD_WR_CMD( 0x200, 0x0000 );
	LCD_WR_CMD( 0x201, 0x0000 );
    
	LCD_WR_CMD( 0x100, 0x7120 );
	LCD_WR_CMD( 0x007, 0x0103 );
	delay( 1 );
	LCD_WR_CMD( 0x007, 0x0113 );
}



int main (void)
{
    printf("bcm2835 init now\n");
    if (!bcm2835_init())
    {
        printf("bcm2835 init error\n");
        return 1;
    }
    bcm2835_gpio_fsel(SPICS, BCM2835_GPIO_FSEL_OUTP) ;
    bcm2835_gpio_fsel(SPIRS, BCM2835_GPIO_FSEL_OUTP) ;
    bcm2835_gpio_fsel(SPIRST, BCM2835_GPIO_FSEL_OUTP) ;
    
#ifdef BCM2708SPI
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_2);
    
    //bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
    //bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1,LOW);
#else
    bcm2835_gpio_fsel(SPISCL, BCM2835_GPIO_FSEL_OUTP) ;
    bcm2835_gpio_fsel(SPISCI, BCM2835_GPIO_FSEL_OUTP) ;
#endif
    printf ("Raspberry Pi MZTX06A LCD initialising...\n") ;
    printf ("http://jwlcd-tp.taobao.com\n") ;
    
    //  loadPGM("etao.pbm");
    //  for (;;)
    //{
	LCD_Init();
//	loadFrameBuffer_diff();
	//LCD_showbuffer();
	//LCD_test();
	//LCD_clear(RGB565(130,130,150));
	//LCD_clear(0x00);
	//LCD_QQ();
    //draw_circle(100,100,50,RGB565(130,30,220));
	//draw_circle(200,10, 20,RGB565(100,2,23));
	//draw_circle(30,160, 80, RGB565(22,134,100)); 
    //DisplayString("Windows",1,0);
	//write_dot(10,10,0xffff);
	//delay(500);
    //y}
    return 0 ;
}

