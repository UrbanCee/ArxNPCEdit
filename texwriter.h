#ifndef TEXWRITER_H
#define TEXWRITER_H

#include <QtCore>

class Tex{
public:
    static QString embedIntoDocument(const QString &);
    static QString quarterPageMiniSetup(const QString &);
    static QString animalAttrib(const QString &,int);
    static QString animalSpecial(const QString &,int,int);
    static QString animalSkills(const QString &,int,int);
    static QString skill2Dice(int);
};



#endif // TEXWRITER_H
