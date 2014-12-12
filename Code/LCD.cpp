#include <Arduino.h>
#include "ST7036.h"
#include "LCD_C0220BiZ.h"
#include <Wire.h>

#define CHAR_WIDTH  5;


//LCD_C0220BIZ lcd = LCD_C0220BIZ ( );
ST7036 lcd = ST7036 ( 2, 20, 0x78 );


void setup ()
{

   lcd.init ();
   lcd.setContrast(10);

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
