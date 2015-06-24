#ifndef _ILI9163_PRIV_H_
#define _ILI9163_PRIV_H_
/*
  (c) 2015-06-24 Jens Hauke <jens.hauke@4k2.de>
  largely based on work of Sumotoy:
     https://github.com/sumotoy/TFT_ILI9163C
     Copyright (c) 2014, .S.U.M.O.T.O.Y., coded by Max MC Costa.
  -------------------------------------------------------------------------------
    TFT_ILI9163C Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TFT_ILI9163C Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Based on https://github.com/sumotoy/TFT_ILI9163C */

//DID YOU HAVE A RED PCB, BLACk PCB or WHAT DISPLAY TYPE???????????? ---> SELECT HERE <----
#define __144_RED_PCB__//128x128
//#define __144_BLACK_PCB__//128x128
//#define __22_RED_PCB__//240x320
//---------------------------------------

#if defined(__144_RED_PCB__)
/*
This display:
http://www.ebay.com/itm/Replace-Nokia-5110-LCD-1-44-Red-Serial-128X128-SPI-Color-TFT-LCD-Display-Module-/271422122271
This particular display has a design error! The controller has 3 pins to configure to constrain
the memory and resolution to a fixed dimension (in that case 128x128) but they leaved those pins
configured for 128x160 so there was several pixel memory addressing problems.
I solved by setup several parameters that dinamically fix the resolution as needed so below
the parameters for this diplay. If you have a strain or a correct display (can happen with chinese)
you can copy those parameters and create setup for different displays.
*/
	#define _TFTWIDTH		128//the REAL W resolution of the TFT
	#define _TFTHEIGHT		128//the REAL H resolution of the TFT
	#define _GRAMWIDTH		128
	#define _GRAMHEIGH		160//160
	#define _GRAMSIZE		_GRAMWIDTH * _GRAMHEIGH//*see note 1
	#define __COLORSPC		1// 1:GBR - 0:RGB
	#define __GAMMASET3		//uncomment for another gamma
	#define __OFFSET		32//*see note 2
	//Tested!
#elif defined (__144_BLACK_PCB__)
	#define _TFTWIDTH		128//the REAL W resolution of the TFT
	#define _TFTHEIGHT		128//the REAL H resolution of the TFT
	#define _GRAMWIDTH		128
	#define _GRAMHEIGH		128
	#define _GRAMSIZE		_GRAMWIDTH * _GRAMHEIGH//*see note 1
	#define __COLORSPC		1// 1:GBR - 0:RGB
	#define __GAMMASET1		//uncomment for another gamma
	#define __OFFSET		0
	//not tested
#elif defined (__22_RED_PCB__)
/*
Like this one:
http://www.ebay.it/itm/2-2-Serial-SPI-TFT-LCD-Display-Module-240x320-Chip-ILI9340C-PCB-Adapter-SD-Card-/281304733556
Not tested!
*/
	#define _TFTWIDTH		240//the REAL W resolution of the TFT
	#define _TFTHEIGHT		320//the REAL H resolution of the TFT
	#define _GRAMWIDTH		240
	#define _GRAMHEIGH		320
	#define _GRAMSIZE		_GRAMWIDTH * _GRAMHEIGH
	#define __COLORSPC		1// 1:GBR - 0:RGB
	#define __GAMMASET1		//uncomment for another gamma
	#define __OFFSET		0
#else
	#define _TFTWIDTH		128//128
	#define _TFTHEIGHT		160//160
	#define _GRAMWIDTH		128
	#define _GRAMHEIGH		160
	#define _GRAMSIZE		_GRAMWIDTH * _GRAMHEIGH
	#define __COLORSPC		1// 1:GBR - 0:RGB
	#define __GAMMASET1
	#define __OFFSET		0
#endif
/*
	Note 1: The __144_RED_PCB__ display has hardware addressing of 128 x 160
	but the tft resolution it's 128 x 128 so the dram should be set correctly

	Note 2: This is the offset between image in RAM and TFT. In that case 160 - 128 = 32;
*/
//--------- Keep out hands from here!-------------

#define	BLACK			0x0000
#define WHITE			0xFFFF

//ILI9163C registers-----------------------
#define CMD_NOP		0x00//Non operation
#define CMD_SWRESET	0x01//Soft Reset
#define CMD_SLPIN	0x10//Sleep ON
#define CMD_SLPOUT	0x11//Sleep OFF
#define CMD_PTLON	0x12//Partial Mode ON
#define CMD_NORML	0x13//Normal Display ON
#define CMD_DINVOF	0x20//Display Inversion OFF
#define CMD_DINVON	0x21//Display Inversion ON
#define CMD_GAMMASET	0x26//Gamma Set (0x01[1],0x02[2],0x04[3],0x08[4])
#define CMD_DISPOFF	0x28//Display OFF
#define CMD_DISPON	0x29//Display ON
#define CMD_IDLEON	0x39//Idle Mode ON
#define CMD_IDLEOF	0x38//Idle Mode OFF
#define CMD_CLMADRS	0x2A//Column Address Set
#define CMD_PGEADRS	0x2B//Page Address Set

#define CMD_RAMWR	0x2C//Memory Write
#define CMD_RAMRD	0x2E//Memory Read
#define CMD_CLRSPACE	0x2D//Color Space : 4K/65K/262K
#define CMD_PARTAREA	0x30//Partial Area
#define CMD_VSCLLDEF	0x33//Vertical Scroll Definition
#define CMD_TEFXLON	0x35//Tearing Effect Line ON
#define CMD_TEFXLOF	0x34//Tearing Effect Line OFF
#define CMD_MADCTL	0x36//Memory Access Control
#define CMD_VSSTADRS	0x37//Vertical Scrolling Start address
#define CMD_PIXFMT	0x3A//Interface Pixel Format
#define CMD_FRMCTR1	0xB1//Frame Rate Control (In normal mode/Full colors)
#define CMD_FRMCTR2	0xB2//Frame Rate Control(In Idle mode/8-colors)
#define CMD_FRMCTR3	0xB3//Frame Rate Control(In Partial mode/full colors)
#define CMD_DINVCTR	0xB4//Display Inversion Control
#define CMD_RGBBLK	0xB5//RGB Interface Blanking Porch setting
#define CMD_DFUNCTR	0xB6//Display Fuction set 5
#define CMD_SDRVDIR	0xB7//Source Driver Direction Control
#define CMD_GDRVDIR	0xB8//Gate Driver Direction Control

#define CMD_PWCTR1	0xC0//Power_Control1
#define CMD_PWCTR2	0xC1//Power_Control2
#define CMD_PWCTR3	0xC2//Power_Control3
#define CMD_PWCTR4	0xC3//Power_Control4
#define CMD_PWCTR5	0xC4//Power_Control5
#define CMD_VCOMCTR1	0xC5//VCOM_Control 1
#define CMD_VCOMCTR2	0xC6//VCOM_Control 2
#define CMD_VCOMOFFS	0xC7//VCOM Offset Control
#define CMD_PGAMMAC	0xE0//Positive Gamma Correction Setting
#define CMD_NGAMMAC	0xE1//Negative Gamma Correction Setting
#define CMD_GAMRSEL	0xF2//GAM_R_SEL


#endif /* _ILI9163_PRIV_H_ */
