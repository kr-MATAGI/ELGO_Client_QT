
#include "JsonParser.h"

//QT
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

// Control
#include "Logger/ControlLogger.h"

//========================================================
QString JsonParser::RemoteVersionParse(const QString &src)
//========================================================
{
    QJsonDocument doc = QJsonDocument::fromJson(src.toUtf8());
    QJsonObject jsonObj = doc.object();
    QString version = jsonObj["version"].toString();
    ELGO_CONTROL_LOG("Remote Version : %s", version.toUtf8().constData());

    return version;
}
