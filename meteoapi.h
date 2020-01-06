#ifndef METEOAPI_H
#define METEOAPI_H


#include <QPixmap>

using namespace std;

class MeteoApi
{   

    private:

        float temp;
        float humidity;
        float pressure;
        float tempMax;
        float tempMin;
        QPixmap icone;
        QString nomVille;
        QString description;

    public:
        // Constructeur qui prend en paramètres la ville, l'unité et la langue saisies sur l'interface
        MeteoApi(QString nomVille, QString unite, QString langue);

        float getTemp();
        float getPressure();
        float getHumidity();
        float getTempMax();
        float getTempMin();
        QPixmap getIcone();
        QString getNomVille();
        QString getDescription();

 };


#endif // METEOAPI_H
