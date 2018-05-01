#ifndef JSONCLASSES_H
#define JSONCLASSES_H

#include <QJsonObject>
#include <QJsonArray>

bool writeJson(const QString &filename,const QJsonObject &object);
QJsonObject readJson(const QString &filename);

#endif // JSONCLASSES_H
