#ifndef DVD_H
#define DVD_H
#include <QString>
#include "medium.h"

class DVD : public Medium{
public:
    DVD(QString title, QString author):Medium(title, author){}
    DVD(QString title, QString author, int borrower, QList<Person*>* personList):Medium(title,author,borrower,personList){}
    QString print(){
        return "3,"+getTitle()+","+getAuthor();
    }
    QString getType(){return "DVD";}
};

#endif // DVD_H
