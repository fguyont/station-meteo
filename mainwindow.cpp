#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include "meteocapteur.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QString sDate = QDateTime::currentDateTime().toString("dddd dd MMMM yyyy hh:mm:ss");

    MeteoCapteur *meteoCapteur = new MeteoCapteur;
  //  meteoCapteur->initMesures();

    float tempC = meteoCapteur->getCTemp();

    float tempF = meteoCapteur->getFTemp();
    float hum = meteoCapteur->getHumidity();
    float pres = meteoCapteur->getPressure();


    //QTimer::singleShot(myTime, myObject, SLOT(myMethodInMyObject()));

   // QTimer::singleShot(100, this, SLOT(tempC = meteoCapteur->getCTemp()));


    ui->setupUi(this);
    ui->lblDateHeure->setText(sDate);
    ui->txtTemp->setText(QString::number(tempC, 'f', 1));
    //ui->txtTemp->setText(QString::number(tempC) + " " + QString::number(tempF) + " " +QString::number(humidite) + " " +QString::number(pression) + " "  );

    ui->txtHum->setText(QString::number(hum, 'f', 1));
    ui->txtPres->setText(QString::number(pres, 'f', 1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

