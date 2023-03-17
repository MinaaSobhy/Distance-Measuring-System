/*
 * HC-SR04_sensor.c
 *
 *  Created on: OCT 13, 2022
 *      Author: Mina Sobhy
 *      description: Source file for the ultrasonic sensor driver
 *
 */


#include "HC-SR04_sensor.h"
#include "icu.h"
#include "util/delay.h"
#include "gpio.h"




/*global variables definitions to store the time */
uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Ultrasonic_init(void)
{
	/* Create configuration structure for ICU driver */
	Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

	/* Set the Call back function pointer in the ICU driver */
	Icu_setCallBack(Ultrasonic_edgeProcessing);

	/* Initialize the ICU driver */
	Icu_init(&Icu_Config);

	/* Setup the direction for the trigger pin as output pin
	 * through the GPIO driver
	 */
	GPIO_setupPinDirection(TRIGGER_PORT, TRIGGER_PIN, PIN_OUTPUT);
}

void Ultrasonic_Trigger(void)
{
	/* generating an output signal for 10 us */
	GPIO_writePin(TRIGGER_PORT, TRIGGER_PIN, LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(TRIGGER_PORT, TRIGGER_PIN, LOGIC_LOW);
}


uint16 Ultrasonic_readDistance(void)
{
	/* Send the trigger pulse */
	Ultrasonic_Trigger();

	/*return the distance in cm */
	return ((uint16)(float32)((g_timeHigh)/57.47));
}

void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}

	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();

		/* Clear the timer counter register to start measurements again */
		g_edgeCount =0;
		Icu_clearTimerValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
}
