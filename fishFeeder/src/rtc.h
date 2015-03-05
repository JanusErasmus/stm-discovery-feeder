

void initRTC();
void rtc_setTime(char * argv[], int argc);

/* Set an alarm that will happen each set minutes */
void rtc_setMinuteAlarm(uint8_t minutes, void(*cb)(void));

/* Set an alarm that will happen each set hours */
void rtc_setHourAlarm(uint8_t hours, void(*cb)(void));
