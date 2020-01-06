#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include "meteocapteur.h"
#include "meteoapi.h"
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
/*#include <QtCharts>
#include <QChartView>
#include <QSplineSeries>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>*/

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QString sDate = QDateTime::currentDateTime().toString("dddd dd MMMM yyyy");

    ui->setupUi(this);
    ui->rbCel->setChecked(true);
    ui->rbH24->setChecked(true);
    ui->lblHeure->setText(QDateTime::currentDateTime().toString("h:mm:ss"));
    ui->rbFra->setChecked(true);
    setFrench();
    ui->rbJour->setChecked(true);
    setJour();
    ui->lblDate->setText(sDate);
    ui->lblDate->setText(QDateTime::currentDateTime().toString("dddd dd MMMM yyyy"));

    update();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(60000);


}


void MainWindow::update() {

    MeteoCapteur *meteoCapteur = new MeteoCapteur;

    float tempC = meteoCapteur->getCTemp();
    float tempF = meteoCapteur->getFTemp();
    float hum = meteoCapteur->getHumidity();
    float pres = meteoCapteur->getPressure();


    ui->txtHum->setText(QString::number(hum, 'f', 1));
    ui->txtPres->setText(QString::number(pres, 'f', 1));

    QString nomVille = ui->cboChoixVille->currentText();

    QString unite="";
    QString langue="FR";

    if (ui->rbCel->isChecked()) {
        ui->txtTemp->setText(QString::number(tempC, 'f', 1));

        unite="metric";
        ui->lblUnitTemp->setText("(C)");
        ui->lblUnitTempExt->setText("(C)");
        ui->lblUnitTempMaxExt->setText("(C)");
        ui->lblUnitTempMinExt->setText("(C)");
        ui->lblUnitDiffTemp->setText("(C)");
    }
    else {
        ui->txtTemp->setText(QString::number(tempF, 'f', 1));
        ui->lblUnitTemp->setText("(F)");
        ui->lblUnitTempExt->setText("(F)");
        ui->lblUnitTempMaxExt->setText("(F)");
        ui->lblUnitTempMinExt->setText("(F)");
        ui->lblUnitDiffTemp->setText("(F)");
    }

    if (ui->rbEng->isChecked()) {
        langue="ENG";
    }


    MeteoApi *meteoApi = new MeteoApi(nomVille, unite, langue);
    ui->txtTempExt->setText(QString::number(meteoApi->getTemp(), 'f', 1));
    ui->txtPresExt->setText(QString::number(meteoApi->getPressure(), 'f', 1));
    ui->txtHumExt->setText(QString::number(meteoApi->getHumidity(), 'f', 1));
    ui->txtTempMinExt->setText(QString::number(meteoApi->getTempMin(), 'f', 1));
    ui->txtTempMaxExt->setText(QString::number(meteoApi->getTempMax(), 'f', 1));
    ui->lblVille->setText(meteoApi->getNomVille());
    ui->lblClim->setPixmap(meteoApi->getIcone());
    ui->lblDesc->setText(meteoApi->getDescription());

    double diff;

    if (ui->rbCel->isChecked()){
        diff = meteoCapteur->getCTemp() - meteoApi->getTemp();
    }
    else {
        diff = meteoCapteur->getFTemp() - meteoApi->getTemp();
    }
    ui->txtDiffTemp->setText(QString::number(diff, 'f', 1));
/*
        QNetworkRequest requestPrevision(QUrl("https://api.openweathermap.org/data/2.5/forecast?q="+nomVille+",FR&appid=30cee15a222b2ecc72642bae79b29814&units=metric&lang=FR"));
        requestPrevision.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkAccessManager manager;
        QNetworkReply * reply = manager.get(requestPrevision);
        while(!reply->isFinished())
        {
            qApp->processEvents();
        }
        QByteArray reponse = reply->readAll();
        QJsonDocument jsonReponse = QJsonDocument::fromJson(reponse);
        jsonReponse.toVariant().toString();
         reply->deleteLater();
         QJsonObject jsonObject = jsonReponse.object();
         QJsonArray listArray = jsonObject["list"].toArray();
         QSplineSeries * series = new QSplineSeries();
         series->setName("Relevés de températures toutes les 3 heures");
         for (int var = 0, h=0; var < listArray.count(); ++var ,h+=3)
         {
             QJsonObject objList = listArray[var].toObject();
             QJsonObject objMain = objList["main"].toObject();
             // qDebug() << "temp : " << objMain["temp"].toDouble();
             QJsonValue prevision = (objMain["temp"]);
             qreal valX = static_cast<qreal>(h);
             qreal valY = static_cast<qreal>(objMain["temp"].toDouble());
             qDebug() << "Point : " << valX << " " << valY;
             series->append(valX, valY);
         }

         QChart * chart = new QChart();
         chart->legend()->show();
         chart->addSeries(series);
         chart->setTitle("Prévisions météorologiques pour les 5 prochains jours :");
         chart->createDefaultAxes();
         chart->axes(Qt::Vertical).first()->setRange(-10, 30);
         chart->axisX()->hide();

         QCategoryAxis * axisX = new QCategoryAxis();
         QDate today = QDate::currentDate();

         QString jour = today.toString("dddd dd MMMM yyyy");
         qDebug() << jour;
         axisX->append(jour, 24);

         QString jour2 = today.addDays(1).toString("dddd dd MMMM yyyy");
         qDebug() << jour2;
         axisX->append(jour2, 48);

         QString jour3 = today.addDays(2).toString("dddd dd MMMM yyyy");
         qDebug() << jour3;
         axisX->append(jour3, 72);

         QString jour4 = today.addDays(3).toString("dddd dd MMMM yyyy");
         qDebug() << jour4;
         axisX->append(jour4, 96);

         QString jour5 = today.addDays(4).toString("dddd dd MMMM yyyy");
         qDebug() << jour5;
         axisX->append(jour5, 120);

         axisX->setRange(0, 120);

         chart->addAxis(axisX, Qt::AlignBottom);
         series->attachAxis(axisX);

         QChartView * chartView = new QChartView(chart);
         chartView->setRenderHint(QPainter::Antialiasing);
         ui->verticalLayout->takeAt(0);
         ui->verticalLayout->addWidget(chartView);
*/


}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_cboChoixVille_currentTextChanged(const QString &arg1)
{
 //   qDebug() << arg1;

    update();

}

void MainWindow::on_rbH12_clicked()
{
    ui->lblHeure->setText(QDateTime::currentDateTime().toString("h:mm:ss ap"));
}

void MainWindow::on_rbH24_clicked()
{
    ui->lblHeure->setText(QDateTime::currentDateTime().toString("h:mm:ss"));
}

void MainWindow::on_rbCel_clicked()
{
    ui->lblUnitTemp->setText("(C)");
    update();
}

void MainWindow::on_rbFah_clicked()
{
    ui->lblUnitTemp->setText("(F)");
    update();
}

void MainWindow::on_cboPolice_currentFontChanged(const QFont &f)
{
    ui->lblVille->setFont(f);
    ui->lblHum->setFont(f);
    ui->lblDate->setFont(f);
    ui->lblClim->setFont(f);
    ui->lblPres->setFont(f);
    ui->lblHeure->setFont(f);
    ui->lblTitre->setFont(f);
    ui->lblInt->setFont(f);
    ui->lblExt->setFont(f);
    ui->lblTemp->setFont(f);
    ui->lblTempExt->setFont(f);
    ui->lblPresExt->setFont(f);
    ui->lblHumExt->setFont(f);
    ui->lblDesc->setFont(f);
    ui->lblDiff->setFont(f);
    ui->lblTempMaxExt->setFont(f);
    ui->lblTempMinExt->setFont(f);
    ui->lblPara->setFont(f);
    ui->lblChoixVille->setFont(f);
    ui->lblChoixPolice->setFont(f);
    ui->rbCel->setFont(f);
    ui->rbFah->setFont(f);
    ui->rbH12->setFont(f);
    ui->rbH24->setFont(f);
    ui->rbFra->setFont(f);
    ui->rbEng->setFont(f);
    ui->rbJour->setFont(f);
    ui->rbNuit->setFont(f);
}


void MainWindow::setJour() {
    ui->lblDate->setStyleSheet("color: black;");
    ui->lblExt->setStyleSheet("color: black;");
    ui->lblTitre->setStyleSheet("color: black;");
    ui->lblTemp->setStyleSheet("color: black;");
    ui->lblPara->setStyleSheet("color: black;");
    ui->lblPres->setStyleSheet("color: black;");
    ui->lblInt->setStyleSheet("color: black;");
    ui->lblHum->setStyleSheet("color: black;");
    ui->lblHeure->setStyleSheet("color: black;");
    ui->lblHumExt->setStyleSheet("color: black;");
    ui->lblDiff->setStyleSheet("color: black;");
    ui->lblTempExt->setStyleSheet("color: black;");
    ui->lblTempMaxExt->setStyleSheet("color: black;");
    ui->lblTempMinExt->setStyleSheet("color: black;");
    ui->lblVille->setStyleSheet("color: black;");
    ui->lblUnitTemp->setStyleSheet("color: black;");
    ui->lblUnitTempExt->setStyleSheet("color: black;");
    ui->lblUnitTempMaxExt->setStyleSheet("color: black;");
    ui->lblUnitTempMinExt->setStyleSheet("color: black;");
    ui->lblPresExt->setStyleSheet("color: black;");
    ui->lblChoixVille->setStyleSheet("color: black;");
    ui->lblChoixPolice->setStyleSheet("color: black;");
    ui->rbH12->setStyleSheet("color: black;");
    ui->rbH24->setStyleSheet("color: black;");
    ui->rbCel->setStyleSheet("color: black;");
    ui->rbFah->setStyleSheet("color: black;");
    ui->rbFra->setStyleSheet("color: black;");
    ui->rbEng->setStyleSheet("color: black;");
    ui->rbJour->setStyleSheet("color: black;");
    ui->rbNuit->setStyleSheet("color: black;");

    QPixmap bkgnd(":/bg/jour.jpg");
        bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        this->setPalette(palette);
        this->setStyleSheet("color: black;");
}


void MainWindow::on_rbJour_clicked()
{

    setJour();
}

void MainWindow::on_rbNuit_clicked()
{

    ui->lblDate->setStyleSheet("color: white;");
    ui->lblExt->setStyleSheet("color: white;");
    ui->lblTitre->setStyleSheet("color: white;");
    ui->lblTemp->setStyleSheet("color: white;");
    ui->lblPara->setStyleSheet("color: white;");
    ui->lblPres->setStyleSheet("color: white;");
    ui->lblInt->setStyleSheet("color: white;");
    ui->lblHum->setStyleSheet("color: white;");
    ui->lblHeure->setStyleSheet("color: white;");
    ui->lblHumExt->setStyleSheet("color: white;");
    ui->lblDiff->setStyleSheet("color: white;");
    ui->lblTempExt->setStyleSheet("color: white;");
    ui->lblTempMaxExt->setStyleSheet("color: white;");
    ui->lblTempMinExt->setStyleSheet("color: white;");
    ui->lblVille->setStyleSheet("color: white;");
    ui->lblUnitTemp->setStyleSheet("color: white;");
    ui->lblUnitTempExt->setStyleSheet("color: white;");
    ui->lblUnitTempMaxExt->setStyleSheet("color: white;");
    ui->lblUnitTempMinExt->setStyleSheet("color: white;");
    ui->lblPresExt->setStyleSheet("color: white;");
    ui->lblChoixVille->setStyleSheet("color: white;");
    ui->lblChoixPolice->setStyleSheet("color: white;");
    ui->rbH12->setStyleSheet("color: white;");
    ui->rbH24->setStyleSheet("color: white;");
    ui->rbCel->setStyleSheet("color: white;");
    ui->rbFah->setStyleSheet("color: white;");
    ui->rbFra->setStyleSheet("color: white;");
    ui->rbEng->setStyleSheet("color: white;");
    ui->rbJour->setStyleSheet("color: white;");
    ui->rbNuit->setStyleSheet("color: white;");
    QPixmap bkgnd(":/bg/nuit.jpeg");
        bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        this->setPalette(palette);


}

void MainWindow::setEnglish() {
    ui->lblVille->setText("City name");
    ui->lblHum->setText("Humidity (%)");
    ui->lblDate->setText(QLocale{QLocale::English}.toString(QDateTime::currentDateTime(), "dddd MMMM dd yyyy"));
    ui->lblPres->setText("Pressure (hPa)");
    ui->lblTitre->setText("Meteo Station Project");
    ui->lblInt->setText("Indoor statements");
    ui->lblExt->setText("Outdoor statements");
    ui->lblTemp->setText("Temperature");
    ui->lblTempExt->setText("Temperature");
    ui->lblPresExt->setText("Pressure (hPa)");
    ui->lblHumExt->setText("Humidity (%)");
    ui->lblPara->setText("Settings");
    ui->lblChoixVille->setText("City choice");
    ui->lblChoixPolice->setText("Font choice");
    ui->rbJour->setText("Day");
    ui->rbNuit->setText("Night");
    ui->rbH12->setText("12h clock");
    ui->rbH24->setText("24h clock");
    ui->lblDiff->setText("In/Out Temp diff");
}

void MainWindow::on_rbEng_clicked()
{
    setEnglish();
    update();
}

void MainWindow::setFrench() {
    ui->lblVille->setText("Nom ville");
    ui->lblHum->setText("Humidité (%)");
    ui->lblDate->setText(QDateTime::currentDateTime().toString("dddd dd MMMM yyyy"));
    ui->lblPres->setText("Pression (hPa)");
    ui->lblTitre->setText("Projet Station Météo");
    ui->lblInt->setText("Mesures intérieures");
    ui->lblExt->setText("Mesures extérieures");
    ui->lblTemp->setText("Température");
    ui->lblTempExt->setText("Température");
    ui->lblPresExt->setText("Pression (hPa)");
    ui->lblHumExt->setText("Humidité (%)");
    ui->lblPara->setText("Paramètres");
    ui->lblChoixVille->setText("Choix de la ville");
    ui->lblChoixPolice->setText("Choix de la police");
    ui->rbJour->setText("Jour");
    ui->rbNuit->setText("Nuit");
    ui->rbH12->setText("Format 12h");
    ui->rbH24->setText("Format 24h");
    ui->lblDiff->setText("Diff Temp Int/Ext");
}

void MainWindow::on_rbFra_clicked()
{
    setFrench();
    update();
}
