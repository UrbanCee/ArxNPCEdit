#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <QSpinBox>
#include <QLabel>
#include <QtCore>
#include <QLineEdit>
#include <QJsonObject>
#include <QVector>
#include <QPushButton>
#include <QLayout>
#include "chardata.h"
#include "texwriter.h"
#include "jsonclasses.h"

class CObjectInterface
{
public:
    CObjectInterface(const QString &name);
    virtual ~CObjectInterface() = 0;
    virtual QJsonValue getJsonValue() const = 0;
    virtual void setJsonValue(QJsonValue) = 0;
    QString getName() const {return qstrName;}
protected:
    QString qstrName;
};


class CAttributeSpinBox : public QSpinBox, public CObjectInterface
{
    Q_OBJECT
public:
    CAttributeSpinBox(QWidget *, const QString &);
    QJsonValue getJsonValue() const {return QJsonValue(value());}
    void setJsonValue(QJsonValue value){setValue(value.toInt());}
};

class CSkillSpinBox : public QSpinBox, public CObjectInterface
{
    Q_OBJECT
public:
    CSkillSpinBox(QWidget *, const QString &);
    ~CSkillSpinBox(){}
    QString getDice();
    static QString getDice(int);
    QJsonValue getJsonValue() const {return QJsonValue(value());}
    void setJsonValue(QJsonValue value){setValue(value.toInt());}
public slots:
    void updateDice();
signals:
    void diceUpdated(QString);
};

class CSkillObject : public QObject, public CObjectInterface
{
    Q_OBJECT
public:
    CSkillObject(QWidget *parent, const QString &);
    ~CSkillObject(){}
    QJsonObject getSkillObj() const;
    CAttributeSpinBox *boxStock;
    CSkillSpinBox *boxFP;
    QLabel *labelDice, *labelSkillName;
    void setValues(QJsonObject);
    QJsonValue getJsonValue() const {return QJsonValue(getSkillObj());}
    void setJsonValue(QJsonValue value);
public slots:
    void changeEnableStatus(int i);
};

class CAttributeLineEdit : public QLineEdit, public CObjectInterface
{
    Q_OBJECT
public:
    CAttributeLineEdit(QWidget *, const QString &, const QString &);
    ~CAttributeLineEdit(){}
    QJsonValue getJsonValue() const {return QJsonValue(text());}
    void setJsonValue(QJsonValue value){setText(value.toString());}
private:
};


class CWeaponObject : public QObject
{
    Q_OBJECT
public:
    CWeaponObject(QWidget *parent);
    CWeaponObject(QWidget *parent,QJsonObject weapon);
    ~CWeaponObject();
    void setToValues(const QJsonObject &);
    QJsonObject getJsonObject() const;
    static void loadMetaData();
    QHBoxLayout *hboxName;
    QHBoxLayout *hboxAttack;
    QHBoxLayout *hboxIni;
    QHBoxLayout *hboxAV;
    QHBoxLayout *hboxDmg;
    void createLayouts();
public slots:
    void removeThisObject();
signals:
    void removeMe(CWeaponObject *);
private:
    static QStringList metaData;
    void initialize(QWidget *);
    QVector<CObjectInterface *> weaponObjects;

    CAttributeLineEdit *edtName;
    CAttributeSpinBox *boxAngriffStock;
    CSkillSpinBox *boxAngriffFP;
    QLabel *labelDice;
    CAttributeSpinBox *boxIniStock;
    CAttributeLineEdit *edtIniDice;
    CAttributeSpinBox *boxAvAngr;
    CAttributeLineEdit *edtDmgDice;
    CAttributeSpinBox *boxDmgStock;
    QPushButton *butDelete;

};



QString fp2dice(int iFP);



#endif // CUSTOMWIDGETS_H
