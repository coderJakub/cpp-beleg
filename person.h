#ifndef PERSON_H
#define PERSON_H
#include <QString>


class Person{
private:
    QString name;
    QString firstname;

public:
    Person(QString firstname, QString name);
    ~Person();
    void setName(QString name){this->name=name;}
    void setFirstname(QString firstname){this->firstname=firstname;}
    QString getName(){return name;}
    QString getFirstname(){return firstname;}
    static Person* parse(QString Line);
    QString print();
};

#endif // PERSON_H
