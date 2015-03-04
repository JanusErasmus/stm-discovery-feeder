#include <stm32f10x.h>
#include <misc.h>

#include "terminal.h"
#include "led.h"
#include "temp.h"
#include "rtc.h"

void delay();

typedef int size_t;
void* memcpy( void * destination, const void * source, size_t num );

void minuteAction(void)
{
	t_print("5 Minutes has passed\n");
	rtc_displayTime();
}

void hourAction(void)
{
	t_print("==== HOURLY ===\n");
	t_print("An HOUR has passed\n");
	rtc_displayTime();
}

/// Main function.  Called by the startup code.
int main(void)
{
	//when interrupts are going to be used
	NVIC_Configuration();

	initTerminal();
	initLED();
	initTemp();
	initRTC();

	t_print("Fish Feeder - Build: ");
	t_print(__DATE__);
	t_print(" ");
	t_print(__TIME__);
	t_print("\n");


	rtc_setMinuteAlarm(5, minuteAction);
	rtc_setHourAlarm(1, hourAction);

	while(1)
	{
		delay();
	}

	return 0;
}

void* memcpy( void * destination, const void * source, int num )
{
	uint8_t* d = destination;
	const uint8_t* s = source;

	for(int k= 0; k < num; k++)
		d[k] = s[k];

	return 0;
}

void delay()
{
	volatile int t = 2500000;

	while(t >0 )
	{
		t--;
		//GPIO_SetBits(GPIOD, GPIO_Pin_4);
		t--;
		//GPIO_ResetBits(GPIOD, GPIO_Pin_4);
	}
}




