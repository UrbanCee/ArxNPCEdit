#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPushButton>
#include "customwidgets.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void createNewWeapon();
private:
    Ui::MainWindow *ui;
    QVector<CSkillObject *> skillsObjects;
    QVector<CWeaponObject *> weaponObjects;
    QPushButton *butAddWeapon;
    void createWidgets();
    void addWeaponToGrid(CWeaponObject *);
};

#endif // MAINWINDOW_H
