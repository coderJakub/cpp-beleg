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
    LoadPersonFromFile();
    LoadMediaFromFile();
    InitializeUI();
}

void MainWindow::InitializePersonTab(){

//---------------------------------------------------------
//---Sichtbarkeit auf das Tabellen-Widget initialisieren---
//---------------------------------------------------------

    SetViewPerson(true);
    SetAddPerson(false);

//------------------------------------------------------
//---Funktionalität zum "addButtonPerson" hinzufügen---
//------------------------------------------------------

    //Pointer auf "addButtonPerson"-Button einrichten
    QPushButton *addPerson = this->findChild<QPushButton*>("addButtonPerson");

    //Funktion hinzufügen, die beim click-Event des Buttons aufgerufen wird
    //->Add-Widget soll sichbar werden und geleert werden
    QObject::connect(addPerson, &QPushButton::clicked, [=](){

        //Auf Formlayout-Items zugreifen
        QLineEdit *firstName = this->findChild<QLineEdit *>("firstnameLineEdit");
        QLineEdit *lastName = this->findChild<QLineEdit *>("nameLineEdit");
        QLabel *warning = this->findChild<QLabel*>("warningPerson");

        //Formlayout leeren
        warning->setText("");
        lastName->setText("");
        firstName->setText("");

        //Sichtbarkeit auf das Add-Widget ändern
        SetViewPerson(false);
        SetAddPerson(true);
    });

//---------------------------------------------------------
//---Funktionalität zur "personAddButtonBox" hinzufügen---
//---------------------------------------------------------

    //Pointer auf "personAddButtonBox"-DialogButtonBox einrichten 
    QDialogButtonBox *addPersonButtons = this->findChild<QDialogButtonBox *>("personAddButtonBox");

    //Funktion hinzufügen, die beim accepted-Event der DialogButtonBox aufgerufen wird
    //->Person soll zur personList hinzugefügt werden wenn diese noch nicht existiert
    //->Danach soll personList erneut in der Tabelle geladen werden
    //->Add-Widget wird wieder ausgeblendet
    QObject::connect(addPersonButtons, &QDialogButtonBox::accepted, [=](){

        //Auf Formlayout-Items zugreifen
        QLineEdit *firstName = this->findChild<QLineEdit *>("firstnameLineEdit");
        QLineEdit *lastName = this->findChild<QLineEdit *>("nameLineEdit");
        QLabel *warning = this->findChild<QLabel*>("warningPerson");

        //Überprüfung ob Felder auch mit Text befüllt sind
        if((firstName->text().trimmed()).isEmpty() || (lastName->text().trimmed()).isEmpty()){
            warning->setText("Die Texte Name und Vorname müssen befüllt sein!");
            return;
        }
        
        //Überprüfung ob Person schon existiert
        //->searchPerson erzeugt Rückgabewert -1, wenn sich noch keine Person mit gleichem Vor- und Nachnamen im System befindet
        if(searchPerson(firstName->text(), lastName->text())!=-1){
            warning->setText(firstName->text()+" "+lastName->text()+" ist schon im System eingefügt wurden!");
            return;
        }

        //Überprüfung wenn ob die Eingabe ein Komma enthält
        //->sonst würde Fehler bei Dateiarbeit entstehen
        if(firstName->text().contains(',') || lastName->text().contains(',')){
            warning->setText("Es darf bei der Eingabe kein , benutzt werden");
            return;
        }   

        //Person zur Liste hinzufügen
        personList.append(new Person(firstName->text(), lastName->text()));

        //Erneutes Laden der personList in die Tabelle
        LoadPerson();

        //Sichtbarkeit auf das View-Widget ändern
        SetViewPerson(true);
        SetAddPerson(false);
    });

    //Funktion hinzufügen, die beim rejected-Event der DialogButtonBox aufgerufen wird
    //->lediglich Sichtbarkeit auf View-Widget ändern
    QObject::connect(addPersonButtons, &QDialogButtonBox::rejected, [=](){

        //Sichtbarkeit auf das View-Widget ändern
        SetViewPerson(true);
        SetAddPerson(false);
    });
    LoadPerson();
}

void MainWindow::InitializeMediaTab(){

//---------------------------------------------------------
//---Sichtbarkeit auf das Tabellen-Widget initialisieren---
//---------------------------------------------------------

    SetViewMedium(true);
    SetAddMedium(false);

//------------------------------------------------------
//---Funktionalität zum "addButtonPerson" hinzufügen---
//------------------------------------------------------

    //Pointer auf "addButtonMedium"-Button einrichten
    QPushButton *addMedium = this->findChild<QPushButton*>("addButtonMedium");

    //Funktion hinzufügen, die beim click-Event des Buttons aufgerufen wird
    //->Add-Widget soll sichbar werden und geleert werden
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
    
//---------------------------------------------------------
//---Funktionalität zur "mediumAddButtonBox" hinzufügen---
//---------------------------------------------------------

    //Pointer auf "personAddButtonBox"-DialogButtonBox einrichten
    QDialogButtonBox *addMediumButtons = this->findChild<QDialogButtonBox *>("mediumAddButtonBox");

    //Funktion hinzufügen, die beim accepted-Event der DialogButtonBox aufgerufen wird
    //->Medium soll zur mediaList hinzugefügt werden
    //->Danach soll mediaList erneut in der Tabelle geladen werden
    //->Add-Widget wird wieder ausgeblendet
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

        //Überprüfung wenn ob die Eingabe ein Komma enthält
        //->sonst würde Fehler bei Dateiarbeit entstehen
        if(title->text().contains(',') || author->text().contains(',')){
            warning->setText("Es darf bei der Eingabe kein , benutzt werden");
            return;
        }   

        //Erstellung des Objekts von geforderter Klasse
        Medium* temp=NULL;
        if(medium->currentText()=="Buch")temp = new Book(title->text(), author->text());
        else if(medium->currentText()=="CD")temp= new CD(title->text(), author->text());
        else if(medium->currentText()=="DVD")temp= new DVD(title->text(), author->text());
        else{
            cerr << "Ungültige Eingabe " << endl;
            exit(-1);
        }

        //Medium zur Liste hinzufügen
        mediumList.append(temp);

        //Erneutes Laden der mediaList in die Tabelle
        LoadMedia();

        //Sichtbarkeit auf das View-Widget ändern
        SetViewMedium(true);
        SetAddMedium(false);
    });

    //Funktion hinzufügen, die beim rejected-Event der DialogButtonBox aufgerufen wird hinzufügen
    //->lediglich Sichtbarkeit auf View-Widget ändern
    QObject::connect(addMediumButtons, &QDialogButtonBox::rejected, [=](){

        //Sichtbarkeit auf das View-Widget ändern
        SetViewMedium(true);
        SetAddMedium(false);
    });
    LoadMedia();
}

void MainWindow::InitializeBorrowTab(){

//---------------------------------------------------------
//---Funktionalität zum "searchButtonBorrow" hinzufügen---
//---------------------------------------------------------

    //Pointer auf "searchButtonBorrow"-Button einrichten
    QPushButton *searchMedium = this->findChild<QPushButton*>("searchButtonBorrow");

    //Funktion hinzufügen, die beim click-Event des Buttons aufgerufen wird
    //->in der Tabelle sollen alle gesuchten Medien aufgelistet werden
    //->wenn ein Medium noch nicht ausgeliehen ist soll ein Button zum Ausleihen hinzugefügt werden
    QObject::connect(searchMedium, &QPushButton::clicked, [=](){

        //Pointer auf Tabelle und FormLayout einrichten
        QTableWidget *table = this->findChild<QTableWidget *>("tableWidgetBorrow");
        QLineEdit *title = this->findChild<QLineEdit*>("titelLineEditBorrow");
        QLineEdit *author = this->findChild<QLineEdit*>("authorLineEditBorrow");
        QComboBox *type = this->findChild<QComboBox*>("mediumComboBoxBorrow");
        QCheckBox *showBorrowed = this->findChild<QCheckBox*>("checkBoxBorrow");

        //Eingaben in Variablen speichern
        QString titleText = title->text();
        QString authorText = author->text();

        //Tabelle leeren
        table->setRowCount(0);
        
        //Variable zum zählen der hinzugefügten Zeilen
        int row=0;

        //Schleife, die durch mediaList interiert
        //->wenn ein Medium die Forderrungen erfüllt soll es in der Tabelle angezeigt werden
        for (int i = 0; i < mediumList.size(); i++) {

            //Überprüfung ob Medium Forderungen erfüllt, sonst wird zum nächstem Medium gesprungen
            //Forderrungen:
            //->Medium soll vom gefordertem Typ sein
            //->Titel soll mit dem Input soll mit Input beginnen (bei "" wird diese ignoriert)
            //->Autor soll mit dem Input soll mit Input beginnen (bei "" wird diese ignoriert)
            //->Wenn CheckBox auf false eingestellt ist darf Medium nicht ausgeliehen sein
            if (!mediumList[i]->getTitle().startsWith(titleText, Qt::CaseInsensitive)||
                !mediumList[i]->getAuthor().startsWith(authorText, Qt::CaseInsensitive)||
                !(mediumList[i]->getType()==type->currentText())||
                !(mediumList[i]->getBorrower()==nullptr || showBorrowed->checkState()))
                continue;

            //_____Dieser Code wird nur ausgeführt ist, wenn Medium Forderrungen erfüllt______

            //Einfügen einer neuen Zeile in die Tabelle
            table->insertRow(row);

            //In Spalte 1 soll der Typ des Mediums eingetragen werden
            QTableWidgetItem *typeB =new QTableWidgetItem(mediumList[i]->getType());
            table->setItem(row, 0, typeB);

            //In Spalte 2 soll der Titel des Mediums eingetragen werden
            QTableWidgetItem *titleB = new QTableWidgetItem(mediumList[i]->getTitle());
            table->setItem(row, 1, titleB);

            //In Spalte 3 soll der Autor des Mediums eingetragen werden
            QTableWidgetItem *author = new QTableWidgetItem(mediumList[i]->getAuthor());
            table->setItem(row, 2, author);

            //Wenn das Medium schon ausgeliehen ist soll in der 4 Spalte "Ausgliehen" stehen
            if(mediumList[i]->getBorrower()!=nullptr){
                QTableWidgetItem *borrowState =new QTableWidgetItem("Ausgeliehen");

                //Textfarbe auf rot ändern
                QColor redColor(Qt::red);
                borrowState->setForeground(redColor);

                //Text dick darstellen
                QFont font;
                font.setBold(true);
                borrowState->setFont(font);

                //Einfügen des Items in die 4 Spalte
                table->setItem(row, 3, borrowState);

                //zum nächstem Medium springen
                row++;
                continue;
            }

            //_____Dieser Code wird nur ausgeführt, wenn Medium nicht ausgeliehen ist______

            //Pointer auf Medium in currM speichern
            Medium* currM = mediumList[i];

            //Neuen Button mit Text "Ausleihen" erstellen
            QPushButton *borrowButton = new QPushButton("Ausleihen");
            
            //Funktion hinzufügen, die beim click-Event des Buttons aufgerufen wird
            //Funktion borrow wird aufgerufen
            //->ändert Sichtbarkeit auf Widget, in welchem Person, welche das Medium ausleihen will eingetragen wird
            QObject::connect(borrowButton, &QPushButton::clicked, [=](){
                borrow(currM);
            });

            //Button in Spalte 4 einfügen
            table->setCellWidget(row, 3, borrowButton);

            row++;
        }
    });
}

void MainWindow::InitializeReturnTab(){

//--------------------------------------------------------
//---Funktionalität zum "searchButtonReturn" hinzufügen---
//--------------------------------------------------------

    //Pointer auf "addButtonPerson"-Button einrichten
    QPushButton *searchButtonReturn = this->findChild<QPushButton*>("searchButtonReturn");

    //Funktion hinzufügen, die beim click-Event des Buttons aufgerufen wird
    //->Es sollen alles ausgliehenen Medien der Person aufgezählt werden
    QObject::connect(searchButtonReturn, &QPushButton::clicked, [=](){

        //Zugriff auf FromLayout-items
        QLineEdit* firstname = this->findChild<QLineEdit*>("firstnameLineEditReturn");
        QLineEdit* lastname = this->findChild<QLineEdit*>("nameLineEditReturn");
        QLabel* warning = this->findChild<QLabel*>("warningLabelReturn");

        //Person aus der Eingabe suchen
        int idx = searchPerson(firstname->text(), lastname->text());

        //Wenn diese nicht gefunden wurde warning ausgeben
        if(idx==-1){
            warning->setText("Person nicht gefunden");
            return;
        }

        //___Dieser Code wird nur ausgeführt wenn Person gefunden wurde___

        //Pointer auf Table-Widget einrichten
        QTableWidget *table = this->findChild<QTableWidget *>("tableWidgetReturn");

        //Tabelle leeren
        table->setRowCount(0);

        //Variable zum zählen der hinzugefügten Zeilen
        int row =0;

        //Schleife die durch mediaList iteriert
        //->wenn Person Ausleiher des Mediums ist wird dieser ausgegeben
        for(int i=0; i<mediumList.size(); i++){

            //Überprüfung ob Person Medium ausgeliehen hat
            if(mediumList[i]->getBorrower() != personList[idx])
                continue;
            
            //___Dieser Code wird nur ausgeführt wenn Person Medium ausgeliehen hat__
            
            //neue Zeile einfügen
            table->insertRow(row);

            //In Spalte 1 soll der Typ des Mediums eingetragen werden
            QTableWidgetItem *typeI =new QTableWidgetItem(mediumList[i]->getType());
            table->setItem(row, 0, typeI);

            //In Spalte 2 soll der Titel des Mediums eingetragen werden
            QTableWidgetItem *titleB = new QTableWidgetItem(mediumList[i]->getTitle());
            table->setItem(row, 1, titleB);

            //In Spalte 3 soll der Autor des Mediums eingetragen werden
            QTableWidgetItem *author = new QTableWidgetItem(mediumList[i]->getAuthor());
            table->setItem(row, 2, author);
            
            //Neuen Button mit Text "Rückgabe" erstellen
            QPushButton *returnButton = new QPushButton("Rückgabe");
            
            //Pointer auf Medium in currM speichern
            Medium* currM = mediumList[i];

            //Funktion hinzufügen, die beim click-Event des Buttons aufgerufen wird
            //->Rückgabe des Mediums -> borrower auf NULL
            QObject::connect(returnButton, &QPushButton::clicked, [=](){
                currM->setBorrower(NULL);

                //Tabelle neu laden
                LoadMedia();

                //Klick auf searchButton simulieren, dass Tabelle neu geladen wird
                searchButtonReturn->click();
            });

            //Einfügen des Buttons in vierte Spalte
            table->setCellWidget(row, 3, returnButton);
            row++;
        }
    });
}

void MainWindow::borrow(Medium* medium){

//-------------------------------------------------------------------------
//---Leeren des Widgets zum Ausleihen und Sichtbarkeit auf dieses ändern---
//-------------------------------------------------------------------------

    //Auf Formlayout-Items zugreifen
    QLineEdit * lastName = this->findChild<QLineEdit*>("nameLineEditBorrow");
    QLineEdit * firstName= this->findChild<QLineEdit*>("firstnameLineEditBorrow");
    QLabel *warning = this->findChild<QLabel*>("warningLabelBorrow");

    //FormLayout leeren
    lastName->setText("");
    firstName->setText("");
    warning->setText("");

    //Sichtbarkeit auf Borrow-Widget ändern
    SetAddBorrower(true);
    SetSearchMedium(false);

//-----------------------------------------------------
//---Funktionalität zur "buttonBoxBorrow" hinzufügen---
//-----------------------------------------------------

    //Pointer auf "QDialogButtonBox"-DialogButtonBox einrichten
    QDialogButtonBox *borrowBox = this->findChild<QDialogButtonBox *>("buttonBoxBorrow");

    //Funktion hinzufügen, die beim accepted-Event der DialogButtonBox aufgerufen wird
    //->wenn Person gefunden wird soll dem Medium die Person als Ausleiher hinzugefügt werden
    QObject::connect(borrowBox, &QDialogButtonBox::accepted, [=](){

        //Index der Person finden, wenn dieser -1 ist wurde die Person nicht gefunden
        int i = searchPerson(firstName->text(), lastName->text());

        //Person wurde nicht gefunden -> warning
        if(i==-1){
            warning->setText("Person nicht gefunden");
            return;
        }

        //Person wurde gefunden->hinzufügen dieser

        //Ausleiher hinzufügen
        medium->setBorrower(personList[i]);

        //Neu Laden der Medium-Tabelle
        LoadMedia();

        //Sichtbarkeit wieder auf Search-Widget ändern
        SetAddBorrower(false);
        SetSearchMedium(true);

        //Klick auf Suche simulieren, sodass Tabelle neu geladen wird
        QPushButton *searchMedium = this->findChild<QPushButton*>("searchButtonBorrow");
        searchMedium->click();

        //Funktion bei accepted wieder entfernen, sodass beim nächstem Funktion für ein anderes Medium eingefügt werden kann
        QObject::disconnect(borrowBox, &QDialogButtonBox::accepted, nullptr, nullptr);
    });

    //Funktion hinzufügen, die beim rejected-Event der DialogButtonBox aufgerufen wird
    //->Sichtbarkeit auf Search-Widget ändern
    QObject::connect(borrowBox, &QDialogButtonBox::rejected, [=](){

        //Sichtbarkeit wieder auf Search-Widget ändern
        SetAddBorrower(false);
        SetSearchMedium(true);

        //Funktion bei accepted wieder entfernen, sodass beim nächstem Funktion für ein anderes Medium eingefügt werden kann
        QObject::disconnect(borrowBox, &QDialogButtonBox::accepted, nullptr, nullptr);
    });
}

void MainWindow::InitializeUI(){
    
//--------------------------------------------
//---Personenverwaltungs-Tab initialisieren---
//--------------------------------------------

    InitializePersonTab();

//------------------------------------------
//---Mediumverwaltungs-Tab initialisieren---
//------------------------------------------

    InitializeMediaTab();
    
//---------------------------------
//---Ausleihe-Tab initialisieren---
//---------------------------------
    
    InitializeBorrowTab();

//---------------------------------
//---Rückgabe-Tab initialisieren---
//---------------------------------
    
    InitializeReturnTab();


    //Sichtbarkeit auf das View-Widget initialisieren
    SetViewMedium(true);
    SetAddMedium(false);
    SetAddBorrower(false);
    SetSearchMedium(true);
}

void MainWindow::LoadMediaFromFile(){
    QFile file(QApplication::applicationDirPath() + "/Media.txt");

    //Wenn die Datei nicht existiert wird eine neue erstellt
    if(!file.exists()){
        QSaveFile *file=new QSaveFile(QApplication::applicationDirPath() + "/Media.txt");
        file->open(QIODevice::WriteOnly | QIODevice::Text);
        file->commit();
        delete file;
    }

    //Lesen der Datei
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd())
            mediumList.append(Medium::parse(in.readLine(),&personList));
        file.close();
    }

    //Bei Fehler Programmbeendung
    else{
        cerr << "Fehler beim öffnen von Media.txt" <<endl;
        exit(-1);
    }
}

void MainWindow::LoadPersonFromFile(){
    QFile file(QApplication::applicationDirPath() + "/Person.txt");

    //Wenn die Datei nicht existiert wird eine neue erstellt
    if(!file.exists()){
        QSaveFile *file=new QSaveFile(QApplication::applicationDirPath() + "/Person.txt");
        file->open(QIODevice::WriteOnly | QIODevice::Text);
        file->commit();
        delete file;
    }
    
    //Lesen der Datei
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while(!in.atEnd())
            personList.append(Person::parse(in.readLine()));
        file.close();
    }
    
    //Bei Fehler Programmbeendung
    else{
        cerr << "Fehler beim öffnen von Media.txt" <<endl;
                exit(-1);
    }
}

void MainWindow::SaveMedia(){
    QFile file(QApplication::applicationDirPath() + "/Media.txt");

    //Laden von personList in die Datei
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (Medium* medium : mediumList) {
            out << medium->print();

            //Einfügen des Index der Person in personList
            //->beim Laden wieder personList in gleicher Reihenfolge -> Indizes stimmen überein 
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
    QFile file(QApplication::applicationDirPath() + "/Person.txt");
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

    //Wechsele Sicht auf Add-Widget
    SetViewMedium(false);
    SetAddMedium(true);

    //Pointer auf Widgetelemente und diese befüllen
    QComboBox* typeB = this->findChild<QComboBox *>("mediumComboBoxMedium");
    QLineEdit* titleB =this->findChild<QLineEdit *>("titleLineEditMedium");
    QLineEdit* authorB=this->findChild<QLineEdit *>("authorLineEditMedium");
    QLabel *warning = this->findChild<QLabel*>("warningMedium");
    typeB->setCurrentText( currM->getType());
    typeB->setDisabled(true); //Wechseln des Mediums nicht erlauben
    titleB->setText(currM->getTitle());
    authorB->setText(currM->getAuthor());

    //neue Funktion zu den Buttons hinzufügen
    QDialogButtonBox *addMediumButtons = this->findChild<QDialogButtonBox *>("mediumAddButtonBox");
    QObject::disconnect(addMediumButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
    QObject::disconnect(addMediumButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
    QObject::connect(addMediumButtons, &QDialogButtonBox::accepted, [=](){
        //Überprüfung ob Titel-Feld auch mit Text befüllt ist
        if(titleB->text().trimmed().isEmpty()){
            warning->setText("Titel muss befüllt sein");
            return;
        }

        //Überprüfung wenn ob die Eingabe ein Komma enthält
        //->sonst würde Fehler bei Dateiarbeit entstehen
        if(titleB->text().contains(',') || authorB->text().contains(',')){
            warning->setText("Es darf bei der Eingabe kein , benutzt werden");
            return;
        }

        //Medium ändern
        currM->setTitle(titleB->text());
        currM->setAuthor(authorB->text());

        //Ursprünglichen Stand wiederherstellen
        typeB->setDisabled(false);
        QObject::disconnect(addMediumButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(addMediumButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
        InitializeMediaTab();
    });
    QObject::connect(addMediumButtons, &QDialogButtonBox::rejected, [=](){
        
        //Ursprünglichen Stand wiederherstellen
        typeB->setDisabled(false);
        QObject::disconnect(addMediumButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(addMediumButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
        InitializeMediaTab();
    });
}

void MainWindow::EditPerson(Person* currP){

    //Sichtbarkeit auf Add-Medium ändern
    SetViewPerson(false);
    SetAddPerson(true);

    //Pointer auf Widgetelemente anlegen und diese befüllen
    QLineEdit* nameB =this->findChild<QLineEdit *>("nameLineEdit");
    QLineEdit* firstnameB=this->findChild<QLineEdit *>("firstnameLineEdit");
    QLabel *warning = this->findChild<QLabel*>("warningPerson");
    nameB->setText(currP->getName());
    firstnameB->setText(currP->getFirstname());
    
    //neue Funktion zu den Buttons hinzufügen
    QDialogButtonBox *addPersonButtons = this->findChild<QDialogButtonBox *>("personAddButtonBox");
    QObject::disconnect(addPersonButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
    QObject::disconnect(addPersonButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
    QObject::connect(addPersonButtons, &QDialogButtonBox::accepted, [=](){
        //Überprüfung ob Felder auch mit Text befüllt sind
        if((firstnameB->text().trimmed()).isEmpty() || (nameB->text().trimmed()).isEmpty()){
            warning->setText("Die Texte Name und Vorname müssen befüllt sein!");
            return;
        }
        
        //Überprüfung ob Person schon existiert
        //->searchPerson erzeugt Rückgabewert -1, wenn sich noch keine Person mit gleichem Vor- und Nachnamen im System befindet
        if(searchPerson(firstnameB->text(), nameB->text())!=-1){
            warning->setText(firstnameB->text()+" "+nameB->text()+" ist schon im System eingefügt wurden!");
            return;
        }

        //Überprüfung wenn ob die Eingabe ein Komma enthält
        //->sonst würde Fehler bei Dateiarbeit entstehen
        if(firstnameB->text().contains(',') || nameB->text().contains(',')){
            warning->setText("Es darf bei der Eingabe kein , benutzt werden");
            return;
        }   

        //Ändern der Person
        currP->setName(nameB->text());
        currP->setFirstname(firstnameB->text());

        //Ursprünglichen Stand wiederherstellen
        QObject::disconnect(addPersonButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(addPersonButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
        InitializeUI();
    });
    QObject::connect(addPersonButtons, &QDialogButtonBox::rejected, [=](){

        //Ursprünglichen Stand wiederherstellen
        QObject::disconnect(addPersonButtons, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(addPersonButtons, &QDialogButtonBox::rejected, nullptr, nullptr);
        InitializeUI();
    });
}

void MainWindow::LoadMedia(){

    //Pointer auf Mediatab herstellen
    QTableWidget *table = this->findChild<QTableWidget *>("tableWidgetMedium");
    table->setSortingEnabled(false);
    table->setRowCount(0); //Tabelle leeren

    //Jedes Medium in der Liste in Tabelle hinzufügen
    for(int row=0; row<mediumList.size(); ++row){
        table->insertRow(row);

        QTableWidgetItem *type =new QTableWidgetItem(mediumList[row]->getType());
        table->setItem(row, 0, type);

        QTableWidgetItem *title = new QTableWidgetItem(mediumList[row]->getTitle());
        table->setItem(row, 1, title);

        QTableWidgetItem *author = new QTableWidgetItem(mediumList[row]->getAuthor());
        table->setItem(row, 2, author);

        //Wenn ein Ausleiher exsitiert diesen einfügen
        QTableWidgetItem *borrower=NULL;
        if(mediumList[row]->getBorrower()==NULL)
            borrower=new QTableWidgetItem("");
        else
            borrower=new QTableWidgetItem(mediumList[row]->getBorrower()->getFirstname()+" "+mediumList[row]->getBorrower()->getName());
        table->setItem(row, 3, borrower);

        //Deletebutton hinzufügen
        QPushButton *deleteButton = new QPushButton("Löschen");
        table->setCellWidget(row, 4, deleteButton);

        Medium* currM = mediumList[row];
        QObject::connect(deleteButton, &QPushButton::clicked, [=](){
            int rowC = table->currentRow();
            table->removeRow(rowC);
            mediumList.removeAll(currM);
        });

        //Editbutton hinzufügen
        QPushButton *editButton = new QPushButton("Ändern");
        table->setCellWidget(row, 5, editButton);
        QObject::connect(editButton, &QPushButton::clicked, [=](){EditMedium(currM);});
    }
    table->setSortingEnabled(true);
}

void MainWindow::LoadPerson(){
    //Pointer auf Tablewidget
    QTableWidget *table = this->findChild<QTableWidget *>("tableWidgetPerson");

    table->setRowCount(0); //Tabelle leeren
    table->setSortingEnabled(false);

    //Jede Person in Tabelle einfügen
    for(int row=0; row<personList.size(); ++row){
        table->insertRow(row);

        QTableWidgetItem *firstname = new QTableWidgetItem(personList[row]->getFirstname());
        table->setItem(row, 0, firstname);

        QTableWidgetItem *name = new QTableWidgetItem(personList[row]->getName());
        table->setItem(row, 1, name);

        QPushButton *listButton = new QPushButton("Auflisten");
        table->setCellWidget(row, 2, listButton);

        //Button hinzufügen der ausgeliehene Medien der Person anzeigt
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

        //Deletebutton hinzufügen
        QPushButton *deleteButton = new QPushButton("Löschen");
        table->setCellWidget(row, 3, deleteButton);

        QObject::connect(deleteButton, &QPushButton::clicked, [=](){
            int rowC = table->currentRow();
            table->removeRow(rowC);
            personList.removeAll(currP);
        });

        //EditButton hinzufügen
        QPushButton *editButton = new QPushButton("Ändern");
        table->setCellWidget(row, 4, editButton);
        QObject::connect(editButton, &QPushButton::clicked, [=](){EditPerson(currP);});
    }
    table->setSortingEnabled(true);
}

int MainWindow::searchPerson(QString firstname, QString name){
    for(int i=0; i<personList.size(); i++){
        if(personList[i]->getFirstname().compare(firstname)==0 &&
            personList[i]->getName().compare(name)==0)
            return i; //Rückgabe des Index der Person
    }
    return -1; //Wenn Person nicht existiert dann Rückgabe -1
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
