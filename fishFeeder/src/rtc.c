#include <stm32f10x.h>
#include <misc.h>
#include <stm32f10x_rcc.h>

#include "rtc.h"
#include "terminal.h"

uint8_t mSecond = 0;
uint8_t mMinute = 0;
uint8_t mHour = 0;

uint8_t mMinuteValue = 0;
uint8_t mMinuteAlarm = 0;
uint8_t mMinuteAlarmCount = 0;
void (*minute_cb_func)(void);

uint8_t mHourValue = 0;
uint8_t mHourAlarm = 0;
uint8_t mHourAlarmCount = 0;
void (*hour_cb_func)(void);

void RTC_Configuration(void);

void initRTC()
{
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		/* Backup data register value is not correct or not yet programmed (when
		       the first time the program is executed) */

		t_print("Configuring RTC....\n");

		RTC_Configuration();

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		t_print("RTC has time....\n");
	}

	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();

	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Enable the RTC Interrupt */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void updateTime()
{
	uint32_t TimeVar =  RTC_GetCounter();

	/* Reset RTC Counter when Time is 23:59:59 */
	if (RTC_GetCounter() >= 0x0001517F)
	{
		RTC_SetCounter(0x0);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}

	/* Compute  hours */
	mHour = TimeVar / 3600;
	/* Compute minutes */
	mMinute = (TimeVar % 3600) / 60;
	/* Compute seconds */
	mSecond = (TimeVar % 3600) % 60;

}

void rtc_setMinuteAlarm(uint8_t minutes, void(*cb)(void))
{
	mMinuteAlarm = minutes;
	mMinuteAlarmCount = minutes;
	minute_cb_func = cb;
}

void rtc_setHourAlarm(uint8_t hours, void(*cb)(void))
{
	mHourAlarm = hours;
	mHourAlarmCount = hours;
	hour_cb_func = cb;
}

void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

}


void checkAlarms()
{
	if(!mMinuteValue)
		mMinuteValue = mMinute;

	if(!mHourValue)
			mHourValue = mHour;

	if(mMinuteValue != mMinute)
	{
		mMinuteValue = mMinute;

		if(mMinuteAlarm && (--mMinuteAlarmCount == 0))
		{
			mMinuteAlarmCount = mMinuteAlarm;

			if(minute_cb_func)
				minute_cb_func();
		}
	}

	if(mHourValue != mHour)
		{
			mHourValue = mHour;

			if(mHourAlarm && (--mHourAlarmCount == 0))
			{
				mHourAlarmCount = mHourAlarm;
				t_print("Hour Alarm!!\n");

				if(hour_cb_func)
					hour_cb_func();
			}
		}
}

void rtc_displayTime()
{
	t_print("Time: ");
	d_print(mHour);
	t_print(":");
	d_print(mMinute);
	t_print(":");
	d_print(mSecond);
	t_print("\n");
}

void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    updateTime();
    checkAlarms();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

  }
}
