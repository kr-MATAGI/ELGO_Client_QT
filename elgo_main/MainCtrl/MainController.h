#pragma once
#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "MainCtrl.h"
#include "DB/MainDBCtrl.h"

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

private:
    MainCtrl *m_mainCtrl;
    MainDBCtrl *m_dbCtrl;
};

#endif // MAINCONTROLLER_H
