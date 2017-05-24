#include "gfx.h"

#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

// deoarece pe y ecranul incepe de la pixelul 33, avem nevoie de un offset
#define OFFSET _height - _width 

//draw START 
int snake[80];
typedef struct {
    uint8_t x;
    uint8_t y;
} coord;
coord snake_coord[80];
int direction;
short x1Obst1 = 10;
short y1Obst1 = 10;
short x2Obst1 = 100;
short y2Obst1 = 10;

short x1Obst2 = 60;
short y1Obst2 = 10;
short x2Obst2 = 60;
short y2Obst2 = 60;

short x1Obst3 = 110;
short y1Obst3 = 20;
short x2Obst3 = 110;
short y2Obst3 = 110;


void initGame(){
	int i;
	for(i = 60; i < 80; i++ )
		snake[i] = 1;
	for(i = 0; i < 60; i++ )
		snake[i] = 0;
	for(i = 0; i < 10; i++ )
		direction = 0;
	for(i = 0; i < 80; i++ )
		if(snake[i] == 1){
			snake_coord[i].x = i;
			snake_coord[i].y = 30;
		}
}

void draw_snake() {
	int i;
	for(i = 0; i < 80; i++ )
		if(snake[i] == 1){
			LCD_draw_pixel(snake_coord[i].x, snake_coord[i].y, 64);
		}
}

void update_snake(){
//right
	int i;
	if(direction == 1){
		for(i = 80; i > 0; i-- )
			if(snake[i] == 1) {
				snake_coord[i].x = snake_coord[i-1].x;
				snake_coord[i].y = snake_coord[i-1].y;
			}
		snake_coord[i].x += 1; 
	}
}


void drawObstacles() {
	GFX_draw_line(10, 10, 100, 10, 64);
	GFX_draw_line(60, 10, 60, 60, 64);
	GFX_draw_line(110, 20, 110, 110, 64);
}

void gameStart(){
	GFX_draw_line(10, 10, 30, 10, 64);
	GFX_draw_line(10, 10, 10, 30, 64);
	GFX_draw_line(10, 30, 30, 30, 64);
	GFX_draw_line(30, 30, 30, 50, 64);
	GFX_draw_line(30, 50, 10, 50, 64);
}

int main(void){
    // init the lcd display
    LCD_init();
	initGame();
	
	LCD_fill_screen(126);
	//GFX_draw_line(100, 100, 120, 120, 64);
	
	DDRA &= ~((1 << PA7));
    PORTA |= (1 << PA7);

    while(1){
		
		while((PINA & (1 << PA7)) != 0);

		if((PINA & (1 << PA7))==0)
        {
            GFX_draw_line(10, 10, 120, 120, 64);
			//LCD_fill_screen(1);
        }

		_delay_ms(10);
    }

    return 0;
}
