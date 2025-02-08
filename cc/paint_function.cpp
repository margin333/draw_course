#include "../../common/model/custom/paint_function.h"

namespace Paint {
namespace Function {

QImage GetNineStretchedImage(QPixmap *pixOri, const QPoint &point1, const QPoint &point2, const QSize &sizeDes)
{
    int nX1 = point1.x();
    int nY1 = point1.y();
    int nX2 = point2.x();
    int nY2 = point2.y();
    int nWidthDes = sizeDes.width();
    int nHeightDes = sizeDes.height();

    int nW = pixOri->width() / 2;

    int nH = pixOri->height() / 2;
    QPixmap pix = pixOri->scaled(QSize(nW, nH), Qt::KeepAspectRatio);

    QPixmap pix_1 = pix.copy(0, 0, nX1, nY1);
    QPixmap pix_3 = pix_1.transformed(QTransform().scale(-1, 1));
    QPixmap pix_7 = pix_1.transformed(QTransform().scale(1, -1));
    QPixmap pix_9 = pix_1.transformed(QTransform().scale(-1, -1));

    QPixmap pix_2 = pix.copy(nX1, 0, pix.width() - nX1 - nX2, nY1);
    QPixmap pix_4 = pix.copy(0, nY1, nX1, pix.height() - nY1 - nY2);
    QPixmap pix_6 = pix.copy(pix.width() - nX2, nY1, nX2, pix.height() - nY1 - nY2);
    QPixmap pix_8 = pix.copy(nX1, pix.height() - nY2, pix.width() - nX1 - nX2, nY2);

    QPixmap pix_5 = pix.copy(nX1, nY1, pix.width() - nX1 - nX2, pix.height() - nY1 - nY2);

    pix_2 = pix_2.scaled(nWidthDes - nX1 - nX2, nY1, Qt::IgnoreAspectRatio);
    pix_4 = pix_4.scaled(nX1, nHeightDes - nY1 - nY2, Qt::IgnoreAspectRatio);
    pix_5 = pix_5.scaled(nWidthDes - nX1 - nX2, nHeightDes - nY1 - nY2, Qt::IgnoreAspectRatio);
    pix_6 = pix_6.scaled(nX2, nHeightDes - nY1 - nY2, Qt::IgnoreAspectRatio);
    pix_8 = pix_8.scaled(nWidthDes - nX1 - nX2, nY2, Qt::IgnoreAspectRatio);

    QImage resultPix(QSize(nWidthDes, nHeightDes), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&resultPix);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(QRect(0, 0, nWidthDes, nHeightDes), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    if (!resultPix.isNull()) {
        painter.drawPixmap(0, 0, pix_1);
        painter.drawPixmap(nX1, 0, pix_2);
        painter.drawPixmap(nWidthDes - nX2, 0, pix_3);

        painter.drawPixmap(0, nY1, pix_4);
        painter.drawPixmap(nX1, nY1, pix_5);
        painter.drawPixmap(nWidthDes - nX2, nY1, pix_6);

        painter.drawPixmap(0, nHeightDes - nY2, pix_7);
        painter.drawPixmap(nX1, nHeightDes - nY2, pix_8);
        painter.drawPixmap(nWidthDes - nX2, nHeightDes - nY2, pix_9);

        painter.end();
    }
    return resultPix;
}

QImage GetNineStretchedImage(QImage *imgOri, const QPoint &point1, const QPoint &point2, const QSize &sizeDes)
{
    int nX1 = point1.x();
    int nY1 = point1.y();
    int nX2 = point2.x();
    int nY2 = point2.y();
    int nWidthDes = sizeDes.width();
    int nHeightDes = sizeDes.height();
    QPixmap pixOri = QPixmap::fromImage(*imgOri);
    int nW = imgOri->width() / 2;

    int nH = imgOri->height() / 2;
    QPixmap pix = pixOri.scaled(QSize(nW, nH), Qt::KeepAspectRatio);

    QPixmap pix_1 = pix.copy(0, 0, nX1, nY1);

    QPixmap pix_3 = pix_1.transformed(QTransform().scale(-1, 1));
    QPixmap pix_7 = pix_1.transformed(QTransform().scale(1, -1));
    QPixmap pix_9 = pix_1.transformed(QTransform().scale(-1, -1));

    QPixmap pix_2 = pix.copy(nX1, 0, pix.width() - nX1 - nX2, nY1);
    QPixmap pix_4 = pix.copy(0, nY1, nX1, pix.height() - nY1 - nY2);
    QPixmap pix_6 = pix.copy(pix.width() - nX2, nY1, nX2, pix.height() - nY1 - nY2);
    QPixmap pix_8 = pix.copy(nX1, pix.height() - nY2, pix.width() - nX1 - nX2, nY2);

    QPixmap pix_5 = pix.copy(nX1, nY1, pix.width() - nX1 - nX2, pix.height() - nY1 - nY2);

    pix_2 = pix_2.scaled(nWidthDes - nX1 - nX2, nY1, Qt::IgnoreAspectRatio);
    pix_4 = pix_4.scaled(nX1, nHeightDes - nY1 - nY2, Qt::IgnoreAspectRatio);
    pix_5 = pix_5.scaled(nWidthDes - nX1 - nX2, nHeightDes - nY1 - nY2, Qt::IgnoreAspectRatio);
    pix_6 = pix_6.scaled(nX2, nHeightDes - nY1 - nY2, Qt::IgnoreAspectRatio);
    pix_8 = pix_8.scaled(nWidthDes - nX1 - nX2, nY2, Qt::IgnoreAspectRatio);

    QImage resultPix(QSize(nWidthDes, nHeightDes), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&resultPix);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(QRect(0, 0, nWidthDes, nHeightDes), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    if (!resultPix.isNull()) {
        painter.drawPixmap(0, 0, pix_1);
        painter.drawPixmap(nX1, 0, pix_2);
        painter.drawPixmap(nWidthDes - nX2, 0, pix_3);

        painter.drawPixmap(0, nY1, pix_4);
        painter.drawPixmap(nX1, nY1, pix_5);
        painter.drawPixmap(nWidthDes - nX2, nY1, pix_6);

        painter.drawPixmap(0, nHeightDes - nY2, pix_7);
        painter.drawPixmap(nX1, nHeightDes - nY2, pix_8);
        painter.drawPixmap(nWidthDes - nX2, nHeightDes - nY2, pix_9);

        painter.end();
    }
    return resultPix;
}

} // namespace Function
} // namespace Paint
