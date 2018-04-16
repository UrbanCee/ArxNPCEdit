#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <QSpinBox>
#include <QLabel>
#include <QtCore>
#include <QLineEdit>
#include <QJsonObject>
#include <QVector>
#include "chardata.h"
#include "texwriter.h"
#include "jsonclasses.h"

class CObjectInterface
{
public:
    CObjectInterface(const QString &name);
    virtual QJsonValue getJsonValue() = 0;
    virtual void setJsonValue(QJsonValue) = 0;
    QString getName(){return qstrName;}
protected:
    QString qstrName;
};


class CAttributeSpinBox : public QSpinBox, public CObjectInterface
{
    Q_OBJECT
public:
    CAttributeSpinBox(QWidget *, const QString &);
    QJsonValue getJsonValue() {return QJsonValue(value());}
    void setJsonValue(QJsonValue value){setValue(value.toInt());}
};

class CSkillSpinBox : public QSpinBox, public CObjectInterface
{
    Q_OBJECT
public:
    CSkillSpinBox(QWidget *, const QString &);
    QString getDice();
    static QString getDice(int);
    QJsonValue getJsonValue() {return QJsonValue(value());}
    void setJsonValue(QJsonValue value){setValue(value.toInt());}
public slots:
    void updateDice();
signals:
    diceUpdated(QString);
};

class CSkillObject : public QObject, public CObjectInterface
{
    Q_OBJECT
public:
    CSkillObject(QWidget *parent, const QString &);
    QJsonObject getSkillObj();
    CAttributeSpinBox *boxStock;
    CSkillSpinBox *boxFP;
    QLabel *labelDice, *labelSkillName;
    void setValues(QJsonObject);
    QJsonValue getJsonValue() {return QJsonValue(getSkillObj());}
    void setJsonValue(QJsonValue value);
public slots:
    void changeEnableStatus(int i);
};

class CAttributeLineEdit : public QLineEdit, public CObjectInterface
{
    Q_OBJECT
public:
    CAttributeLineEdit(QWidget *, const QString &, const QString &);
    QJsonValue getJsonValue() {return QJsonValue(text());}
    void setJsonValue(QJsonValue value){setText(value.toString());}
private:
};


class CWeaponObject : public QObject
{
    Q_OBJECT
public:
    CWeaponObject(QWidget *parent);
    CWeaponObject(QWidget *parent,QJsonObject weapon);
    CAttributeLineEdit *edtName;
    CAttributeSpinBox *boxAngriffStock;
    CSkillSpinBox *boxAngriffFP;
    QLabel *labelDice;
    CAttributeSpinBox *boxIniStock;
    CAttributeLineEdit *edtIniDice;
    CAttributeSpinBox *boxAvAngr;
    CAttributeLineEdit *edtDmgDice;
    CAttributeSpinBox *boxDmgStock;
    void setToValues(const QJsonObject &);
    static void loadMetaData();
private:
    static QStringList metaData;
    void initialize(QWidget *);
    QVector<CObjectInterface *> weaponObjects;
};



QString fp2dice(int iFP);



#endif // CUSTOMWIDGETS_H
