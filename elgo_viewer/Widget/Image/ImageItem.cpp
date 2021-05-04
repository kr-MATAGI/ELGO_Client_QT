
// QT
#include <QPixmap>

// Viewer
#include "ImageItem.h"
#include "Logger/ViewerLogger.h"

//========================================================
ImageItem::ImageItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
//========================================================
{

}

//========================================================
ImageItem::~ImageItem()
//========================================================
{

}

//========================================================
bool ImageItem::SetImageItem(const QString& filePath,  const StyleSheet::PosSizeInfo& posSizeInfo)
//========================================================
{
    bool retValue = true;

    QStringList pathSplit = filePath.split("/");
    m_fileName = pathSplit.back();

    m_posSizeInfo = posSizeInfo;
    QPixmap originPximap;
    const bool bIsLoad = originPximap.load(filePath);
    if(true == bIsLoad)
    {
        QPixmap scaledPximap = originPximap.scaled(m_posSizeInfo.size, Qt::IgnoreAspectRatio);

        this->setPos(m_posSizeInfo.pos);
        this->setPixmap(scaledPximap);
        ELGO_VIEWER_LOG("Set Image  : %s", filePath.toUtf8().constData());
        ELGO_VIEWER_LOG("Image Pos - {x: %d, y: %d, w: %d, h: %d}",
                        m_posSizeInfo.pos.x(), m_posSizeInfo.pos.y(),
                        m_posSizeInfo.size.width(), m_posSizeInfo.size.height());
    }
    else
    {
        retValue = false;
        ELGO_VIEWER_LOG("Error - Not loaded image :%s", filePath.toUtf8().constData());
    }

    return retValue;
}

//========================================================
QString ImageItem::GetImageFileName()
//========================================================
{
    return m_fileName;
}
