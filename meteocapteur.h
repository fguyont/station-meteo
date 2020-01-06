

#ifndef METEOCAPTEUR_H
#define METEOCAPTEUR_H

#include <QObject>
#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

class MeteoCapteur
{

private:

    float fTemp;
    float cTemp;
    float humidity;
    float pressure;



public:
    MeteoCapteur();

    float getCTemp();
    float getFTemp();
    float getPressure();
    float getHumidity();

};

#endif  METEOCAPTEUR_H


