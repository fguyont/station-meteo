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
    /*float var1;
    float var2;*/
    float fTemp;
    float cTemp;
    float humidity;
    float pressure;
    /*int file;
    char* bus;
    char b1 [25];
    char reg [2];*/


public:
    MeteoCapteur();
    void initMesures();
    /*void setVar1 (float);
    void setVar2 (float);
    float getVar1();
    float getVar2();*/
    float getCTemp();
    float getFTemp();
    float getPressure();
    float getHumidity();

};

#endif // METEOCAPTEUR_H
