#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "medium.h"
#include <QMainWindow>
#include <QDialogButtonBox>
#include <QList>
#include "person.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<Medium*> mediumList;
    QList<Person*> personList;

    //Initialiserungsfunktionen
    void InitializeUI();
    void InitializePersonTab();
    void InitializeMediaTab();
    void InitializeBorrowTab();
    void InitializeReturnTab();

    //Laden von Medien/Personen in die Tabellen
    void LoadMedia();
    void LoadPerson();

    //Dateiarbeit
    void SaveMedia();
    void SavePerson();
    void LoadMediaFromFile();
    void LoadPersonFromFile();

    void EditMedium(Medium* currM);
    void EditPerson(Person* currP);
    
    void borrow(Medium* medium);
    int searchPerson(QString firstname, QString name);

    //Funktionen zur Umstellung der Sichtbarkeit
    void SetAddPerson(bool visible);
    void SetViewPerson(bool visible);
    void SetAddMedium(bool visible);
    void SetViewMedium(bool visible);
    void SetSearchMedium(bool visible);
    void SetAddBorrower(bool visible);
};
#endif // MAINWINDOW_H
