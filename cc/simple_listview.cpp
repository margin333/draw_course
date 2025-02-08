#include "simple_listview.h"

namespace Ui {
namespace ListView {

SimpleListView::~SimpleListView(){};

SimpleListView::SimpleListView(QWidget *parent)
    : QListView(parent)
    , colorNoSele_(245, 249, 255)
    , colorNormal_(219, 234, 255)
{
    QPalette palette0 = this->palette();
    palette0.setColor(QPalette::Background, colorNormal_);
    this->setBackgroundRole(QPalette::Background);
    this->setPalette(palette0);

    QPalette palette1 = this->viewport()->palette();
    palette1.setColor(QPalette::Background, colorNormal_);
    this->viewport()->setBackgroundRole(QPalette::Background);
    this->viewport()->setPalette(palette1);

    QPalette palette2 = this->verticalScrollBar()->palette();
    palette2.setColor(QPalette::Background, colorNormal_);
    this->verticalScrollBar()->setBackgroundRole(QPalette::Background);
    this->verticalScrollBar()->setPalette(palette2);

    this->setStyle(&style_);

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    if (this->verticalScrollBar()) {
        this->verticalScrollBar()->setStyle(&styleBar_);
        this->verticalScrollBar()->setAttribute(Qt::WA_OpaquePaintEvent, false);
    }
    this->setViewportMargins(0, 10, 0, 10);

    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, [&]([[maybe_unused]] int value) {
        this->update();
        this->viewport()->update();
        this->verticalScrollBar()->update();
    });

    connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, [&]([[maybe_unused]] int value) {
        this->update();
        this->viewport()->update();
        this->verticalScrollBar()->update();
    });
}

void SimpleListView::setSelectionMode(QAbstractItemView::SelectionMode mode)
{
    QListView::setSelectionMode(mode);
    QPalette palette0 = this->palette();
    QPalette palette1 = this->viewport()->palette();
    QPalette palette2 = this->verticalScrollBar()->palette();
    if (this->selectionMode() == QAbstractItemView::NoSelection) {
        palette0.setColor(QPalette::Background, colorNoSele_);
        palette1.setColor(QPalette::Background, colorNoSele_);
        palette2.setColor(QPalette::Background, colorNoSele_);
    } else {
        palette0.setColor(QPalette::Background, colorNormal_);
        palette1.setColor(QPalette::Background, colorNormal_);
        palette2.setColor(QPalette::Background, colorNormal_);
    }

    this->setBackgroundRole(QPalette::Background);
    this->setPalette(palette0);
    this->viewport()->setBackgroundRole(QPalette::Background);
    this->viewport()->setPalette(palette1);
    this->verticalScrollBar()->setBackgroundRole(QPalette::Background);
    this->verticalScrollBar()->setPalette(palette2);
}

void SimpleListView::paintEvent(QPaintEvent *e) { return QListView::paintEvent(e); }

void SimpleListView::resizeEvent(QResizeEvent *e)
{
    QListView::resizeEvent(e);
    QPainterPath path;
    path.addRoundedRect(this->rect(), 10, 10);
    this->setMask(path.toFillPolygon().toPolygon());

    QRect rect1 = this->rect();
    rect1.adjust(1, 1, -1, -1);
    QPainterPath path1;
    path1.addRoundedRect(rect1, 10, 10);
    QPoint relativePos0 = viewport()->mapTo(this, viewport()->pos());
    path1.translate(-relativePos0.x(), -relativePos0.y());
    viewport()->setMask(path1.toFillPolygon().toPolygon());

    QPainterPath path2;
    path2.addRoundedRect(rect1, 10, 10);
    QPoint relativePos1 = verticalScrollBar()->mapTo(this, verticalScrollBar()->pos());
    path2.translate(-relativePos1.x(), -relativePos1.y());
    verticalScrollBar()->setMask(path2.toFillPolygon().toPolygon());
}

//------------------------------------------------------------------------------
CommonListStyle::CommonListStyle()
    : imgCheckTrue_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_check_press.png")
    , imgCheckFalse_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_check_enable.png")
{
}

void CommonListStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                  const QWidget *widget) const
{
    switch (element) {
    case QStyle::CE_ShapedFrame: {
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        const QListView *listView = qobject_cast<const QListView *>(option->styleObject);
        if (listView) {
            painter->save();

            QRect tmp = option->rect;
            painter->setPen(QPen(QColor(98, 162, 227), 2));
            painter->setBrush(QColor(98, 162, 227));
            painter->drawRect(widget->rect());

            QPalette palette = listView->palette();

            tmp.adjust(2, 2, -2, -2);
            painter->setPen(QPen(QColor(219, 234, 255), 1));
            painter->setBrush(palette.color(QPalette::Background));
            painter->drawRoundedRect(tmp, 10, 10);

            painter->restore();
        }

        return;
    }
    case QStyle::CE_ItemViewItem: {
        QRect rc0 = subElementRect(SE_ItemViewItemText, option, widget);
        const QStyleOptionViewItem *optionViewItem = qstyleoption_cast<const QStyleOptionViewItem *>(option);

        const QListView *listView = qobject_cast<const QListView *>(widget);
        if (listView == nullptr) {
            return;
        }
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        if ((option->state & State_MouseOver) || (option->state & State_Selected)) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(193, 208, 239));

            painter->drawRect(rc0);
        }
        if (listView->selectionMode() == QListView::MultiSelection) {
            QRect rect = QRect(option->rect.left(), option->rect.top(), option->rect.height(), option->rect.height());
            rect.adjust(3, 3, -3, -3);
            if ((option->state & State_Selected)) {
                painter->drawRect(rc0);
                painter->drawImage(rect, imgCheckTrue_);
            } else {
                painter->drawImage(rect, imgCheckFalse_);
            }
        }

        painter->setPen(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->drawText(rc0, Qt::AlignCenter, optionViewItem->text);

        return;
    }
    default:
        return QProxyStyle::drawControl(element, option, painter, widget);
    }
}

//-ComboScrollbarStyle-------------------------------------------------------------------------
void ComboScrollbarStyle::drawComplexControl(ComplexControl element, const QStyleOptionComplex *option,
                                             QPainter *painter, const QWidget *widget) const

{
    if (element == CC_ScrollBar && option) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QRect rectOption = option->rect;
        painter->setPen(Qt::NoPen);

        const QScrollBar *pScrollBar = qobject_cast<const QScrollBar *>(widget);
        if (!pScrollBar) {
            return;
        }
        QPalette palette = pScrollBar->palette();

        painter->setBrush(palette.color(QPalette::Background));
        painter->drawRect(rectOption);

        painter->setPen(QPen(QColor(98, 162, 227), 2));
        painter->drawLine(0, 0, 0, option->rect.height());

        QRect rcSlider = subControlRect(element, option, SC_ScrollBarSlider, widget);
        rcSlider.adjust(rcSlider.width() / 4, 0, -rcSlider.width() / 4, 0);
        if (option->state & State_MouseOver) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(122, 142, 207));
            painter->drawRoundedRect(rcSlider, rcSlider.width() / 2, rcSlider.width() / 2);

            QRect rcSubLine = subControlRect(element, option, SC_ScrollBarSubLine, widget);
            painter->save();
            painter->translate(rcSubLine.topLeft());
            QPainterPath path1;
            path1.moveTo(rcSubLine.width() / 2, rcSubLine.height() / 5);
            path1.lineTo(rcSubLine.width() / 5, rcSubLine.height() * 4 / 5);
            path1.lineTo(rcSubLine.width() * 4 / 5, rcSubLine.height() * 4 / 5);
            path1.closeSubpath();
            painter->drawPath(path1);
            painter->restore();

            QRect rcAddLine = subControlRect(element, option, SC_ScrollBarAddLine, widget);
            painter->save();
            painter->translate(rcAddLine.topLeft());
            QPainterPath path2;
            path2.moveTo(rcAddLine.width() / 2, rcAddLine.height() * 4 / 5);
            path2.lineTo(rcAddLine.width() / 5, rcAddLine.height() / 5);
            path2.lineTo(rcAddLine.width() * 4 / 5, rcAddLine.height() / 5);
            path2.closeSubpath();
            painter->drawPath(path2);
            painter->restore();

        } else {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(193, 208, 255));
            painter->drawRoundedRect(rcSlider, rcSlider.width() / 2, rcSlider.width() / 2);
        }

        painter->restore();
        return;
    } else {
        return QProxyStyle::drawComplexControl(element, option, painter, widget);
    }
}
} // namespace ListView
} // namespace Ui
