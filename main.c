#include "gfx.h"

#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>


//draw START 
int snake[80];

//0-normal; 1-grow twice, 2-shrink, 3-swap commands, 4-reverse move
int food_type;
int score;
int swap;//swap controlls
int delay;

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

void draw_rip(int color) {

int color2;
	if(color == 1)
		color2 = ST7735_RED;
	if(color == 2)
		color2 = ST7735_YELLOW;
	if(color == 1)
		color2 = ST7735_BLUE;
//R
	GFX_draw_line(10, 10, 40, 10, color2);
	GFX_draw_line(40, 10, 40, 40, color2);
	GFX_draw_line(10, 10, 10, 40, color2);
	GFX_draw_line(10, 40, 40, 40, color2);
	GFX_draw_line(10, 40, 40, 70, color2);
	GFX_draw_line(10, 40, 10, 70, color2);
//I
	GFX_draw_line(50, 10, 50, 70, color2);
//P
	GFX_draw_line(60, 10, 90, 10, color2);
	GFX_draw_line(90, 10, 90, 40, color2);
	GFX_draw_line(90, 40, 60, 40, color2);
	GFX_draw_line(60, 40, 60, 10, color2);
	GFX_draw_line(60, 40, 60, 70, color2);

}

void initGame2(){

	delay = 1000;
	food_type = 0;
	swap = 0;
	score = 0;
	//init obastacles
	obstacle[0].x1 = 0;
	obstacle[0].y1 = 64;
	obstacle[0].x2 = 127;
	obstacle[0].y2 = 64;

	obstacle[1].x1 = 64;
	obstacle[1].y1 = 0;
	obstacle[1].x2 = 64;
	obstacle[1].y2 = 127;

	obstacle[2].x1 = 0;
	obstacle[2].y1 = 0;
	obstacle[2].x2 = 1;
	obstacle[2].y2 = 1;
	
	int i;
	//initial size is 20
	for(i = tail; i < head; i++ )
		snake[i] = 1;
	//max size is 80
	for(i = 0; i < tail; i++ )
		snake[i] = 0;
	
	direction = 0;
	//start position
	for(i = tail; i < 80; i++ ) {
		snake_coord[i].x = i+ 20;
		snake_coord[i].y = 100;
	}
	
	food.x = rand() % 128;
	food.y = rand() % 128;

	//food on the same line as snake
	//while(food.y == 30)
		//food.y = rand() % 128;
}
void initGame(){


	delay = 2000;
	food_type = 0;
	swap = 0;
	score = 0;
	//init obastacles
	obstacle[0].x1 = 10;
	obstacle[0].y1 = 10;
	obstacle[0].x2 = 100;
	obstacle[0].y2 = 10;

	obstacle[1].x1 = 70;
	obstacle[1].y1 = 30;
	obstacle[1].x2 = 70;
	obstacle[1].y2 = 70;

	obstacle[2].x1 = 110;
	obstacle[2].y1 = 20;
	obstacle[2].x2 = 110;
	obstacle[2].y2 = 110;
	
	int i;
	//initial size is 20
	for(i = tail; i < head; i++ )
		snake[i] = 1;
	//max size is 80
	for(i = 0; i < tail; i++ )
		snake[i] = 0;
	
	direction = 0;
	//start position
	for(i = 0; i < 80; i++ ) {
		snake_coord[i].x = i;
		snake_coord[i].y = 100;
	}
	
	food.x = rand() % 128;
	food.y = rand() % 128;

	//food on the same line as snake
	while(food.y == 30)
		food.y = rand() % 128;
}
void my_LCD_fill_screen(uint16_t color)
{
	uint8_t x, y;
	int i;
	LCD_set_range(0, 0, 127, 127);

	for (y = 128; y > 0; y--)
		for (x = 128; x > 0; x--)
			for (i = 0;i < num_obs; i++) 				
				if(!(x >= obstacle[i].x1 && x <= obstacle[i].x2))
					if(!(y >= obstacle[i].y1 && y <= obstacle[i].y2))
						LCD_set_pixel(color);
}

void draw_game() {
	int i;
	//draw snake

	for(i = tail; i <= head; i++ )
		if(snake[i] == 1)
			LCD_draw_pixel(snake_coord[i].x, snake_coord[i].y, ST7735_BLACK);
		else // clean snake tail
			LCD_draw_pixel(snake_coord[i].x, snake_coord[i].y, ST7735_BLUE);
		
	LCD_draw_pixel(snake_coord[tail].x, snake_coord[tail].y, ST7735_BLUE);
	//draw food
	if (food_type == 1){
		LCD_draw_pixel(food.x, food.y, ST7735_RED);
		LCD_draw_pixel(food.x+1, food.y+1, ST7735_RED);
		LCD_draw_pixel(food.x, food.y+1, ST7735_RED);
		LCD_draw_pixel(food.x+1, food.y, ST7735_RED);
	}
	else {
		LCD_draw_pixel(food.x, food.y, ST7735_MAGENTA);
		LCD_draw_pixel(food.x+1, food.y+1, ST7735_MAGENTA);
		LCD_draw_pixel(food.x, food.y+1, ST7735_MAGENTA);
		LCD_draw_pixel(food.x+1, food.y, ST7735_MAGENTA);
	}
	//draw obstacles
	for (i = 0;i < num_obs; i++) {
		GFX_draw_line(obstacle[i].x1, obstacle[i].y1, obstacle[i].x2, obstacle[i].y2, ST7735_BLACK);
	}
}
//RIP
void game_over(){
	LCD_fill_screen(ST7735_BLACK);
	int i = 1;
	while(i){
		draw_rip(i);
		_delay_ms(300);
		i++;
		if(i > 3)
			i = 1;
	}
};


void check_colision() {
	int colision_type = 0;
	int i;

	//check with food
	if(snake_coord[head].x >= food.x && snake_coord[head].x <= food.x+3
							&& snake_coord[head].y >= food.y && snake_coord[head].y <= food.y+3){
		LCD_fill_screen(ST7735_BLUE);
		colision_type = food_type;
		score++;
		if (food_type == 0){
			tail = tail - 1;
			snake[tail] = 1;
			/*
						tail = tail - 10;
			snake[tail+9] = 1;
			snake[tail+8] = 1;
			snake[tail+7] = 1;
			snake[tail+6] = 1;
			snake[tail+5] = 1;
			snake[tail+4] = 1;
			snake[tail+3] = 1;
			snake[tail+2] = 1;
			snake[tail+1] = 1;

			*/
		}
		else if (food_type == 1){
			tail = tail - 2;
			snake[tail] = 1;
			snake[tail + 1] = 1;
		}
		else if (food_type == 2){
			tail = tail + 2;
			snake[tail - 1] = 0;
			snake[tail - 2] = 0;
		}
		else if (food_type == 3) {
			swap = swap + 1;
			swap = swap % 3;
		}
		
		else if (food_type == 4) {
		// swap snake movement direction
			coord aux;
			for (i = tail; i < (head + tail)/2; i++){
				aux.x = snake_coord[i].x;
				aux.y = snake_coord[i].y;
				snake_coord[i].x = snake_coord[head - i + tail].x;
				snake_coord[i].y = snake_coord[head - i + tail].y;
				snake_coord[head - i + tail].x = aux.x;
				snake_coord[head - i + tail].y = aux.y;
			}
			for (i = 0; i < tail; i++) {
				snake_coord[i].x = snake_coord[tail].x;
				snake_coord[i].y = snake_coord[tail].y - i - 1;
			}
			if (direction == 1)
				direction = 3;
			else if (direction == 3)
				direction = 1;
			else if (direction == 2)
				direction = 4;
			else if (direction == 4)
				direction = 2;
		} 
		else if (food_type == 5) {
			delay += 300;
		}
		else if (food_type == 6) {
			delay -= 300;
		}
		food.x = rand() % 128;
		food.y = rand() % 128;
		//power up type
		food_type = rand() % 7;
		//check if fruit placement is ok
		for(i = 0; i < head; i++ )
			if(snake[i] == 1) {
				if(snake_coord[head].x == food.x && snake_coord[head].y == food.y){
					food.x = rand() % 128;
					food.y = rand() % 128;
					i = 0;
				}
			}
		
		for (i = 0;i < num_obs; i++)	
			if(food.x >= obstacle[i].x1 && food.x <= obstacle[i].x2)
				if(food.y >= obstacle[i].y1 &&  food.y <= obstacle[i].y2){
					food.x = rand() % 128;
					food.y = rand() % 128;
					i = 0;
				}
	}
	//check collision with walls
	for (i = 0;i < num_obs; i++)	
		if(snake_coord[head].x >= obstacle[i].x1 && snake_coord[head].x <= obstacle[i].x2)
			if(snake_coord[head].y >= obstacle[i].y1 && snake_coord[head].y <= obstacle[i].y2)
				game_over();

	//check collision with tail
	for(i = 0; i < head; i++ )
		if(snake[i] == 1) {
			if(snake_coord[head].x == snake_coord[i].x && snake_coord[head].y == snake_coord[i].y)
				game_over();
	}
}


void move_snake(){
//left
	int i  = 0;
	if(direction == 1){
		for(i = 0; i < head; i++ ) {
			snake_coord[i].x = snake_coord[i+1].x;
			snake_coord[i].y = snake_coord[i+1].y;
		}
		snake_coord[head].x -= 1; 
	}
	//top
	else if(direction == 2){
		for(i = 0; i < head; i++ ) {
			snake_coord[i].x = snake_coord[i+1].x;
			snake_coord[i].y = snake_coord[i+1].y;
		}
		snake_coord[head].y -= 1; 
	}
	//right
	else if(direction == 3){
		for(i = 0; i < head; i++ ) {
			snake_coord[i].x = snake_coord[i+1].x;
			snake_coord[i].y = snake_coord[i+1].y;
		}
		snake_coord[head].x += 1; 
	}
	//bottom
	else if(direction == 4){
		for(i = 0; i < head; i++ ) {
				snake_coord[i].x = snake_coord[i+1].x;
				snake_coord[i].y = snake_coord[i+1].y;
			}
		snake_coord[head].y += 1; 
	}
	
	if(snake_coord[i].x >= 128)
		snake_coord[i].x = snake_coord[i].x % 128;
	if(snake_coord[i].y >= 128)
		snake_coord[i].y = snake_coord[i].y % 128;
	if(snake_coord[i].x < 0)
		snake_coord[i].x = snake_coord[i].x + 128;
	if(snake_coord[i].y < 0)
		snake_coord[i].y = snake_coord[i].y + 128;
	check_colision();
	
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
	int game = 1;
	LCD_init();
	LCD_fill_screen(ST7735_BLUE);
	initGame();
	draw_game();
	
	
	//GFX_draw_line(100, 100, 120, 120, 64);
	//pull up resistence
	DDRD &= ~((1 << PD0));
	DDRD &= ~((1 << PD1));
	DDRD &= ~((1 << PD4));
	DDRD &= ~((1 << PD5));

    PORTD |= (1 << PD0);
	PORTD |= (1 << PD1);
	PORTD |= (1 << PD4);
	PORTD |= (1 << PD5);

    while(1){
		// normal commands
		if (swap == 0) {
			//right - cant switch to right when you go left 
			if((PIND & (1 << PD5)) == 0) {
				if(direction != 1)
					direction = 3;
			}
			else if((PIND & (1 << PD0)) == 0) {
				if(direction != 2)
					direction = 4;
			}
			//cant move to left if the snake is oriented to right at begin
			else if(direction && (PIND & (1 << PD1)) == 0) {
				if(direction != 3)
					direction = 1;
			}
			else if((PIND & (1 << PD4)) == 0) {
				if(direction != 4)
					direction = 2;
			}
		}
		else if (swap == 1) {
			//swap top with bot and left with right 
			if((PIND & (1 << PD5)) == 0) {
				if(direction != 1)
					direction = 3;
			}
			else if((PIND & (1 << PD1)) == 0) {
				if(direction != 2)
					direction = 4;
			}
			else if((PIND & (1 << PD0)) == 0) {
				if(direction != 3)
					direction = 1;
			}
			else if((PIND & (1 << PD4)) == 0) {
				if(direction != 4)
					direction = 2;
			}
		}
		else if (swap == 2) {
			//circular swap 
			if((PIND & (1 << PD1)) == 0) {
				if(direction != 1)
					direction = 3;
			}
			else if((PIND & (1 << PD0)) == 0) {
				if(direction != 2)
					direction = 4;
			}
			else if((PIND & (1 << PD4)) == 0) {
				if(direction != 3)
					direction = 1;
			}
			else if((PIND & (1 << PD5)) == 0) {
				if(direction != 4)
					direction = 2;
			}
		}
		if (game == 1 && score == 2) {
			LCD_fill_screen(ST7735_BLUE);
			//_delay_ms(1000);
			game = 2;
			initGame2();
		}
		if (game == 2 && score == 20) {
			while (1)
				LCD_fill_screen(ST7735_BLUE);

		}
		//my_LCD_fill_screen(ST7735_BLUE);
		_delay_ms(delay);
		
		move_snake();
		draw_game();
    }

    return 0;
}