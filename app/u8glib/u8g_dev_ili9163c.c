/*

  u8g_dev_ili9163c.c

  ILI9163c u8glib driver

  (c) 2015-06-24 Jens Hauke <jens.hauke@4k2.de>

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "u8g.h"
#include "ili9163.h"

#define WIDTH 128
#define HEIGHT 128
#define PAGE_HEIGHT 128

static inline
uint16_t u8g_ili9163c_get_color(u8g_dev_arg_pixel_t *pixel)
{
    return pixel->color | ((uint16_t)pixel->hi_color) << 8;
}

static
void u8g_ili9163c_Set8Pixel(u8g_dev_arg_pixel_t *arg_pixel)
{
    uint8_t pixel = arg_pixel->pixel;
    u8g_uint_t dx = 0;
    u8g_uint_t dy = 0;
    uint16_t color = u8g_ili9163c_get_color(arg_pixel);

    switch( arg_pixel->dir )
    {
    case 0: dx++; break;
    case 1: dy++; break;
    case 2: dx--; break;
    case 3: dy--; break;
    }

    do
    {
	if ( pixel & 128 )
	    ili9163_drawPixel(arg_pixel->x, arg_pixel->y, color);
	arg_pixel->x += dx;
	arg_pixel->y += dy;
	pixel <<= 1;
    } while( pixel != 0  );
}


uint8_t u8g_dev_ili9163c_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
    uint8_t pixel;
    uint16_t scaled_pixel;
    uint8_t i;
    uint8_t dir;
    u8g_uint_t x, y, xx,yy;

    switch(msg) {
    case U8G_DEV_MSG_INIT:
	ili9163_begin();
	break;
    case U8G_DEV_MSG_SET_8PIXEL:
	u8g_ili9163c_Set8Pixel((u8g_dev_arg_pixel_t *)arg);
	break;
    case U8G_DEV_MSG_SET_PIXEL: {
	u8g_dev_arg_pixel_t *p = (u8g_dev_arg_pixel_t *)arg;
	ili9163_drawPixel(p->x, p->y, u8g_ili9163c_get_color(p));
	break;
    }
    case U8G_DEV_MSG_PAGE_FIRST:
	ili9163_clearScreen();
	break;
    case U8G_DEV_MSG_PAGE_NEXT:
	return 0;
    case U8G_DEV_MSG_GET_PAGE_BOX: {
	u8g_box_t *box = (u8g_box_t *)arg;
	box->x0 = 0;
	box->x1 = WIDTH;
	box->y0 = 0;
	box->y1 = HEIGHT;
	return 1;
    }
    case U8G_DEV_MSG_GET_WIDTH:
	*((u8g_uint_t *)arg) = WIDTH;
	break;
    case U8G_DEV_MSG_GET_HEIGHT:
	*((u8g_uint_t *)arg) = HEIGHT;
	break;
    case U8G_DEV_MSG_GET_MODE:
	return U8G_MODE_HICOLOR;
    }
    return 1;
}


u8g_dev_t u8g_dev_ili9163c = { u8g_dev_ili9163c_fn, NULL, NULL };
