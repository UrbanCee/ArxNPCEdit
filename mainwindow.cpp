#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QtGui>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createWidgets();
    ui->comboAnimalSelector->addItem(QString(),"");
    ui->comboAnimalSelector->setCurrentIndex(0);
    animalData.append(AnimalData());
    connect(ui->pushButtonAddNew,SIGNAL(clicked(bool)),this,SLOT(addNPC()));
    connect(ui->pushButtonDeleteCurrent,SIGNAL(clicked(bool)),this,SLOT(deleteCurrentNPC()));
    connect(ui->comboAnimalSelector,SIGNAL(currentIndexChanged(int)),this,SLOT(activeNPCchanged()));
    connect(ui->lineEditName,SIGNAL(textEdited(QString)),this,SLOT(updateName()));
    ui->pushButtonDeleteCurrent->setEnabled(false);
    data2Gui();
}

MainWindow::~MainWindow()
{
    while(!weaponObjects.isEmpty())
        weaponObjects.last()->removeThisObject();
    delete ui;
}

void MainWindow::updateName()
{
    ui->comboAnimalSelector->setItemText(ui->comboAnimalSelector->currentIndex(),ui->lineEditName->text());
}

void MainWindow::addNPC()
{
    gui2Data();
    ui->comboAnimalSelector->blockSignals(true);
    ui->comboAnimalSelector->addItem(QString());
    ui->comboAnimalSelector->setCurrentIndex(ui->comboAnimalSelector->count()-1);
    ui->comboAnimalSelector->blockSignals(false);
    animalData.append(AnimalData());
    iUnsavedIndex=ui->comboAnimalSelector->currentIndex();
    data2Gui();
    ui->pushButtonDeleteCurrent->setEnabled(true);
}

void MainWindow::deleteCurrentNPC()
{
    if (ui->comboAnimalSelector->count()<=1)
        return;
    ui->comboAnimalSelector->blockSignals(true);
    animalData.remove(ui->comboAnimalSelector->currentIndex());
    ui->comboAnimalSelector->removeItem(ui->comboAnimalSelector->currentIndex());
    ui->comboAnimalSelector->blockSignals(false) ;
    ui->pushButtonDeleteCurrent->setEnabled(ui->comboAnimalSelector->count()>1);
    data2Gui();
}

void MainWindow::activeNPCchanged()
{
    gui2Data();
    data2Gui();
}


void MainWindow::addWeaponToGrid(CWeaponObject *wep)
{
    weaponObjects.append(wep);
    connect(wep,SIGNAL(removeMe(CWeaponObject*)),this,SLOT(removeWeapon(CWeaponObject*)));
    updateWeaponLayout();
}


void MainWindow::createNewWeapon()
{
    addWeaponToGrid(new CWeaponObject(ui->groupBoxWeapons));
}

void MainWindow::removeWeapon(CWeaponObject *wep)
{
    weaponObjects.removeAll(wep);
    delete wep;
    updateWeaponLayout();
}

void MainWindow::on_actionNPC_Datei_laden_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Öffne NPC Datei",QDir::currentPath(),"*.animal.json");
    if (fileName.isEmpty() || !fileName.toLower().endsWith(".animal.json"))
        return;
    animalData.clear();
    animalData = AnimalData::loadAnimalFile(fileName);
    ui->comboAnimalSelector->blockSignals(true);
    ui->comboAnimalSelector->clear();
    foreach(const AnimalData &animal,animalData)
    {
        ui->comboAnimalSelector->addItem(animal["Name"].toString());
        ui->comboAnimalSelector->setCurrentIndex(0);
    }
    ui->comboAnimalSelector->blockSignals(false);
    ui->pushButtonDeleteCurrent->setEnabled(animalData.size()>1);
    data2Gui();
}

void MainWindow::on_actionNPC_Datei_speichern_triggered()
{
    gui2Data();
    QString fileName = QFileDialog::getSaveFileName(this,"Speichere NPC Datei",QDir::currentPath(),"*.animal.json");
    if (fileName.isEmpty())
        return;
    if (!fileName.toLower().endsWith(".animal.json"))
        fileName.append(".animal.json");
    AnimalData::writeAnimalFile(animalData,fileName);
}

void MainWindow::on_actionTEX_File_erzeugen_triggered()
{
    gui2Data();
    QString fileName = QFileDialog::getSaveFileName(this,"Exportiere TEX file",QDir::currentPath(),"*.tex");
    if (fileName.isEmpty())
        return;
    if (!fileName.toLower().endsWith(".tex"))
        fileName.append(".tex");
    AnimalData::createTexFileFromAnimalDataArray(animalData,fileName);
}



void MainWindow::data2Gui()
{
    int iIndex=ui->comboAnimalSelector->currentIndex();
    if (iIndex>=animalData.size())
        return;
    iUnsavedIndex=iIndex;
    while(!weaponObjects.isEmpty())
        weaponObjects.last()->removeThisObject();
    AnimalData &animal=animalData[iIndex];
    ui->lineEditName->setText(animal["Name"].toString());
    QJsonObject skillJsonObject = animal["Fertigkeiten"].toObject();
    foreach (CSkillObject* obj,skillsObjects) {
        obj->setJsonValue(skillJsonObject[obj->getName()].toObject());
    }
    QJsonObject attributeJsonObject = animal["Attribute"].toObject();
    foreach (CObjectInterface* obj,attributes) {
        obj->setJsonValue(attributeJsonObject[obj->getName()].toInt());
    }
    QJsonObject additionalJsonObject = animal["Weitere Werte"].toObject();
    foreach (CObjectInterface* obj,additionals) {
        obj->setJsonValue(additionalJsonObject[obj->getName()].toInt());
    }
    QJsonArray weapons = animal["Kampfwerte"].toArray();
    for (QJsonArray::iterator it = weapons.begin();it!=weapons.end();it++)
    {
        QJsonObject weapon = (*it).toObject();
        addWeaponToGrid(new CWeaponObject(ui->groupBoxWeapons,weapon));
    }
    QJsonArray specials = animal["Besondere Fähigkeiten"].toArray();
    QString specialString;
    for (QJsonArray::iterator it = specials.begin();it!=specials.end();it++)
    {
        specialString.append((*it).toString()).append("\n");
    }
    ui->textEditSpecials->setPlainText(specialString);


}

void MainWindow::gui2Data()
{
    if (iUnsavedIndex>=animalData.size() || iUnsavedIndex>=ui->comboAnimalSelector->count())
        return;
    AnimalData newData;
    newData.insert("Name",ui->comboAnimalSelector->itemText(iUnsavedIndex));
    QJsonObject skillJsonObject;
    foreach (CSkillObject* obj,skillsObjects) {
        skillJsonObject.insert(obj->getName(),obj->getJsonValue());
    }
    QJsonObject attributeJsonObject;
    foreach (CObjectInterface* obj,attributes) {
        attributeJsonObject.insert(obj->getName(),obj->getJsonValue());
    }
    QJsonObject additionalJsonObject;
    foreach (CObjectInterface* obj,additionals) {
        additionalJsonObject.insert(obj->getName(),obj->getJsonValue());
    }
    newData.insert("Attribute",attributeJsonObject);
    newData.insert("Weitere Werte",additionalJsonObject);
    newData.insert("Fertigkeiten",skillJsonObject);
    QJsonArray weapons;
    foreach (CWeaponObject *wep,weaponObjects) {
        weapons.append(wep->getJsonObject());
    }
    newData.insert("Kampfwerte",weapons);
    QString qstrSpecialString = ui->textEditSpecials->toPlainText();
    QStringList specialList = qstrSpecialString.split(QString("\n"));
    QJsonArray specials;
    foreach(const QString &str,specialList)
    {
        specials.append(str);
    }
    newData.insert("Besondere Fähigkeiten",specials);
    animalData[iUnsavedIndex]=newData;

}




void MainWindow::updateWeaponLayout()
{
    QGridLayout * grid = new QGridLayout();
    int iRow=0;
    foreach(QLabel *label,weaponLabels)
    {
        grid->addWidget(label,iRow++,0,1,1);
    }
    int iColumn=1;
    for (QLinkedList<CWeaponObject *>::iterator it = weaponObjects.begin(); it != weaponObjects.end(); it++)
    {
        iRow=0;
        (*it)->createLayouts();
        grid->addLayout((*it)->hboxName,iRow++,iColumn,1,1);
        grid->addLayout((*it)->hboxAttack,iRow++,iColumn,1,1);
        grid->addLayout((*it)->hboxIni,iRow++,iColumn,1,1);
        grid->addLayout((*it)->hboxAV,iRow++,iColumn,1,1);
        grid->addLayout((*it)->hboxDmg,iRow++,iColumn,1,1);
        iColumn++;
    }

    grid->addWidget(butAddWeapon,0,iColumn,1,1);
    grid->setColumnStretch(10,100);
    QGridLayout * oldGrid = dynamic_cast<QGridLayout *>(ui->groupBoxWeapons->layout());
    if (oldGrid!=0)
    {
        QLayoutItem *item;
        while((item = oldGrid->takeAt(0)) != 0)
            delete item;
    }
    delete oldGrid;
    ui->groupBoxWeapons->setLayout(grid);
}




void MainWindow::createWidgets()
{
    QJsonObject metaData = readJson(":/res/MetaData_Animals.Arx.json");
    QStringList listAttrValues;
    QJsonArray attributeArray = metaData["Attribute"].toArray();
    for (QJsonArray::iterator it = attributeArray.begin();it!=attributeArray.end();it++)
    {
        listAttrValues << (*it).toString();
    }
    listAttrValues << "Lebensblock" << "Ausdauerblock" << "Manablock" << "Vorkommen";
    QGridLayout * gridAttributes= new QGridLayout;
    QFrame *lineA = new QFrame;
    lineA->setFrameShape(QFrame::HLine);
    lineA->setFrameShadow(QFrame::Sunken);
    QFrame *lineB = new QFrame;
    lineB->setFrameShape(QFrame::HLine);
    lineB->setFrameShadow(QFrame::Sunken);
    int iRow=0;
    foreach(const QString &str,listAttrValues)
    {
        if (iRow==7){
            gridAttributes->addWidget(lineA,iRow++,0,1,2);}
        else if (iRow==11){
            gridAttributes->addWidget(lineB,iRow++,0,1,2);}
        gridAttributes->addWidget(new QLabel(QString("%1:").arg(str)),iRow,0,1,1);
        CAttributeSpinBox *newAttributeSpinBox = new CAttributeSpinBox(ui->frameAttributes,str);
        if (iRow>7)
            additionals.append(newAttributeSpinBox);
        else
            attributes.append(newAttributeSpinBox);
        gridAttributes->addWidget(newAttributeSpinBox,iRow,1,1,1);
        iRow++;
    }
    ui->frameAttributes->setLayout(gridAttributes);

    QGridLayout * gridAddVals = new QGridLayout();
    iRow=0;
    gridAddVals->addWidget(new QLabel("Laufleistung einfach/voll:"),iRow++,0,1,1);
    gridAddVals->addWidget(new QLabel("Schwimmleistung einfach/voll:"),iRow++,0,1,1);
    gridAddVals->addWidget(new QLabel("Flugleistung einfach/voll:"),iRow++,0,1,1);
    gridAddVals->addWidget(new QLabel("Tragkraft einfach/voll:"),iRow++,0,1,1);
    gridAddVals->addWidget(new QLabel("Stemm- und Zugkraft:"),iRow++,0,1,1);
    gridAddVals->addWidget(new QLabel("Geistige Festigkeit:"),iRow++,0,1,1);
    gridAddVals->addWidget(new QLabel("Magieresistenz:"),iRow++,0,1,1);
    gridAddVals->addWidget(new QLabel("Magierenitenz:"),iRow++,0,1,1);
    gridAddVals->addWidget(new QLabel("Rüstungsschutz mag./nat."),iRow++,0,1,1);
    gridAddVals->addWidget(new QLabel("AvR / BeMa"),iRow++,0,1,1);
    iRow=0;
    QStringList listAdditionalValues;
    listAdditionalValues << "Laufleistung einfach" << "Laufleistung voll" << "Schwimmleistung einfach" << "Schwimmleistung voll";
    listAdditionalValues << "Flugleistung einfach" << "Flugleistung voll" << "Tragkraft einfach" << "Tragkraft voll" << "Stemmkraft" << "Zugkraft";
    listAdditionalValues << "Geistige Festigkeit" << "Magieresistenz" << "Magierenitenz";
    listAdditionalValues << "Rüstungsschutz natürlich" << "Rüstungsschutz magisch" << "Ausdauerverbrauch Rüstung" << "Bewegungsmalus";
    int iStrIdx=0;
    while(iStrIdx<listAdditionalValues.size()){
        CAttributeSpinBox *newAttributeSpinBox1 =new CAttributeSpinBox(ui->groupBoxAdditionalValues,listAdditionalValues.at(iStrIdx++));
        gridAddVals->addWidget(newAttributeSpinBox1,iRow,1,1,1);
        additionals.append(newAttributeSpinBox1);
        if (iRow!=5 && iRow!=6 && iRow!=7){
            gridAddVals->addWidget(new QLabel("/"),iRow,2,1,1);
            CAttributeSpinBox *newAttributeSpinBox2 = new CAttributeSpinBox(ui->groupBoxAdditionalValues,listAdditionalValues.at(iStrIdx++));
            gridAddVals->addWidget(newAttributeSpinBox2,iRow,3,1,1);
            additionals.append(newAttributeSpinBox2);
        }
        iRow++;
    }
    ui->groupBoxAdditionalValues->setLayout(gridAddVals);

    QGridLayout * gridSkills = new QGridLayout();
    gridSkills->addWidget(new QLabel("Stock"),0,1,1,1);
    gridSkills->addWidget(new QLabel("FP"),0,2,1,1);
    gridSkills->addWidget(new QLabel("Würfel"),0,3,1,1);

    QJsonArray skills = metaData["Fertigkeiten"].toArray();
    iRow=1;
    for (QJsonArray::iterator it = skills.begin(); it!=skills.end();it++)
    {
        CSkillObject *skill = new CSkillObject(ui->groupBoxSkills,(*it).toString());
        skillsObjects.append(skill);
        gridSkills->addWidget(skill->labelSkillName,iRow,0,1,1);
        gridSkills->addWidget(skill->boxStock,iRow,1,1,1);
        gridSkills->addWidget(skill->boxFP,iRow,2,1,1);
        gridSkills->addWidget(skill->labelDice,iRow,3,1,1);
        iRow++;
    }
    gridSkills->setColumnMinimumWidth(3,70);
    ui->groupBoxSkills->setLayout(gridSkills);

    QStringList listKampfwertItems;
    listKampfwertItems<< "Angriffsart:" << "Angr. Stock+FP:" << "Ini Stock+Würfel" << "AvAngr" << "Schaden Würfel+Stock";
    foreach(QString str,listKampfwertItems)
    {
        weaponLabels.append(new QLabel(str,ui->groupBoxWeapons));
    }
    butAddWeapon = new QPushButton("+",ui->groupBoxWeapons);
    butAddWeapon->setMaximumWidth(30);
    connect(butAddWeapon,SIGNAL(clicked(bool)),this,SLOT(createNewWeapon()));
    updateWeaponLayout();
}



