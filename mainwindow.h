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

    void InitializeUI();
    void LoadMedia();
    void SaveMedia();
    void SavePerson();
    void LoadMediaFromFile();
    void LoadPerson();
    void LoadPersonFromFile();
    void EditMedium(Medium* currM);
    void EditPerson(Person* currP);
    void SetAddPerson(bool visible);
    void SetViewPerson(bool visible);
    void SetAddMedium(bool visible);
    void SetViewMedium(bool visible);
    void SetSearchMedium(bool visible);
    void SetAddBorrower(bool visible);
};
#endif // MAINWINDOW_H
