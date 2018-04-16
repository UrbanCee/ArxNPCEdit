#ifndef CHARDATA_H
#define CHARDATA_H

#include <QtCore>
#include <QJsonObject>
#include <QVector>

#include "jsonclasses.h"
#include "texwriter.h"



class AnimalData : public QJsonObject
{

public:
    AnimalData();
    AnimalData(const QJsonObject &);
    AnimalData(const AnimalData &);
    QString toTexMinipageString() const ;

    static void createEmptyAnimalFile(QString filename);
    static QVector<AnimalData> loadAnimalFile(QString filename);
    static void createTexFileFromAnimalDataArray(QVector<AnimalData> &, QString filename);

    QJsonObject & obj(){return (*this);}
    const QJsonObject & obj() const {return (*this);}
    QJsonObject obj(const QString &str){return obj()[str].toObject();}
    QJsonObject obj(const QString &str)const{return QJsonObject(obj()[str].toObject());}
};


#endif // CHARDATA_H
