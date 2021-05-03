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
    void CheckingDefaultTables(const char* dbPath);

    // playSchedule
    /** @brief */
    void UpdateNewPlaySchedule(const QVector<ScheduleJson::PlaySchedule>& playScheduleList);

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
