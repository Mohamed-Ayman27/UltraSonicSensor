#include <avr/io.h>
#include <util/delay.h> /* To use delay function */
#include "lcd.h"
#include "ultrasonic.h"

/* function main begins program execution */
int main(void)
{
	/********** Initialization Code **********/

	uint16 Distance; /* Variable to store the distance to be displayed on the screen */

	SREG = ( 1 << 7 ); /* Enable global interrupts to generate one using ICU */

	LCD_init(); /* Initialize LCD to represent the distance */

	LCD_displayString("Distance =    cm"); /* Display the standard message for representing distance value */


	while(1)
	{
		/* 1. Initialize the sensor inside the the loop to actively get it's reading when changed
		 * 2. Call Ultrasonic readDistance function to measure the distance
		 * 3. Store the value sent from the function in a variable to be represented
		 */

		Ultrasonic_init();   /* Initialize Ultrasonic to get the distance */

		Distance = Ultrasonic_readDistance();

		LCD_moveCursor(0, 11); /* Move the cursor to the suitable place for representing the distance */


		if( Distance >= 100)
		{
			LCD_intgerToString(Distance);
		}
		else
		{

			LCD_intgerToString(Distance);

			LCD_displayCharacter(' ');
		}
	}
}
