#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H


// Main
#include "MainCtrl.h"
#include "DB/MainDBCtrl.h"
#include "Timer/ScheduleTimer.h"

class MainCtrl;
class MainDBCtrl;
class ScheduleTimer;

class MainController
{
private:
    static MainController *pInstance;

    MainController();
    ~MainController();

public:
    /** @brief */
    static MainController* GetInstance();

    /** @brief */
    void DestoryInstance();

    /** @brief */
    MainCtrl& GetMainCtrl();

    /** @brief */
    MainDBCtrl& GetDBCtrl();

    /** @brief */
    ScheduleTimer& GetScheduleTimer();

private:
    MainCtrl *m_mainCtrl;
    MainDBCtrl *m_dbCtrl;

    ScheduleTimer *m_scheduleTimer;
};

#endif // MAINCONTROLLER_H
