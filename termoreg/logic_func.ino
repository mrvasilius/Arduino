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

void nightLogic(float tempeature)
{
    if (tempeature < 15 && relayStatus == 0)
    {
        changeStatus(ON);
    }
    if (tempeature > 25 && relayStatus == 1)
    {
        changeStatus(OFF);
    }
}

void changeStatus(bool status)
{
    relayStatus = status;
    digitalWrite(RELAY_BUS, status);
}