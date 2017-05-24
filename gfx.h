#ifndef GFX_H_
#define GFX_H_	1

#include <avr/io.h>
#include "lcd.h"

void GFX_init(int16_t width, int16_t height);

void GFX_draw_line(int16_t x1, int16_t y1,
		   int16_t x2, int16_t y2,
		   uint16_t color);








void GFX_print_char(const char chr);
void GFX_print_str(const char str[]);

void GFX_set_cursor(int16_t x, int16_t y);
void GFX_set_text_color(uint16_t color);
void GFX_set_text_size(uint8_t size);
void GFX_set_text_wrap(uint8_t wrap);

int16_t GFX_get_width(void);
int16_t GFX_get_height(void);

void GFX_set_rotation(uint8_t rot);
uint8_t GFX_get_rotation(void);

#endif /* GFX_H_ */
