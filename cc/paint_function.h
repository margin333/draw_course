#ifndef PAINT_FUNCTION_H
#define PAINT_FUNCTION_H

#include <QImage>
#include <QPixmap>
#include <QPainter>

namespace Paint {
namespace Function {

QImage GetNineStretchedImage(QPixmap *pixOri, const QPoint &point1, const QPoint &point2, const QSize &sizeDes);

QImage GetNineStretchedImage(QImage *pixOri, const QPoint &point1, const QPoint &point2, const QSize &sizeDes);

} // namespace Function
} // namespace Paint

#endif // PAINT_FUNCTION_H
