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

    static QVector<AnimalData> loadAnimalFile(const QString &filename);
    static bool writeAnimalFile(const QVector<AnimalData> &,const QString &);
    static void createTexFileFromAnimalDataArray(QVector<AnimalData> &,const QString &filename);
    static QJsonObject metaData;

    QJsonObject & obj(){return (*this);}
    const QJsonObject & obj() const {return (*this);}
    QJsonObject obj(const QString &str){return obj()[str].toObject();}
    QJsonObject obj(const QString &str)const{return QJsonObject(obj()[str].toObject());}
};


#endif // CHARDATA_H
