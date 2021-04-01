
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
bool ImageItem::SetImageItem(const QString& filePath, const StyleSheet::WidgetInfo& widgetInfo)
//========================================================
{
    bool retValue = true;

    m_widgetInfo = widgetInfo;
    QPixmap originPximap;
    const bool bIsLoad = originPximap.load(filePath);
    if(true == bIsLoad)
    {
        QPixmap scaledPximap = originPximap.scaled(m_widgetInfo.size, Qt::IgnoreAspectRatio);

        this->setPos(m_widgetInfo.pos);
        this->setPixmap(scaledPximap);
    }
    else
    {
        retValue = false;
        ELGO_VIEWER_LOG("Error - Not loaded image :%s", filePath.toUtf8().constData());
    }

    return retValue;
}
