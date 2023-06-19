#ifndef CD_H
#define CD_H
#include <QString>
#include "medium.h"

class CD : public Medium{
public:
    CD(QString title, QString author):Medium(title,author){}
    QString print(){
        return "2," + getTitle() + "," + getAuthor();
    }
    QString getType(){return "CD";}
};

#endif // CD_H
