#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

// EFC
#include "ShardMem/ShmCtrl.h"

// Main
#include "MainCtrl.h"
#include "DB/MainDBCtrl.h"

class MainCtrl;
class MainDBCtrl;
class ShmCtrl;

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
    ShmCtrl& GetShmCtrl();

private:
    MainCtrl *m_mainCtrl;
    MainDBCtrl *m_dbCtrl;
    ShmCtrl *m_shmCtrl;
};

#endif // MAINCONTROLLER_H
