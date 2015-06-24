// (c) 2015-06-24 Jens Hauke <jens.hauke@4k2.de>

#include "ili9163.h"
#include "ili9163_priv.h"

#include "platform.h"
#include "osapi.h"

#include "c_types.h"


#define PULLUP PLATFORM_GPIO_PULLUP
#define FLOAT PLATFORM_GPIO_FLOAT
#define OUTPUT PLATFORM_GPIO_OUTPUT
#define INPUT PLATFORM_GPIO_INPUT
#define INTERRUPT PLATFORM_GPIO_INT
#define HIGH PLATFORM_GPIO_HIGH
#define LOW PLATFORM_GPIO_LOW

#define SCK	4  /* GPIO2 */
#define SDA	3  /* GPIO0 */
#define A0	9  /* GPIO3(RXD) */
#define CS	10 /* GPIO1(TXD) */

static
uint8_t rotation = 0;
int16_t _width, _height;

#if 0
// Disable SPI
#define platform_gpio_write(x, y)
#define platform_gpio_mode(x, y, z)
#endif

static
void spiwrite(uint8_t c) {
	uint8_t cnt;
	for (cnt = 0; cnt < 8; cnt++) {
		// digitalWrite(PIN_CLK, LOW);

		// digitalWrite(PIN_DATA, c & 128 ? HIGH : LOW);
		platform_gpio_write(SCK, LOW);
		platform_gpio_write(SDA, c & 128 ? HIGH : LOW);
		platform_gpio_write(SCK, HIGH);
		c = c << 1;
	}
}


static
void ili9163_writecommand(uint8_t c){
	platform_gpio_write(A0, LOW);
	platform_gpio_write(CS, LOW);
	spiwrite(c);
	platform_gpio_write(CS, HIGH);
}


static
void ili9163_writedata(uint8_t c){
	platform_gpio_write(A0, HIGH);
	platform_gpio_write(CS, LOW);
	spiwrite(c);
	platform_gpio_write(CS, HIGH);
}


static
void ili9163_writedata16(uint16_t d){
	platform_gpio_write(A0, HIGH);
	platform_gpio_write(CS, LOW);
	spiwrite(d >> 8);
	spiwrite(d);
	platform_gpio_write(CS, HIGH);
}


static
void ili9163_writedata16_bulk(uint16_t d, uint16_t count){
	uint8_t d_hi = d >> 8;
	platform_gpio_write(A0, HIGH);
	platform_gpio_write(CS, LOW);
	while (count--) {
		spiwrite(d_hi);
		spiwrite(d);
	}
	platform_gpio_write(CS, HIGH);
}


static
void delay(unsigned msec) {
	os_delay_us(msec * 1000);
}


static
void ili9163_setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	ili9163_writecommand(CMD_CLMADRS); // Column
	if (rotation == 0 || rotation > 1){
		ili9163_writedata16(x0);
		ili9163_writedata16(x1);
	} else {
		ili9163_writedata16(x0 + __OFFSET);
		ili9163_writedata16(x1 + __OFFSET);
	}

	ili9163_writecommand(CMD_PGEADRS); // Page
	if (rotation == 0){
		ili9163_writedata16(y0 + __OFFSET);
		ili9163_writedata16(y1 + __OFFSET);
	} else {
		ili9163_writedata16(y0);
		ili9163_writedata16(y1);
	}
	ili9163_writecommand(CMD_RAMWR); //Into RAM
}


static inline
int ili9163_boundaryCheck(uint16_t x, uint16_t y) {
	return x >= _width || y >= _height;
}


void ili9163_drawPixel(uint16_t x, uint16_t y, uint16_t color) {
	if (ili9163_boundaryCheck(x,y)) return;
	ili9163_setAddr(x,y,x+1,y+1);
	ili9163_writedata16(color);
}


static
void ili9163_setRotation(uint8_t m) {
	uint8_t _Mactrl_Data;

	rotation = m % 4; // can't be higher than 3
	switch (rotation) {
	case 0:
		_Mactrl_Data = 0b00001000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	case 1:
		_Mactrl_Data = 0b01101000;
		_width  = _TFTHEIGHT;//-__OFFSET;
		_height = _TFTWIDTH;
		break;
	case 2:
		_Mactrl_Data = 0b11001000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	case 3:
		_Mactrl_Data = 0b10101000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	}
	// colorSpace(__COLORSPC);
	ili9163_writecommand(CMD_MADCTL);
	ili9163_writedata(_Mactrl_Data);
}


void ili9163_fillScreen(uint16_t color) {
	int px;

	//writecommand(CMD_RAMWR);
	ili9163_setAddr(0x00, 0x00, _GRAMWIDTH, _GRAMHEIGH);//go home
	ili9163_writedata16_bulk(color, _GRAMSIZE);
}


void ili9163_clearScreen(void) {
	ili9163_fillScreen(0x0000 /* Black */);
}



static
void ili9163_init(void) {
	uint8_t i;

#if defined(__GAMMASET1)
	const uint8_t pGammaSet[15]= {0x36,0x29,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x12,0x0A,0x11,0x0B,0x06};
	const uint8_t nGammaSet[15]= {0x09,0x16,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x34,0x39};
#elif defined(__GAMMASET2)
	const uint8_t pGammaSet[15]= {0x3F,0x21,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x02,0x0A,0x01,0x00,0x00};
	const uint8_t nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
#elif defined(__GAMMASET3)
	const uint8_t pGammaSet[15]= {0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00};
	//&const uint8_t nGammaSet[15]= {0x00,0x19,0x1C,0x0F,0x14,0x0F,0x2A,0x48,0x3F,0x06,0x1D,0x21,0x3D,0x3F,0x3F};
	const uint8_t nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
#else
	const uint8_t pGammaSet[15]= {0x3F,0x25,0x1C,0x1E,0x20,0x12,0x2A,0x90,0x24,0x11,0x00,0x00,0x00,0x00,0x00};
	const uint8_t nGammaSet[15]= {0x20,0x20,0x20,0x20,0x05,0x15,0x00,0xA7,0x3D,0x18,0x25,0x2A,0x2B,0x2B,0x3A};
#endif

	ili9163_writecommand(CMD_SWRESET);//software reset
	delay(50);
	ili9163_writecommand(CMD_SLPOUT);//exit sleep
	// delay(5);
	ili9163_writecommand(CMD_PIXFMT);//Set Color Format 16bit
	ili9163_writedata(0x05);
	// delay(5);
	ili9163_writecommand(CMD_GAMMASET);//default gamma curve 3
	ili9163_writedata(0x04);//0x04
	// delay(1);
	ili9163_writecommand(CMD_GAMRSEL);//Enable Gamma adj
	ili9163_writedata(0x01);
	// delay(1);
	ili9163_writecommand(CMD_NORML);

	ili9163_writecommand(CMD_DFUNCTR);
	ili9163_writedata(0b11111111);//
	ili9163_writedata(0b00000110);//

	ili9163_writecommand(CMD_PGAMMAC);//Positive Gamma Correction Setting
	for (i=0;i<15;i++){
		ili9163_writedata(pGammaSet[i]);
	}
	ili9163_writecommand(CMD_NGAMMAC);//Negative Gamma Correction Setting
	for (i=0;i<15;i++){
		ili9163_writedata(nGammaSet[i]);
	}

	ili9163_writecommand(CMD_FRMCTR1);//Frame Rate Control (In normal mode/Full colors)
	ili9163_writedata(0x08);//0x0C//0x08
	ili9163_writedata(0x02);//0x14//0x08
	// delay(1);
	ili9163_writecommand(CMD_DINVCTR);//display inversion
	ili9163_writedata(0x07);
	// delay(1);
	ili9163_writecommand(CMD_PWCTR1);//Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
	ili9163_writedata(0x0A);//4.30 - 0x0A
	ili9163_writedata(0x02);//0x05
	// delay(1);
	ili9163_writecommand(CMD_PWCTR2);//Set BT[2:0] for AVDD & VCL & VGH & VGL
	ili9163_writedata(0x02);
	// delay(1);
	ili9163_writecommand(CMD_VCOMCTR1);//Set VMH[6:0] & VML[6:0] for VOMH & VCOML
	ili9163_writedata(0x50);//0x50
	ili9163_writedata(99);//0x5b
	// delay(1);
	ili9163_writecommand(CMD_VCOMOFFS);
	ili9163_writedata(0);//0x40
	// delay(1);

	ili9163_writecommand(CMD_CLMADRS);//Set Column Address
	ili9163_writedata16(0x00);
	ili9163_writedata16(_GRAMWIDTH);

	ili9163_writecommand(CMD_PGEADRS);//Set Page Address
	ili9163_writedata16(0X00);
	ili9163_writedata16(_GRAMHEIGH);
	// set scroll area (thanks Masuda)
	ili9163_writecommand(CMD_VSCLLDEF);
	ili9163_writedata16(__OFFSET);
	ili9163_writedata16(_GRAMHEIGH - __OFFSET);
	ili9163_writedata16(0);
	// ili9163_colorSpace(__COLORSPC);
	ili9163_setRotation(0);
	ili9163_writecommand(CMD_DISPON);//display ON
	// delay(1);
	ili9163_writecommand(CMD_RAMWR);//Memory Write

	// delay(1);
	// ili9163_fillScreen(0x0000);
}


void ili9163_begin(void) {
//    PLATFORM_GPIO_OUTPUT
//    gpio.mode(_GET.on, gpio.OUTPUT)
//    gpio.write(_GET.on, gpio.HIGH)
	/* pin	gpio
	   4	GPIO2
	   3	GPIO0
	   9	GPIO3
	   10	GPIO1
	*/

	platform_gpio_mode(SCK, OUTPUT, FLOAT);
	platform_gpio_mode(SDA, OUTPUT, FLOAT);
	platform_gpio_mode(A0, OUTPUT, FLOAT);
	platform_gpio_mode(CS, OUTPUT, FLOAT);

	platform_gpio_write(SCK, LOW);
	platform_gpio_write(SDA, LOW);
	platform_gpio_write(A0, LOW);
	platform_gpio_write(CS, LOW);

	ili9163_init();
}
