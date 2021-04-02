#ifndef CONTENTSPLAYER_H
#define CONTENTSPLAYER_H

// QT
#include <QWidget>

// Viewer
#include "Definition/ContentsDef.h"
#include "ViewerCtrl/ViewerController.h"

#include "Widget/Video/VideoItem.h"

class ViewerController;

namespace Ui {
class ContentsPlayer;
}

class ContentsPlayer : public QWidget
{
    Q_OBJECT

public:
    /** @brief */
    static ContentsPlayer *pInstance;

    /** @brief */
    explicit ContentsPlayer(QWidget *parent = nullptr);
    /** @brief */
    ~ContentsPlayer();

    /** @brief */
    static ContentsPlayer* GetInstance();
    /** @brief */
    void DestoryInstance();

public:
    /** @brief */
    bool GetCurrentWidgetCapture();

    /** @brief */
    void UpdatePlayerScene(QGraphicsScene& scene);

    /** @brief */
    void StartContentsPlayer();

private:
    Ui::ContentsPlayer *ui;
    QSize m_displaySize;
};

#endif // CONTENTSPLAYER_H
