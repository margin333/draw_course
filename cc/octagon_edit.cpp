#include "octagon_edit.h"

namespace Ui {
namespace OctagonEdit {
OctagonEdit::OctagonEdit(QWidget *parent)
    : QLineEdit(parent)
    , nWidthLeftTopBorder_(2)
    , nWidthRightBottomBorder_(2)
    , nWidthBorder_(1)
    , nAngleRotation_(45)
    , colorBackground_(QColor(207, 207, 207))
    , colorBorderLeftTop_(QColor(166, 166, 166))
    , colorBorderRightBottom_(QColor(255, 255, 255, 255))
    , colorLine_(QColor(130, 130, 130))
    , colorBackgroundTransparent_(QColor(207, 207, 207, 0))
{

    setAttribute(Qt::WA_StyledBackground, true);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, colorBackground_);
    this->setPalette(pal);

    this->setAlignment(Qt::AlignCenter);

    nWidth_ = this->width();
    nHeight_ = this->height();
    nLeftX1_ = this->height() / 4;

    nRightX1_ = nWidth_ - nLeftX1_;
    nWidthCtrl_ = nWidth_ - 2 * nLeftX1_;

    nY3_ = nHeight_ - nLeftX1_;
    nHeightCtrl_ = nHeight_ - 2 * nLeftX1_;

    nHypotenuse_ = nLeftX1_ * fGEN_2;
}

int OctagonEdit::getPaddingWidth() { return nLeftX1_; }

void OctagonEdit::resizeEvent(QResizeEvent *event)
{

    nWidth_ = this->width();
    nHeight_ = this->height();
    nLeftX1_ = this->height() / 4;
    nRightX1_ = nWidth_ - nLeftX1_;
    nWidthCtrl_ = nWidth_ - 2 * nLeftX1_;
    nY3_ = nHeight_ - nLeftX1_;
    nHeightCtrl_ = nHeight_ - 2 * nLeftX1_;
    nHypotenuse_ = nLeftX1_ * fGEN_2;

    QPainterPath path;
    path.moveTo(QPoint(nLeftX1_, 0));
    path.lineTo(QPoint(nWidth_ - nLeftX1_, 0));
    path.lineTo(QPoint(nWidth_, nLeftX1_));
    path.lineTo(QPoint(nWidth_, nHeight_ - nLeftX1_));
    path.lineTo(QPoint(nWidth_ - nLeftX1_, nHeight_));
    path.lineTo(QPoint(nLeftX1_, nHeight_));
    path.lineTo(QPoint(0, nHeight_ - nLeftX1_));
    path.lineTo(QPoint(0, nLeftX1_));
    path.closeSubpath();

    QRegion region1(path.toFillPolygon().toPolygon());
    this->setMask(region1);

    QLineEdit::resizeEvent(event);
}

OctagonEdit::~OctagonEdit() { }

void OctagonEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(colorBackgroundTransparent_);
    painter.drawRect(this->rect());

    QBrush brushLT(colorBorderLeftTop_);
    QBrush brushRB(colorBorderRightBottom_);
    // 1
    QPolygonF octagonPress1;
    octagonPress1 << QPoint(nLeftX1_, 0);
    octagonPress1 << QPoint(nWidth_ - nLeftX1_, 0);
    octagonPress1 << QPoint(nWidth_ - nLeftX1_ + nWidthLeftTopBorder_, nWidthLeftTopBorder_);
    octagonPress1 << QPoint(nLeftX1_, nWidthLeftTopBorder_);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brushLT);
    painter.drawPolygon(octagonPress1);

    // 2
    painter.save();
    painter.translate(nRightX1_, 0);
    painter.rotate(nAngleRotation_);

    // 3
    painter.translate(nHypotenuse_, 0);
    painter.rotate(nAngleRotation_);
    QPolygonF octagonPress3;
    octagonPress3 << QPoint(0, 0);
    octagonPress3 << QPoint(nHeightCtrl_, 0);
    octagonPress3 << QPoint(nHeightCtrl_, nWidthRightBottomBorder_);
    octagonPress3 << QPoint(-nWidthRightBottomBorder_, nWidthRightBottomBorder_);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brushRB);
    painter.drawPolygon(octagonPress3);

    // 4
    painter.translate(nHeightCtrl_ + 1, 0);
    painter.rotate(nAngleRotation_);

    painter.fillRect(0, 0, nHypotenuse_, nWidthRightBottomBorder_, brushRB);

    // 5
    painter.translate(nHypotenuse_, 0);
    painter.rotate(nAngleRotation_);
    QPolygonF octagonPress5;
    octagonPress5 << QPoint(0, 0);
    octagonPress5 << QPoint(nWidthCtrl_, 0);
    octagonPress5 << QPoint(nWidthCtrl_ + nWidthRightBottomBorder_, nWidthRightBottomBorder_);
    octagonPress5 << QPoint(0, nWidthRightBottomBorder_);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brushRB);
    painter.drawPolygon(octagonPress5);

    // 6
    painter.translate(nWidthCtrl_, 0);
    painter.rotate(nAngleRotation_);

    // 7
    painter.translate(nHypotenuse_, 0);
    painter.rotate(nAngleRotation_);
    QPolygonF octagonPress7;
    octagonPress7 << QPoint(0, 0);
    octagonPress7 << QPoint(nHeightCtrl_, 0);
    octagonPress7 << QPoint(nHeightCtrl_, nWidthLeftTopBorder_);
    octagonPress7 << QPoint(-nWidthLeftTopBorder_, nWidthLeftTopBorder_);
    painter.setBrush(brushLT);
    painter.drawPolygon(octagonPress7);

    // 8
    painter.translate(nHeightCtrl_, 0);
    painter.rotate(nAngleRotation_);
    painter.fillRect(0, -1, nHypotenuse_, nWidthLeftTopBorder_, brushLT);

    painter.restore();
}

int OctagonEdit::GetLTBorderWidth() { return this->nWidthLeftTopBorder_; }

int OctagonEdit::GetRBBorderWidth() { return this->nWidthRightBottomBorder_; }

int OctagonEdit::GetBorderWidth() { return this->nWidthBorder_; }
} // namespace OctagonEdit
} // namespace Ui
