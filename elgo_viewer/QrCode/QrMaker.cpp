// EFC
#include "Common/Deifinition.h"

// Viewer
#include "Logger/ViewerLogger.h"
#include "QrMaker.h"

//========================================================
QrMaker::QrMaker()
//========================================================
{

}

//========================================================
QrMaker::~QrMaker()
//========================================================
{

}

//========================================================
void QrMaker::DrawQrCode(QPainter &painter, const QSize sz, const QString &url, QColor color)
//========================================================
{
    ELGO_VIEWER_LOG("url - {%s}, Size - {width : %d, height : %d}", url.toUtf8().constData(), sz.width(), sz.height());

    // NOTE: At this point you will use the API to get the encoding and format you want, instead of my hardcoded stuff:
    char *str = url.toUtf8().data();
    ::qrcodegen::QrCode qrCode = ::qrcodegen::QrCode::encodeText(str, ::qrcodegen::QrCode::Ecc::HIGH);
    const int qrSize = qrCode.getSize() > 0 ? qrCode.getSize() : 1;
    const double width = sz.width();
    const double height = sz.height();
    const double aspect = width / height;
    const double size = ((aspect>1.0) ? height : width);
    const double scale = size/(qrSize);

    // NOTE: For performance reasons my implementation only draws the foreground parts in supplied color.
    // It expects background to be prepared already (in white or whatever is preferred).
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);

    for(int y = 0; y < qrSize; y++) {
        for(int x = 0; x < qrSize; x++) {
            const int color = qrCode.getModule(x, y);  // 0 for white, 1 for black
            if(0x0!=color)
            {
                const double rx1 = x*scale;
                const double ry1 = y*scale;

                QRectF rect(rx1, ry1, scale, scale);
                painter.drawRects(&rect,1);
            }
        }
    }
}
