#ifndef MAINDBCTRL_H
#define MAINDBCTRL_H

// QT
#include <QObject>
#include <QtSql>
#include <QMutex>

// Common
#include "Common/DBDefinition.h"
#include "Common/Interface/ScheduleJsonDef.h"

class MainDBCtrl : public QObject
{
    Q_OBJECT

public:
    explicit MainDBCtrl(QObject *parent = nullptr);
    virtual ~MainDBCtrl();

public:
    /** @brief */
    void InitializeDB();

    /** @brief */
    void MakeDefulatTable(QSqlDatabase& db, const char* dbPath,
                          const QString& tableName);

    // playSchedule
    /** @brief */
    void UpdateNewPlaySchedule(const QVector<ScheduleJson::PlaySchedule>& playScheduleList);

    /** @brief */
    void AddNewPlayDataToDB(const PlayJson::CustomPlayDataJson& playData);
    /** @brief */
    void AddNewPlayDataToDB(const PlayJson::FixedPlayDataJson& playData);

    /** @brief */
    void GetPlayDataFromDB(const int id, const PlayJson::PlayDataType type,
                           PlayJson::CustomPlayDataJson& dest);
    /** @brief */
    void GetPlayDataFromDB(const int id, const PlayJson::PlayDataType type,
                           PlayJson::FixedPlayDataJson& dest);

    /** @brief */
    void ClearAllPlaySchedule();
    /** @brief */
    void DeletePlayScheduleById(const QString& scheduleId);

    /** @brief */
    bool CheckDuplicatedId(const QVector<QString>& dbIdList, const QString& id);

private:
    QMutex *m_mutex;

};

#endif // MAINDBCTRL_H
