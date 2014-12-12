/*
 * Misc_code_resources.c
 *
 * Created: 12/11/2014 8:31:18 PM
 *  Author: cnye





#include <LCD>
#include "LCD_C0220BiZ.h"
#include <Wire.h>
#define CHAR_WIDTH  5;
ST7036 lcd = ST7036 ( 2, 20, 0x78 );



	lcd.init ();
	lcd.setContrast(10);
	lcd.clear ();
	lcd.setCursor ( 0, 0  );
	lcd.print ("Dice Roll 1 = ");
	lcd.print (i);
	lcd.setCursor (1,0);
	lcd.print ("Dice Roll 2 = ");
	lcd.print (i + 1);
	delay (500);





//#define  IOPORT_CREATE_PIN(PORTB, 2)
//#define  IOPORT_CREATE_PIN(PORTB, 3)
//#define  IOPORT_CREATE_PIN(PORTB, 4)
//#define  IOPORT_CREATE_PIN(PORTB, 5)

//#define  IOPORT_CREATE_PIN(PORTC, 4)
//#define  IOPORT_CREATE_PIN(PORTC, 5)




		
		for(i = 0; i < N_ROWS; i++){
			for(j = 0; j < N_ROWS; j++){
				if(grid[i][j].occupied){
					setColor(i,j, COLOR_OCCUPIED);
				}
				else{
	//				setColor(i,j, 0);
				}
			}
		}




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



const int LCD_NUM_CHARS = 25;

uint8_t dice[2] = {0,0};                    // Two die
State   prev_state = INITIAL_PLACEMENT,     // Game States
curr_state = INITIAL_PLACEMENT,
next_state = INITIAL_PLACEMENT;
char    displayString[LCD_NUM_CHARS + 1] = "";                  // String to display to the LCD screen





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
			
			
			
			
			// function to roll dice
			void rollDice(uint8_t dice[2])
			{
				dice[0] = random(1,6);
				delay(2);
				dice[1] = random(1,6);
			}
			
			
			
			*/