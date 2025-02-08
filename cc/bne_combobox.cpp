#include "bne_combobox.h"
namespace Ui {
namespace BNECombo {
BNEComboBox::BNEComboBox(QWidget *parent)
    : QComboBox(parent)
    , nFlagAlign_(Qt::AlignLeft | Qt::AlignVCenter)
{
    // 1.
    CustomComboBoxStyle *pStyleBox = &StyleInstance<CustomComboBoxStyle>::GetIntance();
    this->setStyle(pStyleBox);

    this->setStyleSheet("QComboBox{combobox-popup:0;}");
    this->setMaxVisibleItems(10);

    // 2.
    ComboListViewStyle *pStyleList = &StyleInstance<ComboListViewStyle>::GetIntance();
    view()->setStyle(pStyleList);
    view()->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    if (view()->verticalScrollBar()) {
        // 3.
        ComboScrollbarStyle *pStyleScroll = &StyleInstance<ComboScrollbarStyle>::GetIntance();
        view()->verticalScrollBar()->setStyle(pStyleScroll);

        view()->verticalScrollBar()->setAttribute(Qt::WA_OpaquePaintEvent, false); // WA_NoBackground
    }
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(219, 234, 255));
    view()->viewport()->setBackgroundRole(QPalette::Background);
    view()->viewport()->setPalette(palette);

    this->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void BNEComboBox::SetAlignment(Qt::Alignment align) { nFlagAlign_ = align; }

Qt::Alignment BNEComboBox::GetAlignment() const { return nFlagAlign_; }

int BNEComboBox::GetCurrentIndexUserData()
{
    if (this->currentData().type() == QVariant::Int)
        return this->currentData().toInt();
    else {
        return -1;
    }
}

void BNEComboBox::SetCurrentIndexByUserData(int nValue)
{
    int nRes = this->findData(nValue);
    if (nRes >= 0 && nRes < this->count()) {
        this->setCurrentIndex(nRes);
    }
    return;
}

void BNEComboBox::showPopup()
{
    QComboBox::showPopup();
    QWidget *widget = findChild<QWidget *>();
    QPalette palette = widget->palette();

    palette.setColor(QPalette::Background, QColor(98, 162, 227));
    widget->setBackgroundRole(QPalette::Background);
    widget->setPalette(palette);

    QPainterPath path;
    path.addRoundedRect(widget->rect().adjusted(1, 1, -1, -1), 10, 10);

    QPoint gp = this->mapToGlobal(this->rect().bottomLeft());
    gp.setY(gp.y() + 3);
    widget->move(gp);
    widget->setMask(path.toFillPolygon().toPolygon());
    widget->setAutoFillBackground(true);
}

void BNEComboBox::paintEvent(QPaintEvent *event)
{
    QComboBox::paintEvent(event);

    QPainterPath path;
    path.addRoundedRect(rect().adjusted(1, 1, -1, -1), height() / 3, height() / 3);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(98, 162, 227), 2));
    painter.drawPath(path);
};

void BNEComboBox::resizeEvent(QResizeEvent *event)
{
    QComboBox::resizeEvent(event);
    QPainterPath path;
    path.addRoundedRect(this->rect(), height() / 3, height() / 3);
    this->setMask(path.toFillPolygon().toPolygon());
};

//----ListViewStyle------------------------------------------------------------------------------------------------------
ComboListViewStyle::ComboListViewStyle() { }

ComboListViewStyle::~ComboListViewStyle() { }

QRect ComboListViewStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    QRect rc = QProxyStyle::subElementRect(element, option, widget);
    if (element == SE_ShapedFrameContents) {
        return rc.adjusted(2, 10, -2, -10);
    } else {
        return QProxyStyle::subElementRect(element, option, widget);
    }
};

void ComboListViewStyle ::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                      const QWidget *widget) const
{
    if (CE_ShapedFrame == element) {
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        const QListView *listView = qobject_cast<const QListView *>(option->styleObject);
        if (listView) {
            painter->save();

            QRect tmp = option->rect;

            painter->setPen(QPen(QColor(98, 162, 227), 2));
            painter->setBrush(QColor(98, 162, 227));
            painter->drawRect(widget->rect());

            tmp.adjust(2, 2, -2, -2);
            painter->setPen(QPen(QColor(219, 234, 255), 1));
            painter->setBrush(QColor(219, 234, 255));
            painter->drawRoundedRect(tmp, 10, 10);

            painter->setPen(QPen(QColor(98, 162, 227), 2));
            painter->drawLine(widget->x(), 0, widget->x(), widget->rect().height());

            painter->restore();
        }
    } else if (CE_ItemViewItem == element) {
        QRect rc0 = subElementRect(SE_ItemViewItemText, option, widget);
        const QStyleOptionViewItem *optionViewItem = qstyleoption_cast<const QStyleOptionViewItem *>(option);

        QModelIndex tmp = optionViewItem->index;

        if (option->state & State_MouseOver) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(193, 208, 239));

            painter->drawRoundedRect(rc0.adjusted(3, 0, -3, 0), 4, 4, Qt::AbsoluteSize);
        }

        painter->setPen(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->drawText(rc0, Qt::AlignCenter, optionViewItem->text);

    } else {
        return QProxyStyle::drawControl(element, option, painter, widget);
    }
}

//-ComboListViewStyle--------------------------------------------------------------------------
CustomComboListView::CustomComboListView(QWidget *parent)
    : QListView(parent)
{
    this->setStyle(&styleListview_);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    if (this->verticalScrollBar()) {
        this->verticalScrollBar()->setStyle(&styleScrollbar_);

        this->verticalScrollBar()->setAttribute(Qt::WA_OpaquePaintEvent, false); // WA_NoBackground
    }
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(219, 234, 255));
    this->viewport()->setBackgroundRole(QPalette::Background);
    this->viewport()->setPalette(palette);
};

void CustomComboListView::resizeEvent(QResizeEvent *event)
{
    QListView::resizeEvent(event);

    QPainterPath path;
    path.addRoundedRect(this->rect(), 10, 10);
    this->setMask(path.toFillPolygon().toPolygon());
};

//-ComboScrollbarStyle-------------------------------------------------------------------------
ComboScrollbarStyle::ComboScrollbarStyle()
    : imgScrollSliderOver_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_scrollbar_slider_over.png")
    , imgScrollSlider_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_scrollbar_slider.png")
    , imgScrollSubLine_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_scrollbar_up.png")
    , imgScrollAddLine_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_scrollbar_down.png"){};

QRect ComboScrollbarStyle::subControlRect(ComplexControl element, const QStyleOptionComplex *option,
                                          SubControl whichSubControl, const QWidget *widget) const
{
    return QProxyStyle::subControlRect(element, option, whichSubControl, widget);
};

void ComboScrollbarStyle::drawComplexControl(ComplexControl element, const QStyleOptionComplex *option,
                                             QPainter *painter, const QWidget *widget) const
{
    if (element == CC_ScrollBar && option) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QRect rectOption = option->rect;
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(219, 234, 255));
        painter->drawRect(rectOption);
        if (option->state & State_MouseOver) {
            QRect rcSlider = subControlRect(element, option, SC_ScrollBarSlider, widget);
            rcSlider.adjust(2, 0, -2, 0);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(151, 167, 222));
            painter->drawRoundedRect(rcSlider, rcSlider.width() / 2, rcSlider.width() / 2);

            QRect rcSlider1 = subControlRect(element, option, SC_ScrollBarSlider, widget);
            painter->save();
            QRect rcSubLine = subControlRect(element, option, SC_ScrollBarSubLine, widget);
            painter->translate(rcSlider1.width() / 2, rcSubLine.y());
            QPainterPath pathSubLine;
            pathSubLine.moveTo(0, 0);
            pathSubLine.lineTo(-rcSlider1.width() / 2 + 2, rcSubLine.height() * 3 / 4);
            pathSubLine.lineTo(rcSlider1.width() / 2 - 2, rcSubLine.height() * 3 / 4);
            pathSubLine.closeSubpath();
            pathSubLine.translate(0, 2);
            painter->drawPath(pathSubLine);

            painter->restore();

            painter->save();
            QRect rcAddLine = subControlRect(element, option, SC_ScrollBarAddLine, widget);
            painter->translate(rcSlider1.width() / 2, rcAddLine.y());
            QPainterPath pathAddLine;

            pathAddLine.moveTo(-rcSlider1.width() / 2 + 2, rcAddLine.height() / 4);
            pathAddLine.lineTo(rcSlider1.width() / 2 - 2, rcAddLine.height() / 4);
            pathAddLine.lineTo(0, rcAddLine.height() * 4 / 4);
            pathAddLine.closeSubpath();
            pathSubLine.translate(0, -2);
            painter->drawPath(pathAddLine);

            painter->restore();

        } else {
            QRect rcSlider1 = subControlRect(element, option, SC_ScrollBarSlider, widget);
            rcSlider1.adjust(2, 0, -2, 0);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(178, 188, 222));
            painter->drawRoundedRect(rcSlider1, rcSlider1.width() / 2, rcSlider1.width() / 2);
        }
        painter->restore();
        return;
    } else {
        return QProxyStyle::drawComplexControl(element, option, painter, widget);
    }
}

//---------------------------------------------------------------------------------------------------
CustomComboBoxStyle::CustomComboBoxStyle() { }

void CustomComboBoxStyle::drawComplexControl(ComplexControl which, const QStyleOptionComplex *option, QPainter *painter,
                                             const QWidget *widget) const
{
    if (which == CC_ComboBox) {
        const QStyleOptionComboBox *cbOption = qstyleoption_cast<const QStyleOptionComboBox *>(option);
        if (cbOption) {
            painter->setRenderHints(QPainter::Antialiasing);
            painter->save();
            QRect rect = subControlRect(CC_ComboBox, option, SC_ComboBoxFrame).adjusted(0, 0, 0, 0);
            painter->setPen(Qt::transparent);
            painter->setBrush(QColor(219, 234, 255));
            painter->drawRect(rect);

            painter->restore();
            rect = subControlRect(CC_ComboBox, option, SC_ComboBoxArrow).adjusted(0, 0, 0, 0);

            QStyleOption arrowOpt(*cbOption);
            arrowOpt.rect = rect;
            drawPrimitive(PE_IndicatorArrowDown, &arrowOpt, painter);
        }

    } else {
        return QProxyStyle::drawComplexControl(which, option, painter, widget);
    }
}

void CustomComboBoxStyle::drawPrimitive(PrimitiveElement which, const QStyleOption *option, QPainter *painter,
                                        const QWidget *widget) const
{
    switch (which) {
    case PE_IndicatorArrowDown: {

        painter->setRenderHints(QPainter::Antialiasing);

        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(219, 234, 255));
        painter->drawRect(option->rect);
        painter->restore();

        painter->save();
        painter->translate(option->rect.x(), option->rect.y());
        QPainterPath drawtriangle;

        if ((option->state & QStyle::State_Enabled) && (option->state & QStyle::State_On) &&
            (option->state & QStyle::State_MouseOver) && (option->state & QStyle::State_Active)) {
            drawtriangle.moveTo(option->rect.width() / 3, option->rect.height() * 2 / 3);
            drawtriangle.lineTo(option->rect.width() / 2, option->rect.height() * 1 / 3);
            drawtriangle.lineTo(option->rect.width() * 2 / 3, option->rect.height() * 2 / 3);

        } else {
            drawtriangle.moveTo(option->rect.width() / 3, option->rect.height() / 3);
            drawtriangle.lineTo(option->rect.width() / 2, option->rect.height() * 2 / 3);
            drawtriangle.lineTo(option->rect.width() * 2 / 3, option->rect.height() / 3);
        }
        painter->setPen(QPen(QColor(157, 167, 222), 2));
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(drawtriangle);

        //----------------------------------------------------------------
        painter->restore();
        break;
    }
    default:
        QProxyStyle::drawPrimitive(which, option, painter, widget);
    }
}

QRect CustomComboBoxStyle::subControlRect(ComplexControl whichControl, const QStyleOptionComplex *option,
                                          SubControl whichSubControl, const QWidget *widget) const
{
    if (whichControl == CC_ComboBox && option) {
        switch (whichSubControl) {
        case SC_ComboBoxFrame:
            return QRect(0, 0, option->rect.width() - option->rect.height(), option->rect.height());
        case SC_ComboBoxArrow:
            return QRect(option->rect.width() - option->rect.height(), 0, option->rect.height(), option->rect.height());
        default:
            return QProxyStyle::subControlRect(whichControl, option, whichSubControl, widget);
        }
    } else {
        return QProxyStyle::subControlRect(whichControl, option, whichSubControl, widget);
    }
}

void CustomComboBoxStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                      const QWidget *widget) const
{
    if (element == CE_ComboBoxLabel) {
        const BNEComboBox *pCombo = static_cast<const BNEComboBox *>(widget);
        const QStyleOptionComboBox *comboBoxOption = qstyleoption_cast<const QStyleOptionComboBox *>(option);
        if (comboBoxOption && pCombo) {
            QRect textRect = comboBoxOption->rect;
            textRect.adjust(3, 0, -1 * option->rect.height(), 0);
            painter->setPen(Qt::black);
            painter->setBrush(Qt::NoBrush);
            painter->drawText(textRect, pCombo->GetAlignment(), comboBoxOption->currentText);
            return;
        } else {
            return QProxyStyle::drawControl(element, option, painter, widget);
        }
    }

    return QProxyStyle::drawControl(element, option, painter, widget);
}
} // namespace BNECombo
} // namespace Ui
