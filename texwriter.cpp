#include "texwriter.h"
#include <QDebug>

QString Tex::embedIntoDocument(const QString &filler)
{
    QString texFile;
    QFile f(":/res/Latex_Header.tex");
    if (!f.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Error reading Latex Header File from Res!";
        return QString();
    }
    QTextStream in(&f);
    return QString("%1%2\n\\end{document}").arg(in.readAll()).arg(filler);
}

QString Tex::quarterPageMiniSetup(const QString &str)
{
    return QString("\\fbox{\\begin{minipage}[t][0.48\\textheight][c]{0.48\\textwidth}\n%1\n\\end{minipage}}").arg(str);
}

QString Tex::animalAttrib(const QString &attribName, int iValue)
{
    if (iValue==0)
        return QString("\\textbf{%1:}&-\\\\\n").arg(attribName);
    else
        return QString("\\textbf{%1:}&%2\\\\\n").arg(attribName).arg(iValue);
}

QString Tex::animalSpecial(const QString &name, int val1, int val2)
{
    return QString("%1:&%2&/&%3\\\\\\hline\n").arg(name).arg((val1==0?QString("-"):QString::number(val1))).arg((val2==0?QString("-"):QString::number(val2)));
}

QString Tex::animalSkills(const QString &name, int stock, int fp)
{
    return QString("%1&%2&+&%3\\\\").arg(name).arg(stock).arg(skill2Dice(fp));
}


QString Tex::skill2Dice(int FP)
{
    switch(FP){
    case 1:
        return QString("{\\scriptsize 2W4}");
    case 2:
        return QString("{\\scriptsize W6+w4}");
    case 3:
        return QString("{\\scriptsize 2W6}");
    case 4:
        return QString("{\\scriptsize W6+W8}");
    case 5:
        return QString("{\\scriptsize 2W8}");
    case 6:
        return QString("{\\scriptsize W10+2W4}");
    case 7:
        return QString("{\\scriptsize W10+W4+W6}");
    case 8:
        return QString("{\\scriptsize W10+2W6}");
    case 9:
        return QString("{\\scriptsize W12+2W6}");
    case 10:
        return QString("{\\scriptsize W12+2W6+W4}");
    default:
        return QString("{\\scriptsize W6}");
    }
}
