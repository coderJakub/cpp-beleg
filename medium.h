#ifndef MEDIUM_H
#define MEDIUM_H
#include <QString>
#include <QUuid>
#include "person.h"

class Medium{
private:
    QString title;
    QString author;
    Person* borrower;

public:
    Medium(QString title, QString author);
    ~Medium();
    QString getTitle(){return title;}
    QString getAuthor(){return author;}
    Person* getBorrower(){return borrower;}
    void setTitle(QString title){this->title=title;}
    void setAuthor(QString author){this->author=author;}
    void setBorrower(Person* borrower){this->borrower=borrower;}
    virtual QString getType()=0;
    virtual QString print()=0;
    static Medium* parse(QString Line);
};


#endif // MEDIUM_H
