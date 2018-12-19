void dayLogic(float tempeature)
{
    if (tempeature < 5 && relayStatus == 0)
    {
        changeStatus(ON);
    }
    if (tempeature > 10 && relayStatus == 1)
    {
        changeStatus(OFF);
    }
}

void nightLogic(float temperature)
{
    if (temperature < 15 && relayStatus == 0)
    {
        changeStatus(ON);
    }
    if (temperature > 25 && relayStatus == 1)
    {
        changeStatus(OFF);
    }
}

void reserveLogic(float temperature)
{
    if (temperature < 3 && relayStatus == 1)
    {
        digitalWrite(RESERVE_RELAY_BUS, ON);
    }

    if (temperature > 10)
    {
        digitalWrite(RESERVE_RELAY_BUS, OFF);
    }
}

void changeStatus(bool status)
{
    relayStatus = status;
    digitalWrite(PRIME_RELAY_BUS, status);
}