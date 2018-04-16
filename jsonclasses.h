#ifndef JSONCLASSES_H
#define JSONCLASSES_H

#include <QJsonObject>
#include <QJsonArray>

bool writeJson(QString filename, QJsonObject object);

QJsonObject readJson(QString filename);

#endif // JSONCLASSES_H
