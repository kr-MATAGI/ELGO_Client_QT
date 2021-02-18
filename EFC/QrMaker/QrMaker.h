#ifndef QRMAKER_H
#define QRMAKER_H

#endif // QRMAKER_H

#include <QPainter>

#include "ext_libs/QrCode/QrCode.h"

namespace QrMaker
{
    void DrawQrCode(QPainter &painter, const QSize size, const QString &url, QColor color);
};
