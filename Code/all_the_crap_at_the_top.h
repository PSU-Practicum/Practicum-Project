/*
 * all_the_crap_at_the_top.h
 *
 * Created: 12/11/2014 8:32:32 PM
 *  Author: cnye
 */ 

#include "tablero.h"

const int outPin[7] = { IOPORT_CREATE_PIN(PORTB, 1),
	IOPORT_CREATE_PIN(PORTB, 0),
	IOPORT_CREATE_PIN(PORTD, 7),
	IOPORT_CREATE_PIN(PORTC, 0),
	IOPORT_CREATE_PIN(PORTC, 1),
	IOPORT_CREATE_PIN(PORTC, 2),
	IOPORT_CREATE_PIN(PORTC, 3)};

const int  inPin[7] = { IOPORT_CREATE_PIN(PORTD, 3),
	IOPORT_CREATE_PIN(PORTD, 2),
	IOPORT_CREATE_PIN(PORTD, 4),
	IOPORT_CREATE_PIN(PORTD, 1),
	IOPORT_CREATE_PIN(PORTD, 0),
	IOPORT_CREATE_PIN(PORTD, 5),
	IOPORT_CREATE_PIN(PORTD, 6)};

#include <stdint.h>
#define NUM_LEDS 49
#define N_ROWS 7
#define N_COLS 7
#define NUM_WIN_FLASHES 5
#define WIN_FLASH_DELAY_MS 250
#define PLAYER_CHANGE_FLASH_DELAY_MS 75

#define MAX_DICE 6

// Colors
#define COLOR_SETUP		0x808000	// yellow
#define COLOR_OCCUPIED  0x00FFFF	// teal
#define COLOR_INVALID   0xa00000	// red
#define COLOR_READY     0x800080	// purple?
#define COLOR_BLANK		0

#define x 0
#define y 1

#define ROLL_PIN IOPORT_CREATE_PIN(PORTB, 7)

//States
typedef enum{
	INITIAL_PLACEMENT,
	ROLL_DICE,
	FIRST_MOVE_BEFORE_PICK,
	FIRST_MOVE_AFTER_PICK,
	SECOND_MOVE_BEFORE_PICK,
	SECOND_MOVE_AFTER_PICK,
	INVALID_PICKED_A_PIECE_BEFORE_ROLL,
	INVALID_PLACEMENT_IN_FIRST_MOVE,
	INVALID_PICKED_SECOND_PIECE_IN_FIRST_MOVE,
	INVALID_PLACEMENT_IN_SECOND_MOVE,
	INVALID_PICKED_SECOND_PIECE_IN_SECOND_MOVE,
	INVALID_PICKED_WRONG_PIECE_IN_SECOND_MOVE,
} State;

typedef enum{
	COLUMN,
	ROW,
	DIAGONAL,
} LineType;

struct cRGB led[49], temp_led[49];
bool square_has_piece, change, direction;
int i, j, k;

uint8_t die_of_first_move, axis_of_first_movement;
uint8_t loc[2], orig_loc[2], err_loc[2], first_loc[2]; // Location trackers

uint8_t dice[2] = {0,0};                    // Two dice
State	prev_state = INITIAL_PLACEMENT,     // Game States
		curr_state = INITIAL_PLACEMENT;
//char    displayString[LCD_NUM_CHARS + 1] = "";

const int setupPattern[N_ROWS][N_COLS] = {
	{0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 1},
	{0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0},
	{1, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 0, 0, 0, 0}
};

const int D_setupPattern[N_ROWS][N_COLS] = {
	{1, 0, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 1, 0},
	{0, 0, 0, 0, 0, 0, 1}
};

// The "grid" represents information about each square on the grid
struct square_t grid[N_ROWS][N_COLS];



uint16_t XY( uint8_t col, uint8_t row)
{
	uint16_t i;
	
	if( row & 0x01) {
		// Odd rows run backwards
		uint8_t reverseX = (N_COLS - 1) - col;
		i = (row * N_COLS) + reverseX;
		} else {
		// Even rows run forwards
		i = (row * N_COLS) + col;
	}
	return i;
}

void setColor(uint8_t col, uint8_t row, uint32_t color){
	led[XY(col,row)].g = (color & 0x00ff0000) >> 16;
	led[XY(col,row)].r = (color & 0x0000ff00) >> 8;
	led[XY(col,row)].b = (color & 0x000000ff);
}

void setTempColor(uint8_t col, uint8_t row, uint32_t color){
	temp_led[XY(col,row)].g = (color & 0x00ff0000) >> 16;
	temp_led[XY(col,row)].r = (color & 0x0000ff00) >> 8;
	temp_led[XY(col,row)].b = (color & 0x000000ff);
}

void checkAllSquares(void){
	change = false;
	for (i = 0; i < N_COLS; i++){
		ioport_set_pin_dir(outPin[i], IOPORT_DIR_OUTPUT);
		ioport_set_pin_high(outPin[i]);
		_delay_ms(2);
		for (j = 0; j < N_ROWS; j++){
			square_has_piece = (ioport_get_pin_level(inPin[j]));
			if (grid[i][j].occupied != square_has_piece){
				grid[i][j].changed = true;
				change = true;
				loc[x] = i;
				loc[y] = j;
			}
			else grid[i][j].changed = false;
			grid[i][j].occupied = square_has_piece;
		}
		ioport_set_pin_low(outPin[i]);
		_delay_ms(2);
		ioport_set_pin_dir(outPin[i], IOPORT_DIR_INPUT);
		_delay_ms(2);
	}
}

void winAnimation(){
	for(k = 0; k < NUM_WIN_FLASHES*2; k++){
		for(i=0;i<N_COLS;i++){for(j=0;j<N_ROWS;j++){
			if (grid[i][j].occupied)
				setTempColor(i,j, COLOR_OCCUPIED*(k&1));
			else
				setTempColor(i,j, COLOR_BLANK);
		}}
		_delay_ms(WIN_FLASH_DELAY_MS);
		ws2812_setleds(temp_led,NUM_LEDS);
	}
}

void switchPlayerAnimation(){
	for(i=0;i<N_COLS;i++){
		for(j=0;j<N_ROWS;j++){
			setTempColor(direction? i : ( (N_COLS-1) - i ),j, COLOR_INVALID);
		}
		ws2812_setleds(temp_led,NUM_LEDS);
		_delay_ms(PLAYER_CHANGE_FLASH_DELAY_MS);
		for(j=0;j<N_ROWS;j++){
			setTempColor(direction? i : ( (N_COLS-1) - i ),j, COLOR_BLANK);
		}
	}
	direction = !direction;
}

bool isAligned(){
	LineType lineType = COLUMN;
	int num_hits = 0, col = 0, row = 0;
	for(i=0;i<N_COLS;i++){
		for(j=0;j<N_ROWS;j++){
			if (grid[i][j].occupied)
			{
				num_hits++;
				switch (num_hits)
				{
					case 1:
						col = i;
						row = j;
						break;
					case 2:
						if (i == col){
							lineType = COLUMN;
						}
						else if (j == row)
						{
							lineType = ROW;
						} 
						else if (abs(col - i) == abs(row - j))
						{
							lineType = DIAGONAL;
						}
						else
							return false;
						break;
					default:
						switch (lineType){
							case ROW:
								if(!(j == row)) return false;
							break;
							case COLUMN:
								if(!(i == col)) return false;
							break;
							case DIAGONAL:
								if (!(abs(col - i) == abs(row - j)))
									return false;
							break;		
						}
				}
				if(num_hits == 7) return true;
			}
		}
	}
	return false;
}

void uglyBreak(){
	while(1){
		switchPlayerAnimation();
		direction = !direction;
	}
}

void reset(){
	makeSetupPattern();
	curr_state = INITIAL_PLACEMENT;
}

void goodBreak(){
while (1)
{
	setColor(0,0, 0x0000ff);
	ws2812_setleds(led,NUM_LEDS);
	_delay_ms(300);
	setColor(0,0, 0x00ff00);
	ws2812_setleds(led,NUM_LEDS);
	_delay_ms(300);
}
}
void badBreak(){
while (1)
{
	setColor(0,0, 0x0000ff);
	ws2812_setleds(led,NUM_LEDS);
	_delay_ms(300);
	setColor(0,0, 0xff0000);
	ws2812_setleds(led,NUM_LEDS);
	_delay_ms(300);
}
}