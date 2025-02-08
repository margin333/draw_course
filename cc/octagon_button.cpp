#include "octagon_button.h"
namespace Ui {
namespace OctagonButton {
OctagonButton::OctagonButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
    , nWidthLeftTopBorder_(4)
    , nWidthRightBottomBorder_(5)
    , nWidthBorder_(2)
    , nAngleRotation_(45)
    , bPress_(false)
    , colorBorder_(QColor(117, 117, 117))
    , colorBackground_(QColor(207, 207, 207))
    , colorPressLeftTop_(QColor(166, 166, 166))
    , colorPressRightBottom_(QColor(245, 245, 245))
{
}

OctagonButton::OctagonButton(QWidget *parent)
    : QPushButton(parent)
    , nWidthLeftTopBorder_(4)
    , nWidthRightBottomBorder_(5)
    , nWidthBorder_(2)
    , nAngleRotation_(45)
    , bPress_(false)
    , colorBorder_(QColor(117, 117, 117))
    , colorBackground_(QColor(207, 207, 207))
    , colorPressLeftTop_(QColor(166, 166, 166))
    , colorPressRightBottom_(QColor(245, 245, 245))
{
}

void OctagonButton::mousePressEvent(QMouseEvent *evt)
{

    bPress_ = true;
    QPalette palette = this->palette();
    palette.setColor(QPalette::ButtonText, Qt::red);
    this->setPalette(palette);
    update();
    QPushButton::mousePressEvent(evt);
}

void OctagonButton::mouseReleaseEvent(QMouseEvent *evt)
{
    bPress_ = false;
    QPalette palette = this->palette();
    palette.setColor(QPalette::ButtonText, Qt::black);
    this->setPalette(palette);
    update();
    QPushButton::mouseReleaseEvent(evt);
}

void OctagonButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(colorBackground_);
    painter.drawRect(rect());

    if (!bPress_) {
        QLinearGradient background = QLinearGradient(0, 0, 0, nWidthLeftTopBorder_);
        background.setColorAt(0, colorBorder_.darker(90));
        background.setColorAt(0.7, colorBorder_.darker(50));
        background.setColorAt(1, colorBorder_.lighter(300));
        painter.fillRect(nLeftX1_, 0, nWidth_ - 2 * nLeftX1_, nWidthLeftTopBorder_, background);

        painter.save();
        painter.translate(nRightX1_, 0);
        painter.rotate(nAngleRotation_);
        QLinearGradient background1 = QLinearGradient(0, 0, 0, nWidthBorder_);
        background1.setColorAt(0, colorBorder_.darker(100));
        background1.setColorAt(0.15, colorBorder_.darker(90));
        background1.setColorAt(0.9, colorBackground_);
        painter.fillRect(0, 0, nHypotenuse_, nWidthBorder_, background1);

        painter.translate(nHypotenuse_, 0);
        painter.rotate(nAngleRotation_);
        QLinearGradient background2 = QLinearGradient(0, 0, 0, 7);
        background2.setColorAt(0, colorBorder_.darker(90));
        background2.setColorAt(0.4, colorBorder_.darker(90));
        background2.setColorAt(0.8, colorBorder_.darker(70));
        painter.fillRect(0, 0, nHeightCtrl_, nWidthRightBottomBorder_, background2);

        painter.translate(nHeightCtrl_, 0);
        painter.rotate(nAngleRotation_);
        QLinearGradient background3 = QLinearGradient(0, 0, 0, nWidthRightBottomBorder_);
        background3.setColorAt(0, colorBorder_.darker(120));
        background3.setColorAt(0.3, colorBorder_.darker(100));
        background3.setColorAt(0.5, colorBorder_.darker(80));
        painter.fillRect(0, 0, nHypotenuse_, nWidthRightBottomBorder_, background3);

        painter.translate(nHypotenuse_, 0);
        painter.rotate(nAngleRotation_);
        QLinearGradient background4 = QLinearGradient(0, 0, 0, nWidthRightBottomBorder_);
        background4.setColorAt(0, colorBorder_.darker(120));
        background4.setColorAt(0.3, colorBorder_.darker(90));
        background4.setColorAt(0.5, colorBorder_.darker(70));
        painter.fillRect(0, 0, nWidthCtrl_, nWidthRightBottomBorder_, background4);

        painter.translate(nWidthCtrl_, 0);
        painter.rotate(nAngleRotation_);
        QLinearGradient background5 = QLinearGradient(0, 0, 0, nWidthBorder_);
        background5.setColorAt(0, colorBorder_.darker(100));
        background5.setColorAt(0.15, colorBorder_.darker(90));
        background5.setColorAt(0.9, colorBackground_);
        painter.fillRect(0, 0, nHypotenuse_, nWidthBorder_, background5);

        painter.translate(nHypotenuse_, 0);
        painter.rotate(nAngleRotation_);
        QLinearGradient background6 = QLinearGradient(0, 0, 0, nWidthLeftTopBorder_);
        background6.setColorAt(0, colorBorder_.darker(90));
        background6.setColorAt(0.7, colorBorder_.darker(50));
        background6.setColorAt(1, colorBorder_.lighter(300));
        painter.fillRect(0, 0, nHeightCtrl_, nWidthLeftTopBorder_, background6);

        painter.translate(nHeightCtrl_, 0);
        painter.rotate(nAngleRotation_);
        QLinearGradient background7 = QLinearGradient(0, 0, 0, nWidthLeftTopBorder_);
        background7.setColorAt(0, colorBorder_.darker(90));
        background7.setColorAt(0.7, colorBorder_.darker(50));
        background7.setColorAt(1, colorBorder_.lighter(300));
        painter.fillRect(0, 0, nHypotenuse_, nWidthLeftTopBorder_, background7);

        painter.restore();
    } else {
        QBrush brushLTPress(colorPressLeftTop_);
        QBrush brushRBRelease(colorPressRightBottom_);

        QPolygonF octagonPress1;
        octagonPress1 << QPoint(nLeftX1_, 1);                                                     // 左上
        octagonPress1 << QPoint(nWidth_ - nLeftX1_, 1);                                           // 右上
        octagonPress1 << QPoint(nWidth_ - nLeftX1_ + nWidthLeftTopBorder_, nWidthLeftTopBorder_); // 右下
        octagonPress1 << QPoint(nLeftX1_, nWidthLeftTopBorder_);                                  // 左下
        painter.setBrush(brushLTPress);
        painter.drawPolygon(octagonPress1);

        painter.save();
        painter.translate(nRightX1_, 0);
        painter.rotate(nAngleRotation_);

        painter.translate(nHypotenuse_, 0);
        painter.rotate(nAngleRotation_);
        QPolygonF octagonPress3;
        octagonPress3 << QPoint(0, 0);                        // 左上
        octagonPress3 << QPoint(nHeightCtrl_, 0);             // 右上
        octagonPress3 << QPoint(nHeightCtrl_, nWidthBorder_); // 右下
        octagonPress3 << QPoint(-nWidthRightBottomBorder_, nWidthBorder_);
        painter.setPen(Qt::NoPen);
        painter.setBrush(brushRBRelease);
        painter.drawPolygon(octagonPress3);

        painter.translate(nHeightCtrl_, 0);
        painter.rotate(nAngleRotation_);
        painter.fillRect(0, 0, nHypotenuse_, nWidthBorder_, brushRBRelease);

        painter.translate(nHypotenuse_, 0);
        painter.rotate(nAngleRotation_);
        QPolygonF octagonPress5;
        octagonPress5 << QPoint(0, 0);
        octagonPress5 << QPoint(nWidthCtrl_, 0);
        octagonPress5 << QPoint(nWidthCtrl_ + nWidthRightBottomBorder_, nWidthBorder_);
        octagonPress5 << QPoint(0, nWidthBorder_);
        painter.setPen(Qt::NoPen);
        painter.setBrush(brushRBRelease);
        painter.drawPolygon(octagonPress5);

        painter.translate(nWidthCtrl_, 0);
        painter.rotate(nAngleRotation_);

        painter.translate(nHypotenuse_, 0);
        painter.rotate(nAngleRotation_);
        QPolygonF octagonPress7;
        octagonPress7 << QPoint(0, 0);
        octagonPress7 << QPoint(nHeightCtrl_, 0);
        octagonPress7 << QPoint(nHeightCtrl_, nWidthLeftTopBorder_);
        octagonPress7 << QPoint(-nWidthLeftTopBorder_, nWidthLeftTopBorder_);
        painter.setBrush(brushLTPress);
        painter.drawPolygon(octagonPress7);

        painter.translate(nHeightCtrl_, 0);
        painter.rotate(nAngleRotation_);
        painter.fillRect(0, 0, nHypotenuse_, nWidthLeftTopBorder_, brushLTPress);

        painter.restore();
    }

    painter.setPen(QPen(colorBorder_, 1));
    painter.setBrush(Qt::NoBrush);

    if (!text().isEmpty()) {
        painter.setPen(palette().buttonText().color());
        painter.drawText(this->rect(), Qt::AlignCenter, text());
    }
}

void OctagonButton::resizeEvent(QResizeEvent *event)
{
    nWidth_ = this->width();
    nHeight_ = this->height();

    nLeftX1_ = this->height() / 4;

    nRightX1_ = nWidth_ - nLeftX1_;
    nWidthCtrl_ = nWidth_ - 2 * nLeftX1_;

    nY3_ = nHeight_ - nLeftX1_;
    nHeightCtrl_ = nHeight_ - 2 * nLeftX1_;

    nHypotenuse_ = nLeftX1_ * fGEN_2;

    QPolygon octagon;
    octagon << QPoint(nLeftX1_, 0);
    octagon << QPoint(nWidth_ - nLeftX1_, 0);
    octagon << QPoint(nWidth_, nLeftX1_);
    octagon << QPoint(nWidth_, nHeight_ - nLeftX1_);
    octagon << QPoint(nWidth_ - nLeftX1_, nHeight_);
    octagon << QPoint(nLeftX1_, nHeight_);
    octagon << QPoint(0, nHeight_ - nLeftX1_);
    octagon << QPoint(0, nLeftX1_);

    this->setMask(octagon);

    QPushButton::resizeEvent(event);
}

void OctagonButton::SetBorderWidth(int iWidthLTBorder, int iWidthRBBorder, int iWidthBorder)
{
    this->nWidthLeftTopBorder_ = iWidthLTBorder;
    this->nWidthRightBottomBorder_ = iWidthRBBorder;
    this->nWidthBorder_ = iWidthBorder;
}
} // namespace OctagonButton
} // namespace Ui
