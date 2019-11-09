#ifndef RELAY_BUS
#define RELAY_BUS 12
#endif

#define RELAY_ON 1
#define RELAY_OFF 0

class RelayControl
{
public:
    RelayControl();
    void init(int *theTemp, float *temperature);
    void setRelay(uint8_t hour);
    void setRelayOn();
    void setRelayOff();

private:
    int _bus;
    bool _relayStatus = 0;
    float *_temperature;
    int *_theTemp;
    void checkTemp(int bottom, int top);
};

extern RelayControl RC;

RelayControl::RelayControl() {}

void RelayControl::init(int *theTemp, float *temperature)
{
    _theTemp = theTemp;
    _temperature = temperature;
    pinMode(RELAY_BUS, OUTPUT);
}

void RelayControl::setRelay(uint8_t hour)
{
    if (hour >= 5 && hour < 7)
    {
        checkTemp(2, 5);
        return;
    }
    if (hour >= 7 && hour < 23)
    {
        checkTemp(-1, 1);
        return;
    }
    checkTemp(0, 2);
}

void RelayControl::checkTemp(int bottom, int top)
{
    if (*_temperature < *_theTemp + bottom && _relayStatus == RELAY_OFF)
    {
        _relayStatus = RELAY_ON;
       setRelayOn();
    }
    if (*_temperature > *_theTemp + top && _relayStatus == RELAY_ON)
    {
        _relayStatus = RELAY_OFF;
        setRelayOff();
    }
}

void RelayControl::setRelayOn()
{
    digitalWrite(RELAY_BUS, RELAY_ON);
}

void RelayControl::setRelayOff()
{
    digitalWrite(RELAY_BUS, RELAY_OFF);
}

RelayControl RC = RelayControl();