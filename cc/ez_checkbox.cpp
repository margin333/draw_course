#include "ez_checkbox.h"
#include <QDebug>
namespace Ui {
namespace EzCheckBox {

EzCheckBox::EzCheckBox(QWidget *parent)
    : QCheckBox(parent)
{
    this->setStyle(&style_);
}

EzCheckBox::EzCheckBox(const QString &text, QWidget *parent)
    : QCheckBox(text, parent)
{
    this->setStyle(&style_);
}

EzCheckBox::~EzCheckBox() { }

CustomCheckBoxStyle::CustomCheckBoxStyle(QStyle *baseStyle)
    : QProxyStyle(baseStyle)
    , imageStateOn_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_check_press.png")
    , imageStateOff_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_check_enable.png")
{
}

void CustomCheckBoxStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                        const QWidget *widget) const
{
    if (PE_IndicatorCheckBox == element && option) {

        const QStyleOptionButton *checkBoxOption = qstyleoption_cast<const QStyleOptionButton *>(option);
        if (checkBoxOption) {
            painter->setRenderHint(QPainter::Antialiasing);

            QRect rectIndicator(checkBoxOption->rect);

            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::transparent);
            painter->drawRect(rectIndicator);

            if (checkBoxOption->state & State_On) {
                if (!imageStateOn_.isNull()) {
                    float scaleFactor = float(rectIndicator.width()) / imageStateOn_.width();

                    QImage scaledImage =
                        imageStateOn_.scaled(imageStateOn_.width() * scaleFactor, imageStateOn_.height() * scaleFactor,
                                             Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    if (!scaledImage.isNull()) {
                        painter->drawImage(rectIndicator, scaledImage);
                    }
                }
            } else if (checkBoxOption->state & State_Off) {
                painter->drawImage(rectIndicator, imageStateOff_);
            }
            return;
        }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

QRect CustomCheckBoxStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    if (SE_CheckBoxClickRect == element) {
        QRect rectTmp = QProxyStyle::subElementRect(element, option, widget);
        return rectTmp.adjusted(0, 0, 5, 0);
    } else {
        return QProxyStyle::subElementRect(element, option, widget);
    }
};

void CustomCheckBoxStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                      const QWidget *widget) const
{
    if (CE_CheckBoxLabel == element) {
        const QStyleOptionButton *checkBoxOption = qstyleoption_cast<const QStyleOptionButton *>(option);
        if (checkBoxOption) {
            QRect checkBoxRect = subElementRect(SE_CheckBoxClickRect, option, widget);
            painter->setPen(Qt::black);
            painter->drawText(checkBoxRect, Qt::AlignVCenter | Qt::AlignLeft, checkBoxOption->text);
        } else {
            return QProxyStyle::drawControl(element, option, painter, widget);
        }

    } else {
        return QProxyStyle::drawControl(element, option, painter, widget);
    }
};

} // namespace EzCheckBox
} // namespace Ui
