 /******************************************************************************
 *
 * Module: ULTRASONIC
 *
 * File Name: ultrasonic.c
 *
 * Description: Source file for the ULTRASONIC driver
 *
 * Author: Mohamed Ayman
 *
 *******************************************************************************/
#include <util/delay.h>
#include "ultrasonic.h"
#include "gpio.h"
#include "icu.h"

/* Get edge count*/
uint8 g_edgeCount = 0;

/* Store the High time value */
uint16 g_timeHigh = 0;

/* Store the Period time value */
uint16 g_timePeriod = 0;

/* Store the Period time value + High time value */
uint16 g_timePeriodPlusHigh = 0;

/*Global variable to store the distance read by the sensor*/
uint16 g_distance = 0;

Icu_ConfigType Config = {F_CPU_8, RISING};

void Ultrasonic_init(void)
{
	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT); //Trigger Pin
	GPIO_setupPinDirection(PORTD_ID , PIN6_ID , PIN_INPUT);

	Icu_setCallBack(Ultrasonic_edgeProcessing);

	Icu_init(&Config);

}


void Ultrasonic_Trigger(void)
{
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);

	_delay_us(10);

	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);
}



void Ultrasonic_edgeProcessing()
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
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
	else if(g_edgeCount == 3)
	{
		/* Store the Period time value */
		g_timePeriod = Icu_getInputCaptureValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 4)
	{
		/* Store the Period time value + High time value */
		g_timePeriodPlusHigh = Icu_getInputCaptureValue();
		/* Clear the timer counter register to start measurements again */
		Icu_clearTimerValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
}


uint16 Ultrasonic_readDistance(void)
{
	Ultrasonic_Trigger();
	if(g_edgeCount == 4)
	{
		Icu_DeInit(); /* Disable ICU Driver */
		g_edgeCount = 0;

		g_distance = ( g_timePeriodPlusHigh - g_timePeriod ) / 58;  /* (High_Time) / 58 */
	}

	return g_distance;
}
