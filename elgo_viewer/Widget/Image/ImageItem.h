#ifndef IMAGEITEM_H
#define IMAGEITEM_H

// QT
#include <QGraphicsPixmapItem>

// Viewer
#include "Widget/Definition/ContentsDef.h"

class ImageItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit ImageItem(QGraphicsItem *parent = nullptr);
    virtual ~ImageItem();

    /** @brief */
    bool SetImageItem(const QString& filePath, const StyleSheet::WidgetInfo& widgetInfo);

private:
    StyleSheet::WidgetInfo m_widgetInfo;
};

#endif // IMAGEITEM_H
