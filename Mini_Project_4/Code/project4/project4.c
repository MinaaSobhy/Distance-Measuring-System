/*
 * project4.c
 *
 *  Created on: OCT 13, 2022
 *      Author: Mina Sobhy
 *      description: Source file for a system that receives a distance and diplay it on LCD
 *
 */

#include "std_types.h"
#include "gpio.h"
#include "lcd.h"
#include "HC-SR04_sensor.h"
#include "avr/io.h"



int main (void)
{
	/* declaring a variable to store the current distance */
	uint16 distance;

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* initializing ultrasonic sensor and lcd */
	Ultrasonic_init();
	LCD_init();

	/* displaying the initial titles*/
	LCD_displayString("Distance=    cm");

	for(;;)
	{
		/* get the distance from the ultrasonic sensor*/
		distance = Ultrasonic_readDistance();

		/*printing the distance on the lcd */
		LCD_moveCursor(0,10);
		if (distance >= 100)
		{
			LCD_intgerToString(distance);
		}
		else
		{
			LCD_intgerToString(distance);
			LCD_displayString(" ");
		}
	}

}
