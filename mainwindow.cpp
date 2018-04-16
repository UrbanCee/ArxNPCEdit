#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QtGui>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createWidgets()
{
    QJsonObject metaData = readJson(":/res/MetaData_Animals.Arx.json");
    QStringList listAttrValues;
    for (QJsonArray::iterator it = metaData["Attribute"].toArray().begin();it!=metaData["Attribute"].toArray().end();it++)
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
        gridAttributes->addWidget(new CAttributeSpinBox(ui->frameAttributes,str),iRow,1,1,1);
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
        gridAddVals->addWidget(new CAttributeSpinBox(ui->groupBoxAdditionalValues,listAdditionalValues.at(iStrIdx++)),iRow,1,1,1);
        if (iRow!=5 && iRow!=6 && iRow!=7){
            gridAddVals->addWidget(new QLabel("/"),iRow,2,1,1);
            gridAddVals->addWidget(new CAttributeSpinBox(ui->groupBoxAdditionalValues,listAdditionalValues.at(iStrIdx++)),iRow,3,1,1);
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

    QGridLayout * gridKampfwerte = new QGridLayout();
    QStringList listKampfwertItems;
    listKampfwertItems<< "Angriffsart:" << "Angr. Stock+FP:" << "Ini Stock+Würfel" << "AvAngr" << "Schaden Würfel+Stock";
    iRow=0;
    foreach(QString str,listKampfwertItems)
    {
        gridKampfwerte->addWidget(new QLabel(str,ui->groupBoxWeapons),iRow++,0,1,1);
    }
    butAddWeapon = new QPushButton("Neue Angriffsart",ui->groupBoxWeapons);
    connect(butAddWeapon,SIGNAL(clicked(bool)),this,SLOT(createNewWeapon()));
    gridKampfwerte->addWidget(butAddWeapon,0,1,1,1);
    gridKampfwerte->setColumnStretch(10,100);
    ui->groupBoxWeapons->setLayout(gridKampfwerte);
}

void MainWindow::addWeaponToGrid(CWeaponObject *wep)
{
    int iColumn=weaponObjects.size()+1;
    weaponObjects.append(wep);
    QGridLayout * grid = dynamic_cast<QGridLayout *>(ui->groupBoxWeapons->layout());
    grid->addWidget(butAddWeapon,0,iColumn+1,1,1);
    int iRow=0;
    grid->addWidget(wep->edtName,iRow++,iColumn,1,1);
    QHBoxLayout *hboxAttack = new QHBoxLayout;
    hboxAttack->addWidget(wep->boxAngriffStock);
    hboxAttack->addWidget(wep->boxAngriffFP);
    hboxAttack->addWidget(wep->labelDice);
    grid->addLayout(hboxAttack,iRow++,iColumn,1,1);
    QHBoxLayout *hboxIni = new QHBoxLayout;
    hboxIni->addWidget(wep->boxIniStock);
    hboxIni->addWidget(wep->edtIniDice);
    grid->addLayout(hboxIni,iRow++,iColumn,1,1);
    QHBoxLayout *hboxAV = new QHBoxLayout;
    hboxAV->addWidget(wep->boxAvAngr);
    hboxAV->addStretch(100);
    grid->addLayout(hboxAV,iRow++,iColumn,1,1);
    QHBoxLayout *hboxDmg = new QHBoxLayout;
    hboxDmg->addWidget(wep->edtDmgDice);
    hboxDmg->addWidget(wep->boxDmgStock);
    grid->addLayout(hboxDmg,iRow++,iColumn,1,1);
}

void MainWindow::createNewWeapon()
{
    addWeaponToGrid(new CWeaponObject(ui->groupBoxWeapons));
}
