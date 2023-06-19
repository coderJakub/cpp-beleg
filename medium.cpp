#include "medium.h"
#include <QString>
#include <QStringList>
#include <iostream>
#include "book.h"
#include "CD.h"
#include "DVD.h"

using namespace std;

Medium::Medium(QString title, QString author){
    this->title = title;
    this->author = author;
    this->borrower = NULL;
    return;
}

Medium* Medium::parse(QString Line){
    QStringList parts = Line.split(",");
    if(parts[0].toInt() == 1)return new Book(parts[1], parts[2]);
    else if(parts[0].toInt() == 2)return new CD(parts[1], parts[2]);
    else if(parts[0].toInt() == 3)return new DVD(parts[1], parts[2]);
    else{
        cerr << "In der zu lesenden Datei befinden sich falsche Daten" << endl;
        exit(-1);
    }
    return NULL;
}
