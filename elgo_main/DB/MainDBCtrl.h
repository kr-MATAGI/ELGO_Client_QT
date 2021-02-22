#ifndef MAINDBCTRL_H
#define MAINDBCTRL_H

#include <QObject>
#include <QtSql>
#include <QMutex>

#include "DBDefinition.h"

class MainDBCtrl : public QObject
{
    Q_OBJECT

public:
    explicit MainDBCtrl(QObject *parent = nullptr);
    virtual ~MainDBCtrl();

public:
    /** brief */
    void ConnectionDB();

    /** brief */
    void CreateDeviceDBTable();
    /** brief */
    void CreateAccountDBTable();

    /** brief */
    void GetAllInfoFromDeviceTable();
    /** brief */
    void GetAllInfoFromAccountTable();

private:
    QMutex *m_mutex;

};

#endif // MAINDBCTRL_H
