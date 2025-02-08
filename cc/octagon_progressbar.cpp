#include "octagon_progressbar.h"
namespace Ui {
namespace OctagonProgressBar {
OctagonProgressBar::OctagonProgressBar(QWidget *parent)
    : QProgressBar(parent)
    , nWidthLTBorder_(3)
    , nWidthRBBorder_(2)
    , nWidthBorder_(1)
    , nWidth_(this->width())
    , nHeight_(this->height())
    , nLeftX_(this->height() / 4)
    , nAngleRotation_(45)
    , colorBorder_(QColor(166, 166, 166))
    , colorBackground_(QColor(220, 220, 220))
    , colorBorderLeftTop_(QColor(166, 166, 166))
    , colorLine_(QColor(130, 130, 130))
    , shadow_(nullptr)
{
    nRightX1_ = nWidth_ - nLeftX_;
    nWidthCtrl_ = nWidth_ - 2 * nLeftX_;

    nY3_ = nHeight_ - nLeftX_;
    nHeightCtrl_ = nHeight_ - 2 * nLeftX_;

    nHypotenuse_ = nLeftX_ * fGEN_2;

    shadow_ = new QGraphicsDropShadowEffect(this);
    if (shadow_) {
        shadow_->setColor("#FFFFFF");

        shadow_->setOffset(2, 3);

        shadow_->setBlurRadius(4);
        this->setGraphicsEffect(shadow_);
        this->setStyle(&style_);
    }
}

void OctagonProgressBar::resizeEvent(QResizeEvent *event)
{
    QProgressBar::resizeEvent(event);
    nWidth_ = this->width();
    nHeight_ = this->height();
    nLeftX_ = this->height() / 4;

    nRightX1_ = nWidth_ - nLeftX_;
    nWidthCtrl_ = nWidth_ - 2 * nLeftX_;
    nY3_ = nHeight_ - nLeftX_;
    nHeightCtrl_ = nHeight_ - 2 * nLeftX_;
    nHypotenuse_ = nLeftX_ * fGEN_2;
}

void OctagonProgressBar::paintEvent(QPaintEvent *event)
{
    QProgressBar::paintEvent(event);
    QStyleOptionProgressBar option;
    initStyleOption(&option);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPolygonF octagon;
    octagon << QPoint(nLeftX_, 0);
    octagon << QPoint(nWidth_ - nLeftX_, 0);
    octagon << QPoint(nWidth_, nLeftX_);
    octagon << QPoint(nWidth_, nHeight_ - nLeftX_);
    octagon << QPoint(nWidth_ - nLeftX_, nHeight_);
    octagon << QPoint(nLeftX_, nHeight_);
    octagon << QPoint(0, nHeight_ - nLeftX_);
    octagon << QPoint(0, nLeftX_);
    painter.setPen(Qt::NoPen);
    painter.setBrush(colorBackground_);
    painter.drawPolygon(octagon);

    //  1
    QPolygonF octagonPress1;
    octagonPress1 << QPoint(nLeftX_, 0);
    octagonPress1 << QPoint(nWidth_ - nLeftX_, 0);
    octagonPress1 << QPoint(nWidth_ - nLeftX_ + nWidthLTBorder_, nWidthLTBorder_);
    octagonPress1 << QPoint(nLeftX_, nWidthLTBorder_);

    QLinearGradient background4 = QLinearGradient(0, 0, 0, nWidthLTBorder_);
    background4.setColorAt(0, colorBorder_.darker(110));
    background4.setColorAt(0.2, colorBorder_.darker(105));
    background4.setColorAt(0.4, colorBorder_.darker(100));
    background4.setColorAt(0.6, colorBorder_.darker(95));
    background4.setColorAt(0.8, colorBorder_.darker(90));
    background4.setColorAt(1, colorBackground_);

    painter.setPen(Qt::NoPen);
    painter.setBrush(background4);
    painter.drawPolygon(octagonPress1);

    // 2
    painter.save();
    painter.translate(nRightX1_, 0);

    painter.rotate(nAngleRotation_);

    // 3
    painter.translate(nHypotenuse_, 0);
    painter.rotate(nAngleRotation_);

    // 4
    painter.translate(nHeightCtrl_, 0);
    painter.rotate(nAngleRotation_);

    // 5
    painter.translate(nHypotenuse_, 0);
    painter.rotate(nAngleRotation_);

    // 6
    painter.translate(nWidthCtrl_, 0);
    painter.rotate(nAngleRotation_);

    // 7
    painter.translate(nHypotenuse_, 0);
    painter.rotate(nAngleRotation_);
    QPolygonF octagonPress7;
    octagonPress7 << QPoint(0, 0);
    octagonPress7 << QPoint(nHeightCtrl_, 0);
    octagonPress7 << QPoint(nHeightCtrl_, nWidthLTBorder_ - 1);
    octagonPress7 << QPoint(-nWidthLTBorder_, nWidthLTBorder_ - 1);
    painter.setBrush(colorBorderLeftTop_);
    painter.drawPolygon(octagonPress7);

    // 8
    painter.translate(nHeightCtrl_, 0);
    painter.rotate(nAngleRotation_);
    painter.fillRect(0, 0, nHypotenuse_, nWidthLTBorder_ - 1, colorBorderLeftTop_);

    painter.restore();

    painter.setPen(QPen(colorLine_, 1));
    painter.drawLine(nLeftX_, nHeight_, 0, nHeight_ - nLeftX_);
    painter.drawLine(nWidth_ - nLeftX_, nHeight_, nLeftX_, nHeight_);
    QWidget::paintEvent(event);

    option.text = QString("%1%").arg(value());

    QStylePainter stylePainter(this);
    stylePainter.drawControl(QStyle::CE_ProgressBar, option);
}

StyleProgress::StyleProgress()
    : imgBlock_(":/img/bne_img/img_progress_block.png")
{
}

StyleProgress::~StyleProgress() { }

void StyleProgress::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                  const QWidget *widget) const
{
    if (element == PE_IndicatorProgressChunk) {
        const QStyleOptionProgressBar *progressBarOption = qstyleoption_cast<const QStyleOptionProgressBar *>(option);
        if (progressBarOption) {
            QRect rectProgress = widget->rect();
            rectProgress.setSize(QSize(rectProgress.width() - 3, rectProgress.height() - 2));

            int width_ = rectProgress.width();
            int height_ = rectProgress.height();
            int iTmpX_ = rectProgress.height() / 4;

            QPolygon octagonMask;
            octagonMask << QPoint(iTmpX_ + 2, 1);
            octagonMask << QPoint(width_ - iTmpX_ + 2, 1);
            octagonMask << QPoint(width_ + 2, iTmpX_ + 1);
            octagonMask << QPoint(width_ + 2, height_ - iTmpX_ + 1);
            octagonMask << QPoint(width_ - iTmpX_ + 2, height_ + 1);
            octagonMask << QPoint(iTmpX_ + 2, height_ + 1);
            octagonMask << QPoint(0 + 2, height_ - iTmpX_ + 1);
            octagonMask << QPoint(0 + 2, iTmpX_ + 1);
            QRegion regionMask(octagonMask);
            painter->setClipRegion(regionMask);
            QRect rectOption = option->rect;
            painter->save();
            if (!imgBlock_.isNull()) {
                float scaleFactor = float(rectOption.height()) / imgBlock_.height();

                QImage scaledImage = imgBlock_.scaled(imgBlock_.width() * scaleFactor, rectOption.height() - 3,
                                                      Qt::KeepAspectRatio, Qt::SmoothTransformation);

                int nNum = (rectOption.width() - 2) / (scaledImage.width() + 1);
                if (rectOption.width() == widget->rect().width()) {
                    ++nNum;
                }

                for (int i = 0; i < nNum; ++i) {
                    painter->drawImage(QPoint((scaledImage.width() + 1) * i, 2), scaledImage);
                }
            }
            return;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void StyleProgress::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter,
                                const QWidget *widget) const
{
    switch (element) {
    case CE_ProgressBar:
        if (const QStyleOptionProgressBar *pv2 = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
            drawControl(CE_ProgressBarContents, pv2, painter, widget);
            if (pv2->textVisible) {
                drawControl(CE_ProgressBarLabel, pv2, painter, widget);
            }
        }
        break;
    case CE_ProgressBarContents:
        DrawContains(CE_ProgressBarContents, option, painter, widget);
        break;
    case CE_ProgressBarLabel:
        break;
    case CE_ProgressBarGroove:
    default:
        return QProxyStyle::drawControl(element, option, painter, widget);
    }
}

QRect StyleProgress::subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    switch (element) {
    case SE_ProgressBarLabel:
        if (const QStyleOptionProgressBar *v = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
            QStyleOptionProgressBar v2(*v);
            if (v2.orientation == Qt::Vertical) {
                return QRect(v2.rect.x(), v2.rect.height() * 0.4, v2.rect.width(), v2.rect.height() * 0.2);
            } else {
                return QRect(v2.rect.width() * 0.4, v2.rect.y(), v2.rect.width() * 0.2, v2.rect.height());
            }
        } else {
            return QProxyStyle::subElementRect(element, option, widget);
        }
    default:
        return QProxyStyle::subElementRect(element, option, widget);
    }
}

void StyleProgress::DrawContains(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter,
                                 const QWidget *widget) const
{
    Q_UNUSED(element);
    if (const QStyleOptionProgressBar *v = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
        QStyleOptionProgressBar v2(*v);
        QRect rect_contens = subElementRect(SE_ProgressBarContents, option, widget);
        painter->save();
        painter->setPen(Qt::transparent);
        painter->setBrush(Qt::transparent);

        int cx = 12 * 100 / rect_contens.width();
        int cy = 12 * 100 / rect_contens.height();
        painter->drawRoundRect(rect_contens, cx, cy);
        painter->restore();

        if (v2.orientation == Qt::Vertical) {
            v2.rect.setHeight((v2.progress * v->rect.height()) / v2.maximum);
        } else {
            v2.rect.setWidth((v2.progress * v->rect.width()) / v2.maximum);
        }
        drawPrimitive(PE_IndicatorProgressChunk, &v2, painter, widget);
    }
}
} // namespace OctagonProgressBar
} // namespace Ui
