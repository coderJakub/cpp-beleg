#include "person.h"
#include <QString>
#include <QStringList>

Person::Person(QString firstname, QString name){
    this->firstname=firstname;
    this->name=name;
}

Person* Person::parse(QString Line){
    QStringList parts = Line.split(",");
    return new Person(parts[0], parts[1]);
}


QString Person::print(){
    return getFirstname() +","+getName();
}
