// control
#include "TCPHandler.h"

//========================================================
TCPHandler::TCPHandler(QObject *parent)
//========================================================
{
    m_manager = new QNetworkAccessManager(this);
}

//========================================================
TCPHandler::~TCPHandler()
//========================================================
{
    delete m_manager;
    m_manager = NULL;
}
