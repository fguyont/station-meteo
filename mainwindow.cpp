#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include "meteocapteur.h"
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   // QString sDate = QDateTime::currentDateTime().toString("dddd dd MMMM yyyy");

    MeteoCapteur *meteoCapteur = new MeteoCapteur;
    meteoCapteur->initMesures();

    float tempC = meteoCapteur->getCTemp();

    float tempF = meteoCapteur->getFTemp();
    float hum = meteoCapteur->getHumidity();
    float pres = meteoCapteur->getPressure();




    ui->setupUi(this);
    ui->rbCel->setChecked(true);
    ui->rbH24->setChecked(true);
    ui->rbFra->setChecked(true);
   // ui->lblDate->setText(sDate);
    ui->txtTemp->setText(QString::number(tempC, 'f', 1));
  //  ui->txtTemp->setText(QString::number(tempC) + " " + QString::number(tempF) + " " +QString::number(hum) + " " +QString::number(pres) + " "  );

    ui->txtHum->setText(QString::number(hum, 'f', 1));
    ui->txtPres->setText(QString::number(pres, 'f', 1));


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

    ui->txtTemp->setText(QString::number(tempC, 'f', 1));
    ui->txtHum->setText(QString::number(hum, 'f', 1));
    ui->txtPres->setText(QString::number(pres, 'f', 1));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_cboChoixVille_currentTextChanged(const QString &arg1)
{
    qDebug() << arg1;

    QString nomVille = arg1;
    //QString nomVille = ui->cboChoixVille->currentText();

    QString unite="";
    QString langue="FR";

    if (ui->rbCel->isChecked()) {
        unite="metric";
        ui->lblTempExt->setText("Temperature (C)");
        ui->lblTempMaxExt->setText("Temp Max (C)");
        ui->lblTempMinExt->setText("Temp Max (C)");
    }
    else {
        ui->lblTempExt->setText("Temperature (F)");
        ui->lblTempMaxExt->setText("Temp Max (F)");
        ui->lblTempMinExt->setText("Temp Max (F)");
    }

    if (ui->rbEng->isChecked()) {
        langue="ENG";
    }

    QString urlReq="https://api.openweathermap.org/data/2.5/weather?q="+nomVille+",FR&appid=30cee15a222b2ecc72642bae79b29814&units="+unite+"&lang="+langue;

    /**** CREATION DE LA REQUETE ****/

    /* D'après la documentation d'OpenWeatherMAp la requete Url comprend
     * l'adresse du site, mot cle weather, la ville, l'abréviation du pays,
     * ma clé api, l'unité celcius et la langue*/

   QNetworkRequest request(urlReq);

   // Pour mettre l'entete en http (Pour QNetworkReply)

  /* Pour cela on utilise la methode setHeader qui utilise */

   request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

   /**** GESTION DE LA REQUETE ****/

   /* La classe QNetworkAccessManager permet à l'application
    * d'envoyer des demandes au réseau et de recevoir des réponses
    * Ici on déclare manager le nom de la classe QNetworkAccessManager */

    QNetworkAccessManager manager;

    /* La classe QNetworkReply contient les données et les en-têtes d'une demande
     * envoyée avec QNetworkAccessManager
     * C'est pour cela que l'on a fait la ligne de code 38 request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
     * Ici on déclare reply le nom de la classe QNetworkReply
     * Et on affecte le manager manager avec la methode get() par rapport à la requete request
     * On met un .get() car c'est un pointeur */

    QNetworkReply * reply = manager.get(request);

    /* Tant que la requete n'est pas fini on bouclele processus de l'évenement  */
    while(!reply->isFinished())
    {
        qApp->processEvents();
    }

    /* La classe QByteArray fournit un tableau d'octets
     * Ici on déclare reponse le nom de la classe QByteArray
     * Et on affecte le reply avec la methode readAll() */

    QByteArray reponse = reply->readAll();

    /* On affiche la taille du tableau */

    qDebug() << "Taille du tableau récuperer de la requete : " << reponse.size();


    /**** GESTION DU JSON ****/

    /* La classe QJsonDocument permet de lire et d'écrire des documents JSON
     * Ici on déclare jsonReponse le nom de la classe QJsonDocument
     * Et on affecte le response via la methode statique fromJson() qui provient de QJsonDocument
     * La methode statique fromJson() analyse json en tant que document JSON encodé en UTF-8 et
     * crée un QJsonDocument à partir de celui-ci. Il renvoie un QJsonDocument valide (non nul)
     * si l'analyse réussit. S'il échoue, le document renvoyé sera nul et
     * la variable d' erreur facultative contiendra plus de détails sur l'erreur */

    QJsonDocument jsonReponse = QJsonDocument::fromJson(reponse);

    /* On affiche le JSON avec les methodes
     * toVariant() qui provient de la classe QVariant qui agit comme une union pour les types de données Qt
     * pour ici jsonReponse de type QJsonDocument et toString() de type QString
     * et la methode toString() pour les mettre en chaine de caractère   */

    jsonReponse.toVariant().toString();

    /* Ici on planifie la suppression du retour de la requete */

    reply->deleteLater();


    /**** Tout d'abord on va convertir le document jsonReponse en objet ****/

    /* La classe QJsonObject encapsule un objet JSON
     * Ici on déclare jsonObject le nom de la classe QJsonObject
     * Et on affecte le document jsonReponse via la méthode object() qui renvois le contenue de l'object */

    QJsonObject jsonObject = jsonReponse.object();

    /**** Ensuite on va convertir l'objet weather en tableau car dans la doc api l'objet weather est un tableau avec des clées et des valeurs ****/

    /* La classe QJsonArray encapsule un tableau JSON
     * Ici on déclare jsonArray le nom de la classe QJsonArray
     * Et on affecte l'objet jsonObject via la méthode toArray() qui un tableau */

    QJsonArray weatherArray = jsonObject["weather"].toArray();

    /**** Ensuite on va convertir plusieurs objets en objet car dans la doc api ces objets sont d'autres objets avec des clées et des valeurs ****/

    /* La classe QJsonObject encapsule un objet JSON
     * Ici on déclare main le nom de la classe QJsonObject
     * Et on affecte l'élément main de jsonObject via la méthode toObject() qui convertie l'élément en object */

    QJsonObject main = jsonObject["main"].toObject();

    /**** Et enfin on va faire une boucle pour parcourir et afficher toutes les informations ****/

    QPixmap pix;

    // boucle pour le tableau weather

    for (int var = 0; var < weatherArray.count(); ++var)
    {
        // On commence par convertir l'indice de type tableau en object

        /* La classe QJsonObject encapsule un objet JSON
         * Ici on déclare obj le nom de la classe QJsonObject
         * Et on affecte la valeur de l'indice du tableau jsonArray via la méthode toObject()
         * qui convertie l'élément JSON en object */

        QJsonObject obj = weatherArray[var].toObject();

        // On fini par afficher les informations

        qDebug() << "Ville : " << jsonObject["name"].toString();
        ui->lblVille->setText(jsonObject["name"].toString());

        /* On affiche la valeur de l'object icon tout en le convertissant
         * en chaine de caractère via la méthode toString */
        qDebug() << "icone : " << obj["icon"].toString();

        /* On affiche la valeur de l'object main tout en le convertissant
         * en chaine de caractère via la méthode toString */
        qDebug() << "Temps : " << obj["main"].toString();

        /* On affiche la valeur de l'object description tout en le convertissant
         * en chaine de caractère via la méthode toString */
        qDebug() << "Description : " << obj["description"].toString();
        ui->lblDesc->setText(obj["description"].toString());

        if (obj["description"].toString().contains("couvert") || obj["description"].toString().contains("nuageux") || obj["description"].toString().contains("clouds")) {
            pix = QPixmap (":/icones/clouds.png");
        }
        if (obj["description"].toString().contains("brume") || obj["description"].toString().contains("mist")) {
            pix = QPixmap (":/icones/mist.png");
        }
        if (obj["description"].toString().contains("dégagé") || obj["description"].toString().contains("clear")) {
            pix = QPixmap (":/icones/clear.png");
        }
        if (obj["description"].toString().contains("orageux") || obj["description"].toString().contains("thunder")){
            pix = QPixmap (":/icones/thunder.png");
        }
        if (obj["description"].toString().contains("pluie") || obj["description"].toString().contains("rain")){
            pix = QPixmap (":/icones/rain.png");
        }
        if (obj["description"].toString().contains("neige") || obj["description"].toString().contains("snow")) {
            pix = QPixmap (":/icones/snow.png");
        }

        ui->lblClim->setPixmap(pix);
    }

    /* On affiche les valeurs que l'on souhaite de l'object main tout en le convertissant
     * en double via la méthode toDouble */
    qDebug() << "Température actuelle : " << main["temp"].toDouble();
    ui->txtTempExt->setText(QString::number(main["temp"].toDouble(), 'f', 1));
    qDebug() << "Température minimale : " << main["temp_min"].toDouble();
    ui->txtTempMinExt->setText(QString::number(main["temp_min"].toDouble(), 'f', 1));
    qDebug() << "Température maximale : " << main["temp_max"].toDouble();
    ui->txtTempMaxExt->setText(QString::number(main["temp_max"].toDouble(), 'f', 1));
    qDebug() << "Pression: " << main["pressure"].toDouble();
    ui->txtPresExt->setText(QString::number(main["pressure"].toDouble(), 'f', 1));
    qDebug() << "Humidité : " << main["humidity"].toDouble();
    ui->txtHumExt->setText(QString::number(main["humidity"].toDouble(), 'f', 1));

    /*double diffTemp, tempExt;
    tempExt = main["temp"].toDouble();

    if (tempC > tempExt) {
        diffTemp = tempC - tempExt;
    }
    else {
        diffTemp = tempExt - tempC;
    }

    ui->txtDiffTemp->setText(QString::number(diffTemp, 'f', 1));*/

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
    ui->lblTemp->setText("Température (C)");
}

void MainWindow::on_rbFah_clicked()
{
    ui->lblTemp->setText("Température (F)");
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
    ui->lblTempMin->setFont(f);
    ui->lblTempMax->setFont(f);
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




void MainWindow::on_rbJour_clicked()
{
    this->setStyleSheet("color: blue;"
                        "background-color: orange;"
                        "selection-color: orange;"
                        "selection-background-color: blue;");
}

void MainWindow::on_rbNuit_clicked()
{
    this->setStyleSheet("color: white;"
                        "background-color: black;"
                        "selection-color: white;"
                        "selection-background-color: black;");
}

void MainWindow::on_rbEng_clicked()
{
    ui->lblVille->setText("City name");
    ui->lblHum->setText("Humidity");
    ui->lblDate->setText(QLocale{QLocale::English}.toString(QDateTime::currentDateTime(), "dddd MMMM dd yyyy"));
    ui->lblPres->setText("Pressure");
    ui->lblHeure->setText("Hour");
    ui->lblTitre->setText("Meteo Station Project");
    ui->lblInt->setText("Indoor statements");
    ui->lblExt->setText("Outdoor statements");
    ui->lblTemp->setText("Temperature");
    ui->lblTempExt->setText("Temperature");
    ui->lblPresExt->setText("Pressure");
    ui->lblHumExt->setText("Humidity");
    ui->lblPara->setText("Settings");
    ui->lblChoixVille->setText("City choice");
    ui->lblChoixPolice->setText("Font choice");
    ui->rbJour->setText("Day");
    ui->rbNuit->setText("Night");
    ui->rbH12->setText("12h clock");
    ui->rbH24->setText("24h clock");
}

void MainWindow::on_rbFra_clicked()
{
    ui->lblVille->setText("Nom ville");
    ui->lblHum->setText("Humidité");
    ui->lblDate->setText(QDateTime::currentDateTime().toString("dddd dd MMMM yyyy"));
    ui->lblPres->setText("Pression");
    ui->lblHeure->setText("Heure");
    ui->lblTitre->setText("Projet Station Météo");
    ui->lblInt->setText("Mesures intérieures");
    ui->lblExt->setText("Mesures extérieures");
    ui->lblTemp->setText("Température");
    ui->lblTempExt->setText("Température");
    ui->lblPresExt->setText("Pression");
    ui->lblHumExt->setText("Humidité");
    ui->lblPara->setText("Paramètres");
    ui->lblChoixVille->setText("Choix de la ville");
    ui->lblChoixPolice->setText("Choix de la police");
    ui->rbJour->setText("Jour");
    ui->rbNuit->setText("Nuit");
    ui->rbH12->setText("Format 12h");
    ui->rbH24->setText("Format 24h");
}
