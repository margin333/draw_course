#include "roundrect_button.h"
#include <QDebug>

namespace Ui {
namespace RoundRectBtn {
RoundRectButton::RoundRectButton(QWidget *parent)
    : QPushButton(parent)
    , bPress_(false)
    , colorBackground_(37, 121, 204)
    , colorBackgroundPressed_(3, 61, 128)
    , colorTextEnable_(255, 255, 255)
    , colorTextDisenable_(124, 170, 217)
{
}

RoundRectButton::RoundRectButton(const QString &strText, QWidget *parent)
    : QPushButton(strText, parent)
    , bPress_(false)
    , colorBackground_(37, 121, 204)
    , colorBackgroundPressed_(3, 61, 128)
    , colorTextEnable_(255, 255, 255)
    , colorTextDisenable_(124, 170, 217)
{
}

void RoundRectButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.save();
    QPainterPath path;
    path.addRoundedRect(rect(), height() / 5, height() / 5);
    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);
    if (!this->isEnabled()) {
        painter.setBrush(colorBackground_);
        painter.drawPath(path);
        painter.setPen(QPen(colorTextDisenable_, 2));
    } else {
        if (!bPress_) {
            painter.setBrush(colorBackground_);
        } else {
            painter.setBrush(colorBackgroundPressed_);

        }
        painter.drawPath(path);
        painter.setPen(QPen(colorTextEnable_, 2));
    }

    painter.drawText(rect(), Qt::AlignCenter, this->text());
    painter.restore();
}

void RoundRectButton::resizeEvent(QResizeEvent *event) { return QPushButton::resizeEvent(event); };

void RoundRectButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);
    bPress_ = true;
    update();
}
void RoundRectButton::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);
    bPress_ = false;
    update();
}

} // namespace RoundRectBtn
} // namespace Ui
