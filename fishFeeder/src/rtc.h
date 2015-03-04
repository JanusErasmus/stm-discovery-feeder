#include <stm32f10x_rtc.h>
#include <stm32f10x_bkp.h>
#include <stm32f10x_pwr.h>

void initRTC();
void rtc_displayTime(void);

/* Set an alarm that will happen each set minutes */
void rtc_setMinuteAlarm(uint8_t minutes, void(*cb)(void));

/* Set an alarm that will happen each set hours */
void rtc_setHourAlarm(uint8_t hours, void(*cb)(void));
