#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QWidget>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QDebug>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include <QHeaderView>
#include <QChar>
#include <QTabWidget>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QCheckBox>
#include <QSaveFile>
#include <QColor>
#include <QFont>
#include "medium.h"
#include "CD.h"
#include "DVD.h"
#include "book.h"
#include "person.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitializeUI();

}

void MainWindow::InitializeUI(){
//------PersonenverwaltungTab initialisieren--------
    //Funktionalität zum add-Button hinzufügen
    QPushButton *addPerson = this->findChild<QPushButton*>("addButtonPerson");
    QObject::connect(addPerson, &QPushButton::clicked, [=](){

        //Auf Formlayout-Items zugreifen
        QLineEdit *firstName = this->findChild<QLineEdit *>("firstnameLineEdit");
        QLineEdit *lastName = this->findChild<QLineEdit *>("nameLineEdit");
        QLabel *warning = this->findChild<QLabel*>("warningPerson");

        //Formlayout auf leeren
        warning->setText("");
        lastName->setText("");
        firstName->setText("");

        //Sichtbarkeit auf das Add-Widget ändern
        SetViewPerson(false);
        SetAddPerson(true);
    });

    //Initialisierung der Buttons im Formlayout (sichern einer neuen Personen/abbrechen)
    QDialogButtonBox *addPersonButtons = this->findChild<QDialogButtonBox *>("personAddButtonBox");

    //Funktionalität zum save-Button hinzufügen
    QObject::connect(addPersonButtons, &QDialogButtonBox::accepted, [=](){

        //Auf Formlayout-Items zugreifen
        QLineEdit *firstName = this->findChild<QLineEdit *>("firstnameLineEdit");
        QLineEdit *lastName = this->findChild<QLineEdit *>("nameLineEdit");
        QLabel *warning = this->findChild<QLabel*>("warningPerson");

        //Überprüfung ob Felder auch mit Text befüllt sind
        if((firstName->text().trimmed()).isEmpty() || (lastName->text().trimmed()).isEmpty()){
            warning->setText("Die Texte Name und Vorname müssen befüllt sein");
            lastName->setText("");
            firstName->setText("");
            return;
        }
        //Überprüfung ob Person schon vorhanden -->sonst warning!!

        Person* temp = new Person(firstName->text(), lastName->text());
        personList.append(temp);
        LoadPerson();

        //Person hinzufügen

        //Sichtbarkeit auf das View-Widget ändern
        SetViewPerson(true);
        SetAddPerson(false);
    });

    //Funtionalität zum cancel-Button hinzufügen
    QObject::connect(addPersonButtons, &QDialogButtonBox::rejected, [=](){
        //Sichtbarkeit auf das View-Widget ändern
        SetViewPerson(true);
        SetAddPerson(false);
    });

    //Sichtbarkeit auf das View-Widget intialsieren
    SetViewPerson(true);
    SetAddPerson(false);

//------MediumverwaltungTab initialisieren--------
    //Funktionalität zum add-Button hinzufügen
    QPushButton *addMedium = this->findChild<QPushButton*>("addButtonMedium");
    QObject::connect(addMedium, &QPushButton::clicked, [=](){

        //Auf Formlayout-Items zugreifen
        QComboBox *medium = this->findChild<QComboBox *>("mediumComboBoxMedium");
        QLineEdit *title = this->findChild<QLineEdit *>("titleLineEditMedium");
        QLineEdit *author = this->findChild<QLineEdit *>("authorLineEditMedium");
        QLabel *warning = this->findChild<QLabel*>("warningMedium");

        //Formlayout auf leeren
        warning->setText("");
        medium->setCurrentText("Buch");
        author->setText("");
        title->setText("");

        //Sichtbarkeit auf das Add-Widget ändern
        SetViewMedium(false);
        SetAddMedium(true);
    });

    //Initialisierung der Buttons im Formlayout (sichern eines neuen Mediums/abbrechen)
    QDialogButtonBox *addMediumButtons = this->findChild<QDialogButtonBox *>("mediumAddButtonBox");

    //Funktionalität zum save-Button hinzufügen
    QObject::connect(addMediumButtons, &QDialogButtonBox::accepted, [=](){

        //Auf Formlayout-Items zugreifen
        QComboBox *medium = this->findChild<QComboBox *>("mediumComboBoxMedium");
        QLineEdit *title = this->findChild<QLineEdit *>("titleLineEditMedium");
        QLineEdit *author = this->findChild<QLineEdit *>("authorLineEditMedium");
        QLabel *warning = this->findChild<QLabel*>("warningMedium");

        //Überprüfung ob Titel-Feld auch mit Text befüllt ist
        if(title->text().trimmed().isEmpty()){
            warning->setText("Titel muss befüllt sein");
            return;
        }
        //Überprüfung ob Medium schon vorhanden ->sonst warning!!
        Medium* temp=NULL;
        if(medium->currentText()=="Buch")temp = new Book(title->text(), author->text());
        else if(medium->currentText()=="CD")temp= new CD(title->text(), author->text());
        else if(medium->currentText()=="DVD")temp= new DVD(title->text(), author->text());
        else{
            cerr << "Ungültige Eingabe " << endl;
            exit(-1);
        }
        mediumList.append(temp);
        LoadMedia();
        //Person hinzufügen

        //Sichtbarkeit auf das View-Widget ändern
        SetViewMedium(true);
        SetAddMedium(false);
    });

    //Funktionalität zum cancel-Button hinzufügen
    QObject::connect(addMediumButtons, &QDialogButtonBox::rejected, [=](){
        SetViewMedium(true);
        SetAddMedium(false);
    });

//Ausleige
    QPushButton *searchMedium = this->findChild<QPushButton*>("searchButtonBorrow");
    QObject::connect(searchMedium, &QPushButton::clicked, [=](){
    QTableWidget *table = this->findChild<QTableWidget *>("tableWidgetBorrow");
    table->setRowCount(0);
    QLineEdit *title = this->findChild<QLineEdit*>("titelLineEditBorrow");
    QLineEdit *author = this->findChild<QLineEdit*>("authorLineEditBorrow");
    QComboBox *mediumType = this->findChild<QComboBox*>("mediumComboBoxBorrow");
    QCheckBox *borrowed = this->findChild<QCheckBox*>("checkBoxBorrow");
    QString titleText = title->text();
    QString authorText = author->text();
    int row=0;
    bool showBorrowed = borrowed->checkState();
    for (int i = 0; i < mediumList.size(); i++) {
        QString mediumTitle = mediumList[i]->getTitle();
        if (!mediumTitle.startsWith(titleText, Qt::CaseInsensitive)||
            !mediumTitle.startsWith(authorText, Qt::CaseInsensitive)||
            !(mediumList[i]->getType()==mediumType->currentText())||
            !(mediumList[i]->getBorrower()==nullptr || showBorrowed))
            continue;
        table->insertRow(row);

        QTableWidgetItem *typeI =new QTableWidgetItem(mediumList[i]->getType());
        table->setItem(row, 0, typeI);

        QTableWidgetItem *titleB = new QTableWidgetItem(mediumList[i]->getTitle());
        table->setItem(row, 1, titleB);

        QTableWidgetItem *author = new QTableWidgetItem(mediumList[i]->getAuthor());
        table->setItem(row, 2, author);

        Medium* currM = mediumList[i];
        if(!(mediumList[i]->getBorrower()==nullptr)){
            QTableWidgetItem *borrowState =new QTableWidgetItem("Ausgeliehen");
            QColor redColor(Qt::red);
            borrowState->setForeground(redColor);
            QFont font;
            font.setBold(true);
            borrowState->setFont(font);
            borrowState->setForeground(redColor);
            table->setItem(row, 3, borrowState);
            continue;
        }

        QPushButton *borrowButton = new QPushButton("Ausleihen");
        table->setCellWidget(row, 3, borrowButton);
        QObject::connect(borrowButton, &QPushButton::clicked, [=](){
            QLineEdit * nameLineEdit = this->findChild<QLineEdit*>("nameLineEditBorrow");
            QLineEdit * firstnameLineEdit = this->findChild<QLineEdit*>("firstnameLineEditBorrow");
            QLabel *warning = this->findChild<QLabel*>("warningLabelBorrow");
            nameLineEdit->setText("");
            firstnameLineEdit->setText("");
            warning->setText("");
            SetAddBorrower(true);
            SetSearchMedium(false);
            QDialogButtonBox *borrowBox = this->findChild<QDialogButtonBox *>("buttonBoxBorrow");
            QObject::connect(borrowBox, &QDialogButtonBox::accepted, [=](){
                Person* borrower = NULL;
                for(int i = 0; i<personList.size(); i++){
                    if(personList[i]->getName().compare(nameLineEdit->text())==0 && personList[i]->getFirstname().compare(firstnameLineEdit->text())==0)
                        borrower=personList[i];
                }
                if(borrower!=NULL){
                    currM->setBorrower(borrower);
                    LoadMedia();
                    QObject::disconnect(borrowBox, &QDialogButtonBox::accepted, nullptr, nullptr);
                    SetAddBorrower(false);
                    SetSearchMedium(true);
                }
                else{
                    warning->setText("Person nicht gefunden");
                }
            });
            QObject::connect(borrowBox, &QDialogButtonBox::rejected, [=](){
                SetAddBorrower(false);
                SetSearchMedium(true);
                QObject::disconnect(borrowBox, &QDialogButtonBox::accepted, nullptr, nullptr);
            });
        });
        row++;
    }
    });

//Rückgabe
    QPushButton *searchButtonReturn = this->findChild<QPushButton*>("searchButtonReturn");
    QObject::connect(searchButtonReturn, &QPushButton::clicked, [=](){
        QLineEdit* firstnameB = this->findChild<QLineEdit*>("firstnameLineEditReturn");
        QLineEdit* nameB = this->findChild<QLineEdit*>("nameLineEditReturn");
        QLabel* warning = this->findChild<QLabel*>("warningLabelReturn");
        Person* person = NULL;
        for(int i = 0; i<personList.size(); i++){
            if(personList[i]->getName().compare(nameB->text())==0 && personList[i]->getFirstname().compare(firstnameB->text())==0)
                person=personList[i];
        }
        if(person!=NULL){
            QTableWidget *table = this->findChild<QTableWidget *>("tableWidgetReturn");
            table->setRowCount(0);
            int row =0;
            for(int i=0; i<mediumList.size(); i++){
                if(mediumList[i]->getBorrower() == person){
                    table->insertRow(row);
                    QTableWidgetItem *typeI =new QTableWidgetItem(mediumList[i]->getType());
                    table->setItem(row, 0, typeI);

                    QTableWidgetItem *titleB = new QTableWidgetItem(mediumList[i]->getTitle());
                    table->setItem(row, 1, titleB);

                    QTableWidgetItem *author = new QTableWidgetItem(mediumList[i]->getAuthor());
                    table->setItem(row, 2, author);

                    QPushButton *returnButton = new QPushButton("Rückgabe");
                    table->setCellWidget(row, 3, returnButton);
                    Medium* currM = mediumList[i];
                    QObject::connect(returnButton, &QPushButton::clicked, [=](){
                        currM->setBorrower(NULL);
                        LoadMedia();
                    });
                    row++;
                }
            }
        }
        else{
            warning->setText("Person nicht gefunden");
        }
    });



    //Sichtbarkeit auf das View-Widget initialisieren
    SetViewMedium(true);
    SetAddMedium(false);
    SetAddBorrower(false);
    SetSearchMedium(true);
    LoadPersonFromFile();
    LoadMediaFromFile();
    LoadMedia();
    LoadPerson();
}

void MainWindow::LoadMediaFromFile(){
    QFile file("Media.txt");
    //QFile::remove("Media.txt");
    if(!file.exists()){
        QSaveFile *file=new QSaveFile("Media.txt");
        file->open(QIODevice::WriteOnly | QIODevice::Text);
        file->commit();
        delete file;
    }
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd())
            mediumList.append(Medium::parse(in.readLine(),&personList));
        file.close();
    }
    else{
        cerr << "Fehler beim öffnen von Media.txt" <<endl;
        exit(-1);
    }
}

void MainWindow::LoadPersonFromFile(){
    QFile file("Person.txt");
    //QFile::remove("Person.txt");
    if(!file.exists()){
        QSaveFile *file=new QSaveFile("Person.txt");
        file->open(QIODevice::WriteOnly | QIODevice::Text);
        file->commit();
        delete file;
    }
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd())
            personList.append(Person::parse(in.readLine()));
        file.close();
    }
    else{
        cerr << "Fehler beim öffnen von Media.txt" <<endl;
                exit(-1);
    }
}

void MainWindow::SaveMedia(){
    QFile file("Media.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (Medium* medium : mediumList) {
            out << medium->print();
            if(medium->getBorrower()!=NULL){
                out << "," << personList.indexOf(medium->getBorrower()) << "\n";
            }
            else{
                out << ",\n";
            }
        }
        file.close();
    }
    else {
        cerr << "Fehler beim öffnen von Media.txt" << endl;
        exit(-1);
    }
}

void MainWindow::SavePerson(){
    QFile file("Person.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (Person* person : personList)
            out << person->print() << "\n";
        file.close();
    }
    else {
        cerr << "Fehler beim öffnen von Media.txt" << endl;
                exit(-1);
    }
}

MainWindow::~MainWindow()
{
    SavePerson();
    SaveMedia();
    delete ui;
}

void MainWindow::EditMedium(Medium* currM){
    SetViewMedium(false);
    SetAddMedium(true);
    QString type;
    switch(currM->print().split(",")[0].toInt()){
    case 1: type="Buch"; break;
    case 2: type="CD";break;
    case 3: type="DVD";break;
    }
    QComboBox* typeB = this->findChild<QComboBox *>("mediumComboBoxMedium");
    QLineEdit* titleB =this->findChild<QLineEdit *>("titleLineEditMedium");
    QLineEdit* authorB=this->findChild<QLineEdit *>("authorLineEditMedium");
    QLabel *warning = this->findChild<QLabel*>("warningMedium");
    typeB->setCurrentText(type);
    typeB->setDisabled(true);
    titleB->setText(currM->getTitle());
    authorB->setText(currM->getAuthor());

    QDialogButtonBox *addMediumButtons = this->findChild<QDialogButtonBox *>("mediumAddButtonBox");
    QObject::disconnect(addMediumButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
    QObject::disconnect(addMediumButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
    QObject::connect(addMediumButtons, &QDialogButtonBox::accepted, [=](){
        if(titleB->text().trimmed().isEmpty()){
            warning->setText("Titel muss befüllt sein");
                return;
        }
        //Überprüfung ob Medium schon vorhanden ->sonst warning!!
        currM->setTitle(titleB->text());
        currM->setAuthor(authorB->text());
        typeB->setDisabled(false);
        QObject::disconnect(addMediumButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(addMediumButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
        InitializeUI();
    });
    QObject::connect(addMediumButtons, &QDialogButtonBox::rejected, [=](){
        typeB->setDisabled(false);
        QObject::disconnect(addMediumButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(addMediumButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
        InitializeUI();
    });
}

void MainWindow::EditPerson(Person* currP){
    SetViewPerson(false);
    SetAddPerson(true);
    QLineEdit* nameB =this->findChild<QLineEdit *>("nameLineEdit");
    QLineEdit* firstnameB=this->findChild<QLineEdit *>("firstnameLineEdit");
    QLabel *warning = this->findChild<QLabel*>("warningPerson");
    nameB->setText(currP->getName());
    firstnameB->setText(currP->getFirstname());

    QDialogButtonBox *addPersonButtons = this->findChild<QDialogButtonBox *>("personAddButtonBox");
    QObject::disconnect(addPersonButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
    QObject::disconnect(addPersonButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
    QObject::connect(addPersonButtons, &QDialogButtonBox::accepted, [=](){
        if((firstnameB->text().trimmed()).isEmpty() || (nameB->text().trimmed()).isEmpty()){
            warning->setText("Die Texte Name und Vorname müssen befüllt sein");
                return;
        }
        //Überprüfung ob Medium schon vorhanden ->sonst warning!!
        currP->setName(nameB->text());
        currP->setFirstname(firstnameB->text());
        QObject::disconnect(addPersonButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(addPersonButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
        InitializeUI();
    });
    QObject::connect(addPersonButtons, &QDialogButtonBox::rejected, [=](){
        QObject::disconnect(addPersonButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(addPersonButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
        InitializeUI();
    });
}

void MainWindow::LoadMedia(){
    QTableWidget *table = this->findChild<QTableWidget *>("tableWidgetMedium");
    table->setSortingEnabled(false);
    table->setRowCount(0);
    for(int row=0; row<mediumList.size(); ++row){
        table->insertRow(row);
        QString type;
        switch(mediumList[row]->print().split(",")[0].toInt()){
        case 1: type="Buch"; break;
        case 2: type="CD";break;
        case 3: type="DVD";break;
        }

        QTableWidgetItem *typeI =new QTableWidgetItem(type);
        table->setItem(row, 0, typeI);

        QTableWidgetItem *title = new QTableWidgetItem(mediumList[row]->getTitle());
        table->setItem(row, 1, title);

        QTableWidgetItem *author = new QTableWidgetItem(mediumList[row]->getAuthor());
        table->setItem(row, 2, author);

        QTableWidgetItem *borrower=NULL;
        if(mediumList[row]->getBorrower()==NULL)
            borrower=new QTableWidgetItem("");
        else
            borrower=new QTableWidgetItem(mediumList[row]->getBorrower()->getFirstname()+" "+mediumList[row]->getBorrower()->getName());
        table->setItem(row, 3, borrower);

        QPushButton *deleteButton = new QPushButton("Löschen");
        table->setCellWidget(row, 4, deleteButton);

        Medium* currM = mediumList[row];
        QObject::connect(deleteButton, &QPushButton::clicked, [=](){
            int rowC = table->currentRow();
            table->removeRow(rowC);
            mediumList.removeAll(currM);
        });

        QPushButton *editButton = new QPushButton("Ändern");
        table->setCellWidget(row, 5, editButton);
        QObject::connect(editButton, &QPushButton::clicked, [=](){EditMedium(currM);});
    }
    table->setSortingEnabled(true);
}

void MainWindow::LoadPerson(){
    QTableWidget *table = this->findChild<QTableWidget *>("tableWidgetPerson");
    table->setRowCount(0);
    table->setSortingEnabled(false);
    for(int row=0; row<personList.size(); ++row){
        table->insertRow(row);

        QTableWidgetItem *firstname = new QTableWidgetItem(personList[row]->getFirstname());
        table->setItem(row, 0, firstname);

        QTableWidgetItem *name = new QTableWidgetItem(personList[row]->getName());
        table->setItem(row, 1, name);

        QPushButton *listButton = new QPushButton("Auflisten");
        table->setCellWidget(row, 2, listButton);

        Person* currP = personList[row];
        QObject::connect(listButton, &QPushButton::clicked, [=](){
            QTabWidget *tabWidget = this->findChild<QTabWidget*>("tabWidget");
            QPushButton *searchButton = this->findChild<QPushButton*>("searchButtonReturn");
            tabWidget->setCurrentIndex(3);
            QLineEdit* firstname = this->findChild<QLineEdit*>("firstnameLineEditReturn");
            QLineEdit* name = this->findChild<QLineEdit*>("nameLineEditReturn");
            firstname->setText(currP->getFirstname());
            name->setText(currP->getName());
            searchButton->click();
        });

        QPushButton *deleteButton = new QPushButton("Löschen");
        table->setCellWidget(row, 3, deleteButton);

        QObject::connect(deleteButton, &QPushButton::clicked, [=](){
            int rowC = table->currentRow();
            table->removeRow(rowC);
            personList.removeAll(currP);
        });

        QPushButton *editButton = new QPushButton("Ändern");
        table->setCellWidget(row, 4, editButton);
        QObject::connect(editButton, &QPushButton::clicked, [=](){EditPerson(currP);});
    }
    table->setSortingEnabled(true);
}

void MainWindow::SetViewPerson(bool visible){
    QWidget *ViewWP = this->findChild<QWidget*>("viewWidgetPerson");
    ViewWP->setVisible(visible);
}

void MainWindow::SetAddPerson(bool visible){
    QWidget *AddWP = this->findChild<QWidget*>("addWidgetPerson");
    AddWP->setVisible(visible);
}

void MainWindow::SetViewMedium(bool visible){
    QWidget *ViewWM = this->findChild<QWidget*>("viewWidgetMedium");
    ViewWM->setVisible(visible);
}

void MainWindow::SetAddMedium(bool visible){
    QWidget *AddWM = this->findChild<QWidget*>("addWidgetMedium");
    AddWM->setVisible(visible);
}

void MainWindow::SetSearchMedium(bool visible){
    QWidget *AddWM = this->findChild<QWidget*>("searchWidgetBorrow");
    AddWM->setVisible(visible);
}

void MainWindow::SetAddBorrower(bool visible){
    QWidget *AddWM = this->findChild<QWidget*>("borrowWidgetBorrow");
    AddWM->setVisible(visible);
}
