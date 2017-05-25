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
//delay time in ms
int speed = 300;

int food_type;
typedef struct {
    int x;
    int y;
} coord;

typedef struct {
    int x1;
    int y1;
	int x2;
    int y2;
	
} line_coord;

coord snake_coord[80];
coord food;
int head = 79;
int tail = 59;

int direction;
int num_obs = 3;
line_coord obstacle[3];

void initGame(){

	food_type = 0;

	//init obastacles
	obstacle[0].x1 = 10;
	obstacle[0].y1 = 10;
	obstacle[0].x2 = 100;
	obstacle[0].y2 = 10;

	obstacle[1].x1 = 60;
	obstacle[1].y1 = 10;
	obstacle[1].x2 = 60;
	obstacle[1].y2 = 60;

	obstacle[2].x1 = 110;
	obstacle[2].y1 = 20;
	obstacle[2].x2 = 110;
	obstacle[2].y2 = 110;
	
	int i;
	//initial size is 20
	for(i = 60; i < 80; i++ )
		snake[i] = 1;
	//max size is 80
	for(i = 0; i < 60; i++ )
		snake[i] = 0;
	for(i = 0; i < 10; i++ )
		direction = 0;
	//start position
	for(i = 0; i < 80; i++ )
		if(snake[i] == 1){
			snake_coord[i].x = i;
			snake_coord[i].y = 30;
		}
	
	food.x = rand() % 128;
	food.y = rand() % 128;

	//food on the same line as snake
	while(food.y == 30)
		food.y = rand() % 128;
}

void draw_game() {
	int i;
	//draw snake
	for(i = 0; i <= head; i++ )
		if(snake[i] == 1){
			LCD_draw_pixel(snake_coord[i].x, snake_coord[i].y, ST7735_BLACK);
		}
	//draw food
	LCD_draw_pixel(food.x, food.y, ST7735_BLACK);
	
	//draw obstacles
	for (i = 0;i < num_obs; i++) {
		GFX_draw_line(obstacle[i].x1, obstacle[i].y1, obstacle[i].x2, obstacle[i].y2, ST7735_BLACK);
	}
}

void game_over(){};


int check_colision() {
	int colision_type = 0;
	int i;

	//check with food
	if(snake_coord[head].x == food.x && snake_coord[head].y == food.y){
		colision_type = 1;
		tail--;
		snake[tail] = 1;
		food.x = rand() % 128;
		food.y = rand() % 128;
		//power up type
		food_type = rand() % 5;
		for(i = 0; i < head; i++ )
			if(snake[i] == 1) {
				if(snake_coord[head].x == food.x && snake_coord[head].y == food.y){
					food.x = rand() % 128;
					food.y = rand() % 128;
					i = 0;
				}

		}
	}
	//check collision with walls
	for (i = 0;i < num_obs; i++)	
		if(snake_coord[head].x > obstacle[i].x1 && snake_coord[head].x < obstacle[i].x2)
			if(snake_coord[head].y > obstacle[i].x1 && snake_coord[head].x < obstacle[i].y2)
				return 2;
	//check collision with tail
	for(i = 0; i < head; i++ )
		if(snake[i] == 1) {
			if(snake_coord[head].x == snake_coord[i].x && snake_coord[head].x == snake_coord[i].y)
				return 2;
	}
	
	return colision_type;

}



void move_snake(){
//left
	int i;
	if(direction == 1){
		for(i = 0; i < head; i++ )
			if(snake[i] == 1) {
				snake_coord[i].x = snake_coord[i+1].x;
				snake_coord[i].y = snake_coord[i+1].y;
			}
		snake_coord[head].x -= 1; 
	}
	//top
	else if(direction == 2){
		for(i = 0; i < head; i++ )
			if(snake[i] == 1) {
				snake_coord[i].x = snake_coord[i+1].x;
				snake_coord[i].y = snake_coord[i+1].y;
			}
		snake_coord[head].y -= 1; 
	}
	//right
	else if(direction == 3){
		for(i = 0; i < head; i++ )
			if(snake[i] == 1) {
				snake_coord[i].x = snake_coord[i+1].x;
				snake_coord[i].y = snake_coord[i+1].y;
			}
		snake_coord[head].x += 1; 
	}
	//bottom
	else if(direction == 4){
		for(i = 0; i < head; i++ )
			if(snake[i] == 1) {
				snake_coord[i].x = snake_coord[i+1].x;
				snake_coord[i].y = snake_coord[i+1].y;
			}
		snake_coord[head].y = 1; 
	}
	int result = check_colision();
	if (result == 2)
		game_over();
}

void drawObstacles() {
	GFX_draw_line(10, 10, 100, 10, ST7735_BLACK);
	GFX_draw_line(60, 10, 60, 60, ST7735_BLACK);
	GFX_draw_line(110, 20, 110, 110, ST7735_BLACK);
}

void gameStart(){
	GFX_draw_line(10, 10, 30, 10, ST7735_BLACK);
	GFX_draw_line(10, 10, 10, 30, ST7735_BLACK);
	GFX_draw_line(10, 30, 30, 30, ST7735_BLACK);
	GFX_draw_line(30, 30, 30, 50, ST7735_BLACK);
	GFX_draw_line(30, 50, 10, 50, ST7735_BLACK);
}

int main(void){
    // init the lcd display
    LCD_init();
	LCD_fill_screen(ST7735_BLUE);
	initGame();
	draw_game();
	
	
	//GFX_draw_line(100, 100, 120, 120, 64);
	//pull up resistence
	DDRA &= ~((1 << PA7));
	DDRA &= ~((1 << PA6));
	DDRA &= ~((1 << PA5));
	DDRA &= ~((1 << PA4));
    PORTA |= (1 << PA7);
	PORTA |= (1 << PA6);
	PORTA |= (1 << PA5);
	PORTA |= (1 << PA4);

    while(1){
		//right - cant switch to right when you go left
		if((PINA & (1 << PA7)) == 0)
        {
            if(direction != 1)
				direction = 3;
        }
		
		if((PINA & (1 << PA6)) == 0)
        {
            if(direction != 2)
				direction = 4;
        }
		
		if((PINA & (1 << PA5)) == 0)
        {
            if(direction != 3)
				direction = 1;
        }
		
		if((PINA & (1 << PA4)) == 0)
        {
            if(direction != 4)
				direction = 2;
        }
		move_snake();
		LCD_fill_screen(ST7735_BLUE);
		draw_game();
		_delay_ms(300);
    }

    return 0;
}
