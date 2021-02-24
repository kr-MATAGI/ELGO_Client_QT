#ifndef QRMAKER_H
#define QRMAKER_H

#include <QPainter>
#include <QQuickPaintedItem>
#include "ext_libs/QRCode/QrCode.h"


class QrMaker : public QQuickPaintedItem
{
    Q_OBJECT

public :
    explicit QrMaker(QQuickItem *parent = nullptr);
    virtual ~QrMaker();

public:
    /** @brief */
    void DrawQrCode(QPainter &painter, const QSize size, const QString &url, QColor color);
    /** @brief */
    void paint(QPainter *painter) override;
};
#endif // QRMAKER_H
