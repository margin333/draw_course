#include "roundrect_progressbar.h"

namespace Ui {
namespace RoundRectProgress {
RoundRectProgressBar::RoundRectProgressBar(QWidget *parent)
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
{
    nRightX1_ = nWidth_ - nLeftX_;
    nWidthCtrl_ = nWidth_ - 2 * nLeftX_;

    nY3_ = nHeight_ - nLeftX_;
    nHeightCtrl_ = nHeight_ - 2 * nLeftX_;

    nHypotenuse_ = nLeftX_ * fGEN_2;

    // 1.
    StyleProgressBar *pStyle = &StyleInstance<StyleProgressBar>::GetIntance();
    this->setStyle(pStyle);
}

void RoundRectProgressBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOptionProgressBar option;
    initStyleOption(&option);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.save();
    QPainterPath path;
    path.addRoundedRect(rect(), rect().height() / 2, rect().height());
    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(246, 246, 246));
    painter.drawRect(rect());
    painter.restore();

    QStylePainter stylePainter(this);
    stylePainter.drawControl(QStyle::CE_ProgressBar, option);
}

//-----------------------------------------------------------------------------
StyleProgressBar::StyleProgressBar()
    : imgBlock_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_progress_blue_block.png")
    , imgBackground_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_progress_background.png")
{
    imgBlock_ = imgBlock_.convertToFormat(QImage::Format_ARGB32);
    imgBackground_ = imgBackground_.convertToFormat(QImage::Format_ARGB32);
}

StyleProgressBar::~StyleProgressBar() { }

void StyleProgressBar::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                     const QWidget *widget) const
{
    if (element == PE_IndicatorProgressChunk) {
        const QStyleOptionProgressBar *progressBarOption = qstyleoption_cast<const QStyleOptionProgressBar *>(option);
        if (progressBarOption) {
            QRect rectProgress = widget->rect();
            int nPadding = 1;
            if (rectProgress.height() > 10) {
                nPadding = rectProgress.height() / 10;
            }
            rectProgress.adjust(nPadding + 1, nPadding + 1, -1 * nPadding - 1, -1 * nPadding - 1);

            QPainterPath path;
            path.addRoundedRect(rectProgress, rectProgress.height() / 2, rectProgress.height());
            painter->setClipPath(path);

            QRect rectOption = option->rect;
            painter->save();
            if (!imgBlock_.isNull()) {
                float scaleFactor = float(rectOption.height()) / imgBlock_.height();

                QImage scaledImage = imgBlock_.scaled(imgBlock_.width() * scaleFactor, rectOption.height() - 3,
                                                      Qt::KeepAspectRatio, Qt::SmoothTransformation);

                int nNum = (rectOption.width() - 2) / (scaledImage.width() + widget->height() / 13);
                if (rectOption.width() == widget->rect().width()) {
                    ++nNum;
                }

                for (int i = 0; i < nNum; ++i) {
                    painter->drawImage(QPoint((scaledImage.width() + widget->height() / 13) * i, 2), scaledImage);
                }
            }
            painter->restore();
            return;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

/**************************************************************************
 * 名称:  drawControl
 * 功能:  绘制控制元素，CE_ProgressBar，CE_ProgressBarContents，由于不希望进度条内部有文字出现直接跳过
 * 输入参数:  QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget
 * 输出参数:  NULL
 * 返回值:   NULL
 * 创建者:   jiwei
 * 创建时间: 2024.02.05
 ***************************************************************************/
void StyleProgressBar::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter,
                                   const QWidget *widget) const
{
    switch (element) {
    case CE_ProgressBar: {
        const QStyleOptionProgressBar *pOP = qstyleoption_cast<const QStyleOptionProgressBar *>(option);
        if (pOP) {
            drawControl(CE_ProgressBarContents, pOP, painter, widget);
            if (pOP->textVisible) {
                drawControl(CE_ProgressBarLabel, pOP, painter, widget);
            }
        }
    } break;
    case CE_ProgressBarContents:
        DrawContains(CE_ProgressBarContents, option, painter, widget);
        break;
    case CE_ProgressBarLabel:
        break;
    default:
        return QProxyStyle::drawControl(element, option, painter, widget);
    }
}

QRect StyleProgressBar::subElementRect(QStyle::SubElement element, const QStyleOption *option,
                                       const QWidget *widget) const
{
    switch (element) {
    case SE_ProgressBarContents:
        return option->rect;
    case SE_ProgressBarLabel:
        if (const QStyleOptionProgressBar *pOP = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
            QStyleOptionProgressBar opTmp(*pOP);
            if (opTmp.orientation == Qt::Vertical) {
                return QRect(opTmp.rect.x(), opTmp.rect.height() * 0.4, opTmp.rect.width(), opTmp.rect.height() * 0.2);
            } else {
                return QRect(opTmp.rect.width() * 0.4, opTmp.rect.y(), opTmp.rect.width() * 0.2, opTmp.rect.height());
            }
        } else {
            return QProxyStyle::subElementRect(element, option, widget);
        }
    default:
        return QProxyStyle::subElementRect(element, option, widget);
    }
}

void StyleProgressBar::DrawContains(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter,
                                    const QWidget *widget) const
{
    Q_UNUSED(element);
    const QStyleOptionProgressBar *pOP = qstyleoption_cast<const QStyleOptionProgressBar *>(option);
    if (pOP) {
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        QStyleOptionProgressBar opTmp(*pOP);
        painter->save();
        painter->setPen(Qt::transparent);
        painter->setBrush(Qt::transparent);

        QRect rectProgress = widget->rect();

        QPainterPath path;
        path.addRoundedRect(rectProgress, rectProgress.height() / 2, rectProgress.height());
        painter->setClipPath(path);

        painter->setPen(QPen(QColor(172, 182, 219), 2));
        painter->setBrush(QColor(245, 249, 255));
        painter->drawRoundedRect(rectProgress, rectProgress.height() / 2, rectProgress.height());
        painter->restore();
        if (opTmp.orientation == Qt::Vertical) {
            opTmp.rect.setHeight((opTmp.progress * pOP->rect.height()) / opTmp.maximum);
        } else {
            opTmp.rect.setWidth((opTmp.progress * pOP->rect.width()) / opTmp.maximum);
        }
        drawPrimitive(PE_IndicatorProgressChunk, &opTmp, painter, widget);
    }
}
} // namespace RoundRectProgress
} // namespace Ui
