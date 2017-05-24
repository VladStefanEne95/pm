#include "gfx.h"

#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

// deoarece pe y ecranul incepe de la pixelul 33, avem nevoie de un offset
#define OFFSET _height - _width 

int main(void){
    // init the lcd display
    LCD_init();
	
	LCD_fill_screen(126);
	//GFX_draw_line(100, 100, 120, 120, 64);
	
	DDRA &= ~((1 << PA7));
    PORTA |= (1 << PA7);

    while(1){

		//LCD_fill_screen(126);
		/*LCD_draw_pixel(10, 10, 64);
		LCD_draw_pixel(11, 10, 64);
		LCD_draw_pixel(12, 10, 64);
		LCD_draw_pixel(13, 10, 64);
		LCD_draw_pixel(14, 10, 64);
		LCD_draw_pixel(15, 10, 64);*/
		
		while((PINA & (1 << PA7)) != 0);

		if((PINA & (1 << PA7))==0)
        {
            GFX_draw_line(10, 10, 120, 120, 64);
			//LCD_fill_screen(1);
        }

		_delay_ms(10);
		//GFX_draw_line(10, 10, 20, 20, 64);
    }

    return 0;
}
