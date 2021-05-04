#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H


// Main
#include "MainCtrl.h"
#include "DB/MainDBCtrl.h"

#include "Timer/PlayScheduleTimer.h"
#include "Timer/PowerScheduleTimer.h"

class MainCtrl;
class MainDBCtrl;

class PlayScheduleTimer;
class PowerScheduleTimer;

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
    PlayScheduleTimer& GetPlayTimer();

    /** @brief */
    PowerScheduleTimer& GetPowerTimer();

private:
    MainCtrl *m_mainCtrl;
    MainDBCtrl *m_dbCtrl;

    PlayScheduleTimer *m_playTimer;
    PowerScheduleTimer *m_powerTimer;
};

#endif // MAINCONTROLLER_H
