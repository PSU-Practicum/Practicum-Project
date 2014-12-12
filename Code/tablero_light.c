/*
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "light_ws2812.h"

#include <stdint.h>
#define NUM_LEDS 50
#define ROWS 3
#define DATA_PIN 3
#define ROLL_PIN 4

struct cRGB led[NUM_LEDS];

//States
enum State{
	INITIAL_PLACEMENT,
	ROLL_DICE,
	FIRST_PIECE_B4_ROLL,
	FIRST_PIECE_AFTER_ROLL,
	FIRST_PIECE_PICKED,
	FIRST_PIECE_PLACED,
	SECOND_PIECE_PICKED,
	MISPLACED
};


// Colors
enum Color{
	OPTION = 5,
	INVALID = 4,
	OCCUPIED = 5,
	NONE = 5
};

// Constants
const int N_ROWS = 7;                       // Number of Rows
const int N_COLS = 7;                       // Number of Columns
const int LCD_NUM_CHARS = 25;

const int setupPattern[N_ROWS][N_COLS] = {
	{0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 1},
	{0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0},
	{1, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 0, 0, 0, 0}
};

// Vars
uint8_t current_row = 0;                    // Current row index
uint8_t current_row_values = 0;             // Switch values at the current row
uint8_t dice[2] = {0,0};                    // Two die
State   prev_state = INITIAL_PLACEMENT,     // Game States
curr_state = INITIAL_PLACEMENT,
next_state = INITIAL_PLACEMENT;
char    displayString[LCD_NUM_CHARS + 1] = "";                  // String to display to the LCD screen

// The "grid" represents information about each square on the grid
struct square_t {
	uint8_t color;
	bool old_occupied;
	bool new_occupied;
	bool placement_option;
	bool valid;
} grid[N_ROWS][N_COLS];

// Misc.
uint8_t i = 0;      // Loop iterator


// onRowChange() should be called every time the row change timer expires. All the processing is done *before* the row changes,
// allowing time for the signals to propagate before processing them.
void onRowChange(){
	//TODO: get current row values
	// current_row_values = ??;
	
	// Loop through each column
	for( i = 0 ; i < N_COLS ; i++ ){
		// Assign old and new occupancy values
		grid[current_row][i].old_occupied = grid[current_row][i].new_occupied;
		grid[current_row][i].new_occupied = (current_row_values & 1);
		// Shift the row values to read the next column on the next iteration
		current_row_values >> 1;
		
		// TODO: Check if a piece was picked or placed, update according to algorithm
		if(grid[current_row][i].old_occupied != grid[current_row][i].new_occupied){
			// True if piece was placed
			if(grid[current_row][i].new_occupied){
				//
			}
			else // True if piece was picked
			{
				//
			}
		}
		
		// Update lighting values
		if(grid[current_row][i].placement_option)
		grid[current_row][i].color = OPTION;
		else if(grid[current_row][i].new_occupied){
			if (grid[current_row][i].valid)
			grid[current_row][i].color = OCCUPIED;
			else
			grid[current_row][i].color = INVALID;
		}
		else
		grid[current_row][i].color = NONE;
	}
	
	// TODO:
	updateLEDs();
	
	//TODO: change the row in GPIO
	current_row = (current_row + 1) % N_ROWS;
}

// function to roll dice
void rollDice(uint8_t dice[2])
{
	dice[0] = random(1,6);
	delay(2);
	dice[1] = random(1,6);
}

//TODO:
updateDisplay(displayString);

void main(){	
	// initialize the pushbutton pin as an input
	
	// initialize the pins for reed switch matrix input and output
	
	// set all LEDs to off when power on
	for (int i = 0; i < NUM_LEDS; i++){
		leds[i] = CRGB::Black;
	}
	
	
	
	
	//TODO: initialize GPIOS
	//TODO: initialize TWI
	//TODO: initialize addressable LEDs
	//TODO: setup repeating interrupt timer
	//TODO: enable interrupts
	//TODO: start timer
}



uint16_t XY( uint8_t x, uint8_t y)
{
	uint16_t i;
	
	if( y & 0x01) {
		// Odd rows run backwards
		uint8_t reverseX = (N_COLS - 1) - x;
		i = (y * N_COLS) + reverseX;
		} else {
		// Even rows run forwards
		i = (y * N_COLS) + x;
	}
	
	return i;
}


void checkAllSquares(){
	for (int i = 0; i < ROWS; i++){
		pinMode(i + 8, OUTPUT);
		digitalWrite(i + 8, HIGH);
		delay(2);
		for (int j = 0; j < ROWS; j++){
			if (digitalRead(11 + j))
			leds[XY(i,j)] = CRGB::Red;
			else
			leds[XY(i,j)] = CRGB::Black;
		}
		digitalWrite(i + 8, LOW);
		delay(2);
		pinMode(i + 8, INPUT);
		delay(2);
	}
	
	
	
	
	void gamePlay(){
		switch (curr_state)
		{
			case INITIAL_PLACEMENT:
			int num_correct = 0;
			int num_incorrect = 0;
			for (i = 0; i < N_ROWS; i++){
				for (j = 0; j < N_COLS; j++){
					if (setupPattern[i][j]){
						if (grid[i][j].new_occupied){
							grid[i][j].color = OCCUPIED;
							num_correct++;
						}
						else {
							grid[i][j].color = OPTION;
						}
					}
					else {
						if (grid[i][j].new_occupied){
							grid[i][j].color = INVALID;
							num_incorrect++;
						}
						else {
							grid[i][j].color = NONE;
						}
					}
				}
			}
			if (num_correct == 7 && num_incorrect == 0) next_state == ROLL_DICE;
			break;
			case ROLL_DICE:
			// TODO:
			
			
			
		}
		
	}
	
	
	
}
*/