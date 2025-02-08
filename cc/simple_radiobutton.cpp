#include "simple_radiobutton.h"
namespace Ui {
namespace SimpleRadio {
SimpleRadioButton::SimpleRadioButton(QWidget *parent)
    : QRadioButton(parent)
{
    SimpleRadioStyle *pStyle = &StyleInstance<SimpleRadioStyle>::GetIntance();
    this->setStyle(pStyle);
    // this->setStyle(&style_);
}

SimpleRadioButton::SimpleRadioButton(const QString &text, QWidget *parent)
    : QRadioButton(text, parent)
{
    SimpleRadioStyle *pStyle = &StyleInstance<SimpleRadioStyle>::GetIntance();
    this->setStyle(pStyle);
}

//------------------------------------radiostyle------------

void SimpleRadioStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                     const QWidget *widget) const
{
    if (PE_IndicatorRadioButton == element && option) {
        const QStyleOptionButton *checkBoxOption = qstyleoption_cast<const QStyleOptionButton *>(option);
        if (checkBoxOption) {
            painter->setRenderHint(QPainter::Antialiasing);

            painter->save();

            painter->translate(1, widget->rect().height() / 2 - option->rect.height() / 2);
            QRect rect = option->rect;
            int nW = rect.width();

            int nR = nW / 2;
            QPoint point(nR, nR);
            QPoint point1(nR * 6 / 5, nR * 6 / 5 / 2);
            QRadialGradient gradient(point, nR, point1);
            gradient.setColorAt(0, QColor(39, 91, 186));
            gradient.setColorAt(1, QColor(33, 51, 123));

            painter->setBrush(gradient);

            painter->setPen(Qt::NoPen);

            //--------------------------------------------------------------------------------
            if (checkBoxOption->state & State_On) {
                QPainterPath path1;
                path1.addEllipse(point, 6, 6);

                QPainterPath path2;
                path2.addEllipse(point, 5, 5);

                QPainterPath path3;
                path3.addEllipse(point, 3, 3);
                QPainterPath pathTmp = path1.subtracted(path2);
                QPainterPath pathRes = pathTmp.united(path3);
                painter->drawPath(pathRes);
            } else {
                QPainterPath path1;
                path1.addEllipse(point, 6, 6);

                QPainterPath path2;
                path2.addEllipse(point, 5, 5);

                QPainterPath path3;
                path3.addEllipse(point, 3, 3);
                QPainterPath pathTmp = path1.subtracted(path2);
                painter->drawPath(pathTmp);
            }
            painter->restore();
            return;
        }
    } else {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}

} // namespace SimpleRadio
} // namespace Ui
