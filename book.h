#ifndef BOOK_H
#define BOOK_H
#include <QString>
#include "medium.h"

class Book : public Medium{
public:
    Book(QString title, QString author):Medium(title, author){}
    QString print(){
        return "1,"+getTitle()+","+getAuthor();
    }
    QString getType(){return "Buch";}
};

#endif //BOOK_H
