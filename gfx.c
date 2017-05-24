#include "gfx.h"
#include "glcdfont.h"

#include <avr/pgmspace.h>
#include <stdlib.h>

#define swap(a, b) { a = a ^ b; b = a ^ b; a = a ^ b; }

static int16_t display_width, display_height;
static int16_t cursor_x, cursor_y;

static uint16_t text_color;

static uint8_t text_size;
static uint8_t rotation;
static uint8_t text_wrap;

static void draw_fast_v_line(int16_t x, int16_t y,
			     int16_t height,
			     uint16_t color)
{
	LCD_set_range(x, y, x, y + height - 1);

	while (height--)
		LCD_set_pixel(color);
}


static void draw_fast_h_line(int16_t x, int16_t y,
			     int16_t width,
			     uint16_t color)
{
	LCD_set_range(x, y, x + width - 1, y);

	while (width--)
		LCD_set_pixel(color);
}

void GFX_init(int16_t width, int16_t height)
{
	display_width = width;
	display_height = height;

	cursor_y = cursor_x = 0;

	rotation = 0;

	text_size = 1;
	text_color = 0xFFFF;
	text_wrap = 1;
}

void GFX_draw_line(int16_t x1, int16_t y1,
		   int16_t x2, int16_t y2,
		   uint16_t color)
{
	int16_t dx, dy;
	int16_t err, ystep;
	int16_t gradient = abs(y2 - y1) > abs(x2 - x1);

	if (gradient) {
		swap(x1, y1);
		swap(x2, y2);
	}

	if (x1 > x2) {
		swap(x1, x2);
		swap(y1, y2);
	}

	dx = x2 - x1;
	dy = abs(y2 - y1);
	err = dx / 2;

	if (y1 < y2)
		ystep = 1;
	else
		ystep = -1;

	for (; x1 <= x2; x1++) {
		if (gradient)
			LCD_draw_pixel(y1, x1, color);
		else
			LCD_draw_pixel(x1, y1, color);
		
		err -= dy;
		
		if (err < 0) {
			y1 += ystep;
			err += dx;
		}
	}
}

void GFX_draw_rect(int16_t x, int16_t y,
		   int16_t width, int16_t height,
		   uint16_t color, uint8_t fill)
{
	if ((x >= display_width) || (y >= display_height))
		return;

	if ((x + width - 1) >= display_width)
		width = display_width  - x;

	if ((y + height - 1) >= display_height)
		height = display_height - y;
		
	if (!fill) {
		draw_fast_v_line(x, y, height, color);
		draw_fast_h_line(x, y, width, color);
		draw_fast_v_line(x + width, y, height, color);
		draw_fast_h_line(x, y + height, width, color);
	} else {
		LCD_set_range(x, y, x + width - 1, y + height - 1);

		for (y = height; y > 0; y--)
			for (x = width; x > 0; x--)
				LCD_set_pixel(color);
	}
}



static void put_char(int16_t x, int16_t y,
		     unsigned char chr,
		     uint16_t color,
		     uint8_t size)
{
	int8_t i, j;

	if((x >= display_width) ||
	   (y >= display_height) ||
	   ((x + 5 * size - 1) < 0) ||
	   ((y + 8 * size - 1) < 0))
		return;

	for (i = 0; i < 6; i++) {
		uint8_t line;
		
		if (i == 5)
			line = 0x0;
		else
			line = pgm_read_byte((chr * 5) + i);
		
		for (j = 0; j < 8; j++) {
			if (line & 0x1) {
				if (size == 1) {
					if(rotation % 2 != 0)
						LCD_draw_pixel(y + j,
							       display_width - x - i - size,
							       color);
					else
						LCD_draw_pixel(x + i, y + j, color);
				} else {
					if(rotation % 2 != 0)
						GFX_draw_rect(y + (j * size),
							      display_width - x - (i * size) - size,
							      size, size, color, 1);
					else
						GFX_draw_rect(x + (i * size), y + (j * size),
							      size, size, color, 1);
				}
			}
			
			line >>= 1;
		}
	}
}

void GFX_print_char(const char chr)
{
	if (chr == '\n') {
		cursor_y += text_size * 8;
		cursor_x = 0;
	} else if (chr == '\r') {
		;
	} else {
		put_char(cursor_x, cursor_y, chr,
			 text_color, text_size);

		cursor_x += text_size * 6;

		if (text_wrap &&
		    (cursor_x > (display_width - text_size * 6))) {
			cursor_y += text_size * 8;
			cursor_x = 0;
		}
	}
}

void GFX_print_str(const char str[])
{
	int x = 0;

	while (str[x])
		GFX_print_char(str[x++]);
}

void GFX_set_cursor(int16_t x, int16_t y)
{
	cursor_x = x;
	cursor_y = y;
}

void GFX_set_text_color(uint16_t color)
{
	text_color = color;
}

void GFX_set_text_size(uint8_t size)
{
	text_size = (size > 0) ? size : 1;
}

void GFX_set_text_wrap(uint8_t wrap)
{
	text_wrap = wrap;
}

int16_t GFX_get_width(void)
{
	return display_width;
}

int16_t GFX_get_height(void)
{
	return display_height;
}

void GFX_set_rotation(uint8_t rot)
{
	rotation = rot % 4;

	switch (rotation) {
	case 0:
	case 2:
		display_width = display_width;
		display_height = display_height;

		break;
	case 1:
	case 3:
		display_width = display_height;
		display_height = display_width;

		break;
	}
}

uint8_t GFX_get_rotation(void)
{
	return rotation;
}
