#include "chardata.h"
#include <QDebug>

QJsonObject AnimalData::metaData = readJson(":/res/MetaData_Animals.Arx.json");

QVector<AnimalData> AnimalData::loadAnimalFile(const QString &filename)
{
    QVector<AnimalData> animals;
    QJsonObject fileObject=readJson(filename);
    if (!fileObject["Tiere"].isArray())
    {
        qDebug() << "No Array under \"Tiere\" - Key ";
        return animals;
    }
    QJsonArray animalArray = fileObject["Tiere"].toArray();
    int iIndex=0;
    for (QJsonArray::const_iterator it=animalArray.begin();it!=animalArray.end();it++)
    {
        if (!((*it).isObject()))
        {
            qDebug() << "Object #" << iIndex << "is not a JsonObject!";
            return animals;
        }
        iIndex++;
        animals.append(AnimalData((*it).toObject()));
    }
    return animals;
}

bool AnimalData::writeAnimalFile(const QVector<AnimalData> &animalData, const QString &filename)
{
    QJsonObject fileObject;
    QJsonArray animalArray;
    foreach (const AnimalData &animal,animalData)
    {
        animalArray.append(animal);
    }
    fileObject.insert("Tiere",animalArray);
    return writeJson(filename,fileObject);
}


AnimalData::AnimalData():QJsonObject(){
    QJsonObject attributes;
    QJsonArray attrNames = metaData["Attribute"].toArray();
    for (QJsonArray::const_iterator it=attrNames.begin();it!=attrNames.end();it++)
    {
        attributes.insert((*it).toString(),0);
    }
    insert("Attribute",attributes);

    QJsonObject additionalData;
    QJsonArray additionalDataMeta = metaData["Weitere Werte"].toArray();
    for (QJsonArray::const_iterator it=additionalDataMeta.begin();it!=additionalDataMeta.end();it++)
    {
        additionalData.insert((*it).toString(),0);
    }
    insert("Weitere Werte",additionalData);

    QJsonArray specials;
    insert("Besondere Fähigkeiten",specials);

    QJsonObject skills;
    QJsonArray skillNames = metaData["Fertigkeiten"].toArray();
    for (QJsonArray::const_iterator it=skillNames.begin();it!=skillNames.end();it++)
    {
        QJsonObject skill;
        skill.insert("Stock",0);
        skill.insert("FP",0);
        skills.insert((*it).toString(),skill);
    }
    insert("Fertigkeiten",skills);

    QJsonArray weapons;
    insert("Kampfwerte",weapons);
}
AnimalData::AnimalData(const QJsonObject &obj):QJsonObject(obj){}
AnimalData::AnimalData(const AnimalData &anim):QJsonObject(anim){}


void AnimalData::createTexFileFromAnimalDataArray(QVector<AnimalData> &animalData,const QString &filename)
{
    QString texBody;
    int iCurrentMinipage=0;
    foreach(const AnimalData &data , animalData)
    {
        if (iCurrentMinipage>0){
            if (iCurrentMinipage%2==1)
            {
                texBody.append(" \\hspace{0.2cm}\n");
            }else if (iCurrentMinipage%4==0)
            {
                texBody.append("\n\n\\pagebreak\n");
            }else{
                texBody.append("\n\n\\vspace{0.5cm}\n");
            }
        }
        texBody.append(Tex::quarterPageMiniSetup(data.toTexMinipageString()));
        iCurrentMinipage++;
    }

    QString finalTexFile=Tex::embedIntoDocument(texBody);


    QFile outFile(filename);
    if ( !outFile.open(QIODevice::WriteOnly) )
    {
        qDebug() << "Error writing!\n";
        return;
    }

    outFile.write(finalTexFile.toUtf8());
}


QString AnimalData::toTexMinipageString() const
{
    QString str;
    str.append(QString("{\\hspace{1cm}\\large\\textbf{%1}}\\flushright\n\n").arg(obj()["Name"].toString()));
    str.append("\\begin{minipage}{0.47\\textwidth}\\centering\n\\renewcommand{\\arraystretch}{0.9}\n\\begin{tabular}{ll}\n");
    QJsonArray attrNames = metaData["Attribute"].toArray();
    for (QJsonArray::const_iterator it=attrNames.begin();it!=attrNames.end();it++)
    {
        QString attrName=it->toString();
        str.append(Tex::animalAttrib(attrName,obj("Attribute")[attrName].toInt()));
    }
    str.append("&\\\\[-1ex]");
    str.append(Tex::animalAttrib("Lebensblock",obj("Weitere Werte")["Lebensblock"].toInt()));
    str.append(Tex::animalAttrib("Ausdauerblock",obj("Weitere Werte")["Ausdauerblock"].toInt()));
    str.append(Tex::animalAttrib("Manablock",obj("Weitere Werte")["Manablock"].toInt()));
    str.append("&\\\\[-1ex]");
    str.append(Tex::animalAttrib("Vorkommen",obj("Weitere Werte")["Vorkommen"].toInt()));
    str.append("\\end{tabular}\\end{minipage}\n");

    str.append("\\begin{minipage}{0.49\\textwidth}\\flushright\n\\begin{tabular}{|l|}\\hline\n"
               "\\\\[-2.2ex]\n"
               "\\multicolumn{1}{|c|}{\\textbf{Besondere Fähigkeiten}}\\\\\\hline\n");
    QJsonArray specials = obj()["Besondere Fähigkeiten"].toArray();
    for (int i=0;i<11;i++)
    {
        if (i<specials.size())
            str.append(QString("%1\\\\\n").arg(specials[i].toString()));
        else
            str.append("\\\\\n");
    }
    str.append("\\hline\\end{tabular}\\end{minipage}\n\n\\vspace{0.1cm}");

    str.append("\\begin{minipage}{0.39\\textwidth}\\flushright\\begin{tabular}{|l|r@{\\em}c@{\\em}l|}\\hline\n");
    str.append("\\multicolumn{4}{|c|}{ }\\\\[-2.2ex]\n");
    str.append("\\multicolumn{4}{|c|}{\\textbf{Weitere Werte}}\\\\\\hline\n");
    str.append("&\\phantom{999}&\\phantom{/}&\\phantom{999}\\\\[-2.5ex]\n");
    str.append(Tex::animalSpecial("LL",obj("Weitere Werte")["Laufleistung einfach"].toInt(),obj("Weitere Werte")["Laufleistung voll"].toInt()));
    str.append(Tex::animalSpecial(QString("SL"),obj("Weitere Werte")["Schwimmleistung einfach"].toInt(),obj("Weitere Werte")["Schwimmleistung voll"].toInt()));
    str.append(Tex::animalSpecial(QString("FL"),obj("Weitere Werte")["Flugleistung einfach"].toInt(),obj("Weitere Werte")["Flugleistung voll"].toInt()));
    str.append(Tex::animalSpecial(QString("TK"),obj("Weitere Werte")["Tragkraft einfach"].toInt(),obj("Weitere Werte")["Tragkraft voll"].toInt()));
    str.append(Tex::animalSpecial(QString("SK/ZG"),obj("Weitere Werte")["Stemmkraft"].toInt(),obj("Weitere Werte")["Zugkraft"].toInt()));
    str.append(QString("GF:&\\multicolumn{3}{c|}{%1}\\\\\\hline\n").arg(obj("Weitere Werte")["Geistige Festigkeit"].toInt()));
    int iMagRes=obj("Weitere Werte")["Magieresistenz"].toInt();
    str.append(QString("Mres:&\\multicolumn{3}{c|}{%1}\\\\\\hline\n").arg(  (iMagRes==0?QString("-"):QString("%1\\\%").arg(QString::number(iMagRes)))  ));
    int iMagRen=obj("Weitere Werte")["Magierenitenz"].toInt();
    str.append(QString("Mren:&\\multicolumn{3}{c|}{%1}\\\\\\hline\n").arg( (iMagRen==0?QString("-"):QString::number(iMagRen))  ));
    str.append("&&&\\\\[-2.5ex]\\hline\n&&&\\\\[-2.5ex]\n");
    str.append(Tex::animalSpecial(QString("RsN/RsM"),obj("Weitere Werte")["Rüstungsschutz natürlich"].toInt(),obj("Weitere Werte")["Rüstungsschutz magisch"].toInt()));
    str.append("&&&\\\\[-2.5ex]\n");
    str.append(Tex::animalSpecial(QString("AvR/BM"),obj("Weitere Werte")["Ausdauerverbrauch Rüstung"].toInt(),obj("Weitere Werte")["Bewegungsmalus"].toInt()));
    str.append("\\end{tabular}\\end{minipage}\n");

    str.append("\\begin{minipage}{0.55\\textwidth}\\flushright\\begin{tabular}{|l@{\\hskip 0.5em}r@{\\em}c@{\\em}l|}\\hline\n");
    str.append("&&&\\\\[-2.2ex]\n");
    str.append("\\multicolumn{4}{|c|}{\\textbf{Fertigkeiten}}\\\\\\hline\n&&&\\\\[-2.5ex]");

    QJsonObject skills = obj("Fertigkeiten");
    int iFillerLines=0;
    for (QJsonObject::iterator it=skills.begin();it!=skills.end();it++)
    {
        QJsonObject skillObj=it.value().toObject();
        if (skillObj["Stock"].toInt()>0){
            str.append("\n");
            str.append(Tex::animalSkills(it.key(),skillObj["Stock"].toInt(),skillObj["FP"].toInt()));
        }
        else
            iFillerLines++;
    }
    for (int i=0;i<iFillerLines;i++)
        str.append("\n&&&\\\\");


    str.append("[-2.8ex]\n\\phantom{Wahrnehmung}&\\phantom{99}&\\phantom{+}&\\phantom{\\scriptsize W12+W6+W4}\\\\\\hline\\end{tabular}\\end{minipage}");
    str.append("\n\n\\vspace{0.1cm}\n\n");

    QJsonArray weaponsArray = obj()["Kampfwerte"].toArray();
    QString wepTabularCreation, wepName, wepAngsAW, wepIni,webAvAngAvR,webDmg;
    for (QJsonArray::iterator it = weaponsArray.begin(); it!=weaponsArray.end(); it++)
    {
        QJsonObject currObj=(*it).toObject();
        wepTabularCreation.append("c|");
        wepName.append(QString("&\\textbf{%1}").arg(currObj["Name"].toString()));
        wepAngsAW.append(QString("&%1+%2").arg(currObj["Angriffsstock"].toInt()).arg(Tex::skill2Dice(currObj["Angriffsfertigkeitspunkte"].toInt())));
        wepIni.append(QString("&%1+{\\scriptsize %2}").arg(currObj["Initiative Stock"].toInt()).arg(currObj["Initiative Würfel"].toString()));
        webAvAngAvR.append(QString("&%1+%2").arg(currObj["Ausdauerverbrauch Angriff"].toInt()).arg(obj("Weitere Werte")["Ausdauerverbrauch Rüstung"].toInt()));
        int iSchaden=currObj["Schaden Stock"].toInt();
        QString damageDice=currObj["Schaden Würfel"].toString();
        if (damageDice.isEmpty())
            webDmg.append(QString("&%1").arg(iSchaden));
        else{
            if (iSchaden==0)
            {
                webDmg.append(QString("&{\\scriptsize %1}").arg(damageDice));
            }else{
                QString qstrSchadensStockVorzeichen("$+$");
                if (currObj["Schaden Stock"].toInt()<0)
                    qstrSchadensStockVorzeichen=QString("$-$");
                webDmg.append(QString("&{\\scriptsize %1}%2%3").arg(damageDice).arg(qstrSchadensStockVorzeichen).arg(qAbs(iSchaden)));
            }

        }
        //qDebug() << obj()["Name"].toString() << wepName << webDmg;
    }

    str.append(QString("\\begin{minipage}{0.95\\textwidth}\\begin{tabular}{|l|%1}\\hline\n").arg(wepTabularCreation));
    str.append(QString("\\textbf{Kampfwerte}%1\\\\\\hline\n").arg(wepName));
    str.append(QString("AngS.+AW:%1\\\\\\hline\n").arg(wepAngsAW));
    str.append(QString("Ini:%1\\\\\\hline\n").arg(wepIni));
    str.append(QString("AvAng+AvR:%1\\\\\\hline\n").arg(webAvAngAvR));
    str.append(QString("Schaden:%1\\\\\\hline\n").arg(webDmg));
    str.append("\\end{tabular}\\end{minipage}\n");




    return str;
}


