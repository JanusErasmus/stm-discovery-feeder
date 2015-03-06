#include <stm32f10x.h>
#include <misc.h>

#include "terminal.h"
#include "led.h"
#include "temp.h"
#include "rtc.h"

void delay();

typedef int size_t;
void* memcpy( void * destination, const void * source, size_t num );

void secondAction(uint8_t hour, uint8_t minute)
{
	t_print("Stop feeding the fish\n");

	rtc_setSecondAlarm(0,0);

}

void hourAction(uint8_t hour, uint8_t minute)
{
	t_print("==== HOURLY ===\n");
	t_print("An HOUR has passed\n");
	d_print(hour);
	t_print(":");

	switch(hour)
	{
	case 5:
	case 9:
	case 13:
	case 17:
		t_print("Feed the fish\n");
		rtc_setSecondAlarm(15, secondAction);
		break;
	default:
		break;
	}
}

/// Main function.  Called by the startup code.
int main(void)
{
	//when interrupts are going to be used
	NVIC_Configuration();

	initTerminal();

	t_print("Fish Feeder - Build: ");
	t_print(__DATE__);
	t_print(" ");
	t_print(__TIME__);
	t_print("\n");


	initLED();
	initTemp();
	initRTC();

	rtc_setHourAlarm(1, hourAction);

	t_print(">>");

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




