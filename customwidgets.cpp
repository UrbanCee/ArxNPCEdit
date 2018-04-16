#include "customwidgets.h"
#include<QtGui>

CObjectInterface::CObjectInterface(const QString &name)
    :qstrName(name)
{}


CAttributeSpinBox::CAttributeSpinBox(QWidget *parent, const QString &name)
    :QSpinBox(parent),CObjectInterface(name)
{
        setRange(0,99);
        setValue(0);
}

CSkillSpinBox::CSkillSpinBox(QWidget *parent, const QString &name)
    :QSpinBox(parent),CObjectInterface(name)
{
        setRange(0,10);
        setValue(0);
        connect(this,SIGNAL(valueChanged(int)),this,SLOT(updateDice()));
}

QString CSkillSpinBox::getDice()
{
    return getDice(value());
}




CSkillObject::CSkillObject(QWidget *parent, const QString &name)
    :QObject(parent),CObjectInterface(name)
{
    boxStock = new CAttributeSpinBox(parent,"Stock");
    boxFP = new CSkillSpinBox(parent,"FP");
    labelDice = new QLabel("W6",parent);
    labelSkillName = new QLabel(QString("%1:").arg(qstrName),parent);
    changeEnableStatus(0);
    connect(boxFP,SIGNAL(diceUpdated(QString)),labelDice,SLOT(setText(QString)));
    connect(boxStock,SIGNAL(valueChanged(int)),this,SLOT(changeEnableStatus(int)));
}

QJsonObject CSkillObject::getSkillObj()
{
    QJsonObject obj;
    obj.insert("Stock",boxStock->value());
    obj.insert("FP",boxFP->value());
    return obj;
}

void CSkillObject::setValues(QJsonObject obj)
{
    boxStock->setValue(obj["Stock"].toInt());
    boxFP->setValue(obj["FP"].toInt());
}

void CSkillObject::setJsonValue(QJsonValue value)
{
    setValues(value.toObject());
}

void CSkillObject::changeEnableStatus(int i)
{
    labelDice->setEnabled(i!=0);
    labelSkillName->setEnabled(i!=0);
}





void CSkillSpinBox::updateDice()
{
    emit diceUpdated(getDice());
}


CAttributeLineEdit::CAttributeLineEdit(QWidget *parent, const QString &name, const QString &placeholder)
    :QLineEdit(parent),CObjectInterface(name)
{
    //setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    setPlaceholderText(placeholder);
}



QString CSkillSpinBox::getDice(int i)
{
    switch(i)
    {
    case 1:
        return QString("2W4");
    case 2:
        return QString("W6+W4");
    case 3:
        return QString("2W6");
    case 4:
        return QString("W8+W6");
    case 5:
        return QString("2W8");
    case 6:
        return QString("W10+2W4");
    case 7:
        return QString("W10+W6+W4");
    case 8:
        return QString("W10+2W6");
    case 9:
        return QString("W12+2W6");
    case 10:
        return QString("W12+2W6+W4");
    default:
        return QString("W6");
    }
}

QStringList CWeaponObject::metaData;

CWeaponObject::CWeaponObject(QWidget *parent)
    :QObject(parent)
{
    if (metaData.isEmpty())
        loadMetaData();
    initialize(parent);
}

CWeaponObject::CWeaponObject(QWidget *parent, QJsonObject weapon)
    :QObject(parent)
{
    if (metaData.isEmpty())
        loadMetaData();
    initialize(parent);
    setToValues(weapon);
}

void CWeaponObject::setToValues(const QJsonObject &weapon)
{
    foreach (CObjectInterface *obj,weaponObjects) {
        obj->setJsonValue(weapon[obj->getName()]);
    }
}

void CWeaponObject::loadMetaData()
{
    QJsonObject fullMeta=readJson(":/res/MetaData_Animals.Arx.json");
    QJsonArray weaponArray=fullMeta["Kampfwerte"].toArray();
    if (metaData.size()==weaponArray.size())
        return;
    for (QJsonArray::iterator it=weaponArray.begin();it!=weaponArray.end();it++)
        metaData << (*it).toString();
}

void CWeaponObject::initialize(QWidget *parent)
{
    if (weaponObjects.size()>0)
        return;
    edtName = new CAttributeLineEdit(parent,metaData.at(0),"Angriffsart");
    boxAngriffStock = new CAttributeSpinBox(parent,metaData.at(1));
    boxAngriffFP = new CSkillSpinBox(parent,metaData.at(2));
    labelDice = new QLabel("W6",parent);
    boxIniStock = new CAttributeSpinBox(parent,metaData.at(3));
    edtIniDice = new CAttributeLineEdit(parent,metaData.at(4),"W6?");
    boxAvAngr = new CAttributeSpinBox(parent,metaData.at(5));
    edtDmgDice = new CAttributeLineEdit(parent,metaData.at(7),"zB 2W8");
    boxDmgStock = new CAttributeSpinBox(parent,metaData.at(6));
    connect(boxAngriffFP,SIGNAL(diceUpdated(QString)),labelDice,SLOT(setText(QString)));
    weaponObjects.append(edtName);
    weaponObjects.append(boxAngriffStock);
    weaponObjects.append(boxAngriffFP);
    weaponObjects.append(boxIniStock);
    weaponObjects.append(edtIniDice);
    weaponObjects.append(boxAvAngr);
    weaponObjects.append(boxDmgStock);
    weaponObjects.append(edtDmgDice);
}


