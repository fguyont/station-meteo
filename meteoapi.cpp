#include "meteoapi.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <mainwindow.h>

MeteoApi::MeteoApi(QString nomVille, QString unite, QString langue)
{


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

    //  qDebug() << "Taille du tableau récuperer de la requete : " << reponse.size();


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

   //       qDebug() << "Ville : " << jsonObject["name"].toString();
    //      ui->lblVille->setText(jsonObject["name"].toString());
          this->nomVille=jsonObject["name"].toString();

          /* On affiche la valeur de l'object icon tout en le convertissant
           * en chaine de caractère via la méthode toString */
   //       qDebug() << "icone : " << obj["icon"].toString();

          /* On affiche la valeur de l'object main tout en le convertissant
           * en chaine de caractère via la méthode toString */
     //     qDebug() << "Temps : " << obj["main"].toString();

          /* On affiche la valeur de l'object description tout en le convertissant
           * en chaine de caractère via la méthode toString */
     //     qDebug() << "Description : " << obj["description"].toString();
     //     ui->lblDesc->setText(obj["description"].toString());
          this->description=obj["description"].toString();

          if (obj["description"].toString().contains("couvert") || obj["description"].toString().contains("nuageux") || obj["description"].toString().contains("clouds")) {
              pix = QPixmap (":/icones/clouds.png");
          }
          if (obj["description"].toString().contains("brume") || obj["description"].toString().contains("brouillard") || obj["description"].toString().contains("mist") || obj["description"].toString().contains("fog")) {
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

          this->icone=pix;
      }

      /* On affiche les valeurs que l'on souhaite de l'object main tout en le convertissant
       * en double via la méthode toDouble */
   //   qDebug() << "Température actuelle : " << main["temp"].toDouble();
   //   ui->txtTempExt->setText(QString::number(main["temp"].toDouble(), 'f', 1));
      this->temp=main["temp"].toDouble();
   //   qDebug() << "Température minimale : " << main["temp_min"].toDouble();
   //   ui->txtTempMinExt->setText(QString::number(main["temp_min"].toDouble(), 'f', 1));
      this->tempMin=main["temp_min"].toDouble();
   //   qDebug() << "Température maximale : " << main["temp_max"].toDouble();
   //   ui->txtTempMaxExt->setText(QString::number(main["temp_max"].toDouble(), 'f', 1));
      this->tempMax=main["temp_max"].toDouble();
   //   qDebug() << "Pression: " << main["pressure"].toDouble();
   //   ui->txtPresExt->setText(QString::number(main["pressure"].toDouble(), 'f', 1));
      this->pressure=main["pressure"].toDouble();
   //   qDebug() << "Humidité : " << main["humidity"].toDouble();
   //   ui->txtHumExt->setText(QString::number(main["humidity"].toDouble(), 'f', 1));
      this->humidity=main["humidity"].toDouble();
  /*
      double diffTemp, tempExt;
      tempExt = main["temp"].toDouble();

      if (ui->rbCel->isChecked()) {

          if (tempC > tempExt) {
              diffTemp = tempC - tempExt;
          }
          else {
              diffTemp = tempExt - tempC;
          }
      }

      else {
          if (tempC > tempExt) {
              diffTemp = tempF - tempExt;
          }
          else {
              diffTemp = tempExt - tempF;
          }
      }
      ui->txtDiffTemp->setText(QString::number(diffTemp, 'f', 1));
  */



}

float MeteoApi::getTemp() {
    return this->temp;
}

float MeteoApi::getTempMin() {
    return this->tempMin;
}

float MeteoApi::getTempMax() {
    return this->tempMax;
}

float MeteoApi::getPressure() {
    return this->pressure;
}

float MeteoApi::getHumidity(){
    return this->humidity;
}

QString MeteoApi::getNomVille(){
    return this->nomVille;
}

QPixmap MeteoApi::getIcone(){
    return this->icone;
}

QString MeteoApi::getDescription(){
    return this->description;
}
