/*
 * tablero.h
 *
 * Created: 12/11/2014 5:51:38 PM
 *  Author: cnye
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <asf.h>
#include "light_ws2812.h"

	//Functions
// Return	Name					Params
uint16_t	XY						( uint8_t x, uint8_t y);
void		setColor				(uint8_t col, uint8_t row, uint32_t color);
void		checkAllSquares			(void);
void		makeSetupPattern		(void);
void		setAllColorsFromGrid	(void);
void goodBreak();

struct square_t {
	bool occupied;
	bool changed;
	bool placement_option;
	uint8_t die;
};