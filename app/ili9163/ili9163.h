#ifndef _ILI9163_H_
#define _ILI9163_H_
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
*/
#include "c_types.h"

void ili9163_begin(void);
void ili9163_clearScreen(void);
void ili9163_fillScreen(uint16_t color);
void ili9163_drawPixel(uint16_t x, uint16_t y, uint16_t color);

#endif /* _ILI9163_H_ */
