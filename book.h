#ifndef BOOK_H
#define BOOK_H
#include <QString>
#include "medium.h"

class Book : public Medium{
public:
    Book(QString title, QString author):Medium(title, author){}
    Book(QString title, QString author, int borrower, QList<Person*>* personList):Medium(title,author,borrower,personList){}
    QString print(){
        return "1,"+getTitle()+","+getAuthor();
    }
    QString getType(){return "Buch";}
};

#endif //BOOK_H
