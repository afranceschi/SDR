#include <Arduino.h>
#include "SDR_LIB.h"

SDR_LIB sdr_obj;

void setup()
{
    sdr_obj.Init();
}

void loop()
{
    sdr_obj.Main();
}
