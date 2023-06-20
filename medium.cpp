#include "medium.h"
#include <QString>
#include <QStringList>
#include <QList>
#include <iostream>
#include "book.h"
#include "CD.h"
#include "DVD.h"
#include "person.h"

using namespace std;

Medium::Medium(QString title, QString author){
    this->title = title;
    this->author = author;
    this->borrower=NULL;
    return;
}

Medium::Medium(QString title, QString author, int borrower, QList<Person*>* personList){
    this->title = title;
    this->author = author;
    this->borrower =personList->at(borrower);
    return;
}

Medium* Medium::parse(QString Line, QList<Person*>* personList){
    QStringList parts = Line.split(",");
    if(parts[3]==""){
        switch(parts[0].toInt()){
        case 1:return new Book(parts[1], parts[2]);
        case 2:return new CD(parts[1], parts[2]);
        case 3:return new DVD(parts[1], parts[2]);
        default: cerr << "In der zu lesenden Datei befinden sich falsche Daten" << endl; exit(-1);
        }
    }
    else{
        switch(parts[0].toInt()){
        case 1:return new Book(parts[1], parts[2], parts[3].toInt(), personList);
        case 2:return new CD(parts[1], parts[2], parts[3].toInt(), personList);
        case 3:return new DVD(parts[1], parts[2], parts[3].toInt(), personList);
        default: cerr << "In der zu lesenden Datei befinden sich falsche Daten" << endl; exit(-1);
        }
    }
}
