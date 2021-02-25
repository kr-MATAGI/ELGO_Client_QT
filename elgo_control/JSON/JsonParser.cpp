
#include "JsonParser.h"

//QT
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

//========================================================
QString JsonParser::RemoteVersionParse(const QString &src)
//========================================================
{
    QJsonDocument doc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = doc.object();
    QString version = jsonObj["version"].toString();
    qDebug() << "Remote Version : " << version;

    return version;
}
