/*
 * CFile2.c
 *
 * Created: 12/11/2014 9:34:08 PM
 *  Author: cnye

#include <Arduino.h>



//LCD_C0220BIZ lcd = LCD_C0220BIZ ( );


void setup ()
{


}

int i;

void loop ()
{
	for(i = 0; i < 7; ++i)
	{
		lcd.clear ();
		lcd.setCursor ( 0, 0  );
		lcd.print ("Dice Roll 1 = ");
		lcd.print (i);
		lcd.setCursor (1,0);
		lcd.print ("Dice Roll 2 = ");
		lcd.print (i + 1);
		delay (500);
	}


	
}
*/