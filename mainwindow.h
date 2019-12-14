#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLinkedList>
#include <QPushButton>
#include "customwidgets.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void addNPC();
    void deleteCurrentNPC();
    void createNewWeapon();
    void removeWeapon(CWeaponObject *);
private slots:
    void on_actionNPC_Datei_laden_triggered();
    void data2Gui();
    void gui2Data();
    void activeNPCchanged();
    void updateName();

    void on_actionNPC_Datei_speichern_triggered();

    void on_actionTEX_File_erzeugen_triggered();

    void on_actionPdf_File_erzeugen_LaTeX_req_triggered();

private:
    Ui::MainWindow *ui;
    QVector<AnimalData> animalData;
    QVector<CObjectInterface *> attributes;
    QVector<CObjectInterface *> additionals;
    QVector<CSkillObject *> skillsObjects;
    QLinkedList<CWeaponObject *> weaponObjects;
    QVector<QLabel *> weaponLabels;
    QPushButton *butAddWeapon;
    int iUnsavedIndex=0;
    void createWidgets();
    void addWeaponToGrid(CWeaponObject *);
    void updateWeaponLayout();
};

#endif // MAINWINDOW_H
