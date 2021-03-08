#ifndef MAINDBCTRL_H
#define MAINDBCTRL_H

// QT
#include <QObject>
#include <QtSql>
#include <QMutex>

#include "Common/DBDefinition.h"

class MainDBCtrl : public QObject
{
    Q_OBJECT

public:
    explicit MainDBCtrl(QObject *parent = nullptr);
    virtual ~MainDBCtrl();

public:
    /** @brief */
    void ConnectionDB();

    /** @brief */
    void CheckingDefaultTables();

private:
    QMutex *m_mutex;

};

#endif // MAINDBCTRL_H
