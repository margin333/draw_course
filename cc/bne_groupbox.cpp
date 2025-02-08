#include "bne_groupbox.h"
#include "qstyleoption.h"

BNEGroupBox::BNEGroupBox(QWidget *parent)
    : QGroupBox(parent)
{
    this->setStyle(&style_);
}

//----------------------------------------------------------------------
QRect BNEGroupBoxStyle ::subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc,
                                        const QWidget *widget) const
{
    QRect rect = QProxyStyle::subControlRect(cc, opt, sc, widget);
    if (CC_GroupBox == cc) {
        switch (sc) {
        case SC_GroupBoxLabel:
            rect.adjust(5, 0, 5, 0);
            break;
        default:
            break;
        }
    }
    return rect;
}

void BNEGroupBoxStyle ::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                      const QWidget *widget) const
{
    if (element == PE_FrameGroupBox) {

        if (const QStyleOptionFrame *frame = qstyleoption_cast<const QStyleOptionFrame *>(option)) {
            if (frame->features & QStyleOptionFrame::Flat) {
                return QProxyStyle::drawPrimitive(element, option, painter, widget);
            } else {

                painter->setRenderHint(QPainter::Antialiasing);
                painter->setPen(QColor(178, 205, 243));
                painter->setBrush(Qt::NoBrush);
                painter->drawRoundedRect(frame->rect.adjusted(1, 0, -1, -1), 10, 10);
            }
        }
        return;
    } else {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};
