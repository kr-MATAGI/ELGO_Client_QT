#ifndef QRMAKER_H
#define QRMAKER_H

// QT
#include <QObject>
#include <QPainter>

// ext_libs
#include "ext_libs/QrCode/QrCode.h"


class QrMaker
{

public :
    explicit QrMaker();
    virtual ~QrMaker();

public:
    /** @brief */
    void DrawQrCode(QPainter &painter, const QSize size, const QString &url, QColor color);
};
#endif // QRMAKER_H
