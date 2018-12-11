bool isDay()
{
    DateTime now = RTC.now();
    uint8_t hour = now.hour();
    return hour >= 7 && hour < 23;
}