// Todo auth description

#include "QrMaker.h"
#include <QObject>
#include <QQuickView>
#include <QDebug>

//========================================================
QrMaker::QrMaker(QQuickItem *parent)
    : QQuickPaintedItem(parent)
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
       // NOTE: At this point you will use the API to get the encoding and format you want, instead of my hardcoded stuff:
       char *str = url.toUtf8().data();
       ::qrcodegen::QrCode qrCode = ::qrcodegen::QrCode::encodeText(str, ::qrcodegen::QrCode::Ecc::HIGH);
       const int qrSize = qrCode.getSize() > 0 ? qrCode.getSize() : 1;
       const double width = sz.width();
       const double height = sz.height();
       const double aspect = width / height;
       const double size = ((aspect>1.0) ? height : width);
       const double scale = size/(qrSize+2);

       // NOTE: For performance reasons my implementation only draws the foreground parts in supplied color.
       // It expects background to be prepared already (in white or whatever is preferred).
       painter.setPen(Qt::NoPen);
       painter.setBrush(color);

       for(int y = 0; y < qrSize; y++) {
           for(int x = 0; x < qrSize; x++) {
               const int color = qrCode.getModule(x, y);  // 0 for white, 1 for black
               if(0x0!=color) {
                   const double rx1 = (x+1)*scale;
                   const double ry1 = (y+1)*scale;

                   QRectF rect(rx1, ry1, scale, scale);
                   painter.drawRects(&rect,1);
               }
           }
       }
}

//========================================================
void QrMaker::paint(QPainter *painter)
//========================================================
{
    // QQuickItem's paint override
    QQuickView view;
    view.setSource(QUrl("qrc:/contentPlayer.qml"));
    QObject *QRItem = view.rootObject();

    QSize qSize(0, 0);
    QString url = "http://www.naver.com"; // get ip from shared memory
    QColor color = Qt::black;
    if(QRItem)
    {
        const QVariant width = QRItem->property("width");
        const QVariant height = QRItem->property("height");
        qSize.setWidth(width.toInt());
        qSize.setHeight(height.toInt());

        qDebug("QR Size - width : %d, height : %d", width.toInt(), height.toInt());
    }
    else
    {
        qSize.setWidth(400);
        qSize.setHeight(400);

        qDebug("Not Connected QRItem (deafult size)");
    }

    DrawQrCode(*painter, qSize, url, color);

    delete QRItem;
}
