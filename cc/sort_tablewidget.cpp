#include "sort_tablewidget.h"

namespace Ui {
namespace SortTable {
SimpleTableWidget::SimpleTableWidget(QWidget *parent)
    : QTableWidget(parent)
    , pointLast_(0, 0)
    , nStepTmp_(0)
    , nValueTmp_(0)
    , bMoving_(false)
    , bPressed_(false)
{
    this->installEventFilter(this);
    this->viewport()->installEventFilter(this);
    this->horizontalHeader()->installEventFilter(this);
    this->verticalHeader()->installEventFilter(this);
    this->verticalScrollBar()->installEventFilter(this);

    this->setGridStyle(Qt::NoPen);

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    verticalScrollBar()->setStyle(&styleScrollbar_);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    this->setStyleSheet("QTableWidget QTableCornerButton {"
                        "background-color: rgb(235, 243, 255);"
                        "}");
    this->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                            "border-top:0px solid rgb(174, 184, 220);"
                                            "border-left:0px solid rgb(174, 184, 220);"
                                            "border-right:0px solid rgb(174, 184, 220);"
                                            "border-bottom: -0.5px solid rgb(174, 184, 220);"
                                            "background-color: rgb(235, 243, 255);"
                                            "}");
    this->verticalHeader()->setStyleSheet("QHeaderView::section{"
                                          "border-top:0px solid rgb(174, 184, 220);"
                                          "border-left:0px solid rgb(174, 184, 220);"
                                          "border-right:0px solid rgb(174, 184, 220);"
                                          "border-bottom: 0px solid rgb(174, 184, 220);"
                                          "background-color: rgb(235, 243, 255);"
                                          "}");

    // 1.
    SimpleTableWidgetStyle *pStyleTable = &StyleInstance<SimpleTableWidgetStyle>::GetIntance();
    this->setStyle(pStyleTable);

    // 2.
    SimpleTableHeaderProxyStyle *pStyleHeader = &StyleInstance<SimpleTableHeaderProxyStyle>::GetIntance();
    this->horizontalHeader()->setStyle(pStyleHeader);
    this->verticalHeader()->setStyle(pStyleHeader);

    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, [&]([[maybe_unused]] int value) {
        this->viewport()->update();
        this->horizontalHeader()->viewport()->update();
        this->verticalHeader()->viewport()->update();

        this->horizontalHeader()->update();
        this->verticalHeader()->update();
        this->update();
    });
}

bool SimpleTableWidget::viewportEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Resize: {
        QTableWidget::viewportEvent(event);
        return true;
    }
    case QEvent::Scroll:
        this->viewport()->update();
        return true;
    case QEvent::Gesture:
        this->viewport()->update();
        return true;
    case QEvent::MouseButtonPress: {
        QTableWidget::viewportEvent(event);
        bPressed_ = true;
        QMouseEvent *tmp = ((QMouseEvent *)event);
        pointLast_ = tmp->pos();
        nValueTmp_ = verticalScrollBar()->value();

        this->viewport()->update();
        this->horizontalHeader()->viewport()->update();
        this->verticalHeader()->viewport()->update();

        this->horizontalHeader()->update();
        this->verticalHeader()->update();
        this->update();
        return true;
    }
    case QEvent::MouseMove: {
        QTableWidget::viewportEvent(event);
        QMouseEvent *tmp = ((QMouseEvent *)event);
        if (bPressed_) {
            bMoving_ = true;
            this->setSelectionMode(QTableWidget::NoSelection);
            nStepTmp_ = pointLast_.y() - tmp->y();
            int nValue = nStepTmp_;
            this->verticalScrollBar()->setValue(nValueTmp_ + nValue);
            this->viewport()->update();
            this->horizontalHeader()->viewport()->update();
            this->verticalHeader()->viewport()->update();

            this->horizontalHeader()->update();
            this->verticalHeader()->update();
            this->update();
        }
        return false;
    }
    case QEvent::MouseButtonRelease: {
        QTableWidget::viewportEvent(event);
        bPressed_ = false;
        if (bMoving_) {
            this->setSelectionMode(QTableWidget::ExtendedSelection);
            nStepTmp_ = 0;
            nValueTmp_ = verticalScrollBar()->value();
            this->viewport()->update();
            this->horizontalHeader()->viewport()->update();
            this->verticalHeader()->viewport()->update();

            this->horizontalHeader()->update();
            this->verticalHeader()->update();
            this->update();
        }
        return true;
    }
    default:
        return QTableWidget::viewportEvent(event);
    }
}

void SimpleTableWidget::AutoSetMask()
{
    bool bVisible0 = this->verticalScrollBar()->isVisible();
    bool bVisible1 = this->verticalHeader()->isVisible();
    int nRes = 0;
    nRes |= bVisible0;
    nRes |= (bVisible1 << 1);

    qreal radius1 = 10;
    QRectF rect0(this->verticalScrollBar()->rect());
    QRectF rect1(this->verticalHeader()->rect());
    QRectF rect2(this->horizontalHeader()->rect());
    QRectF rect3(this->viewport()->rect());

    switch (nRes) {
    case 0: {
        QPainterPath path1;
        path1.moveTo(rect2.bottomLeft());
        path1.lineTo(rect2.bottomRight());
        path1.lineTo(rect2.topRight().x(), rect2.topRight().y() + radius1);
        path1.arcTo(
            QRectF(QPoint(rect2.topRight().x() - (radius1 * 2), rect2.topRight().y()), QSize(radius1 * 2, radius1 * 2)),
            0, 90);

        path1.lineTo(rect2.topLeft().x() + radius1, rect2.topLeft().y());
        path1.arcTo(QRectF(QPoint(rect2.topLeft().x(), rect2.topLeft().y()), QSize(radius1 * 2, radius1 * 2)), 90, 90);
        path1.closeSubpath();
        this->horizontalHeader()->viewport()->setMask(path1.toFillPolygon().toPolygon());
        this->horizontalHeader()->setMask(path1.toFillPolygon().toPolygon());

        QPainterPath path2;
        path2.moveTo(rect3.topRight());
        path2.lineTo(rect3.topLeft());
        path2.lineTo(rect3.bottomLeft().x(), rect3.bottomLeft().y() - radius1);
        path2.arcTo(QRectF(QPoint(rect3.bottomLeft().x(), rect3.bottomLeft().y() - radius1 * 2),
                           QSize(radius1 * 2, radius1 * 2)),
                    180, 90);

        path2.lineTo(rect3.bottomRight().x() - radius1, rect3.bottomRight().y());
        path2.arcTo(QRectF(QPoint(rect3.bottomRight().x() - radius1 * 2, rect3.bottomRight().y() - radius1 * 2),
                           QSize(radius1 * 2, radius1 * 2)),
                    270, 90);
        path2.closeSubpath();
        this->viewport()->setMask(path2.toFillPolygon().toPolygon());

        break;
    }

    case 1: {

        QPainterPath path1;
        path1.moveTo(rect2.bottomLeft());
        path1.lineTo(rect2.bottomRight());
        path1.lineTo(rect2.topRight());

        path1.lineTo(rect2.topLeft().x() + radius1, rect2.topLeft().y());
        path1.arcTo(QRectF(QPoint(rect2.topLeft().x(), rect2.topLeft().y()), QSize(radius1 * 2, radius1 * 2)), 90, 90);
        path1.closeSubpath();
        this->horizontalHeader()->viewport()->setMask(path1.toFillPolygon().toPolygon());
        this->horizontalHeader()->setMask(path1.toFillPolygon().toPolygon());

        QPainterPath path2;
        path2.moveTo(rect3.topRight());
        path2.lineTo(rect3.topLeft());
        path2.lineTo(rect3.bottomLeft().x(), rect3.bottomLeft().y() - radius1);
        path2.arcTo(QRectF(QPoint(rect3.bottomLeft().x(), rect3.bottomLeft().y() - radius1 * 2),
                           QSize(radius1 * 2, radius1 * 2)),
                    180, 90);

        path2.lineTo(rect3.bottomRight());

        path2.closeSubpath();
        this->viewport()->setMask(path2.toFillPolygon().toPolygon());

        QPainterPath path3;
        path3.moveTo(rect0.topLeft());
        path3.lineTo(rect0.bottomLeft());

        path3.lineTo(rect0.bottomRight().x() - radius1, rect0.bottomRight().y());
        path3.arcTo(QRectF(QPoint(rect0.bottomRight().x() - radius1 * 2, rect0.bottomRight().y() - radius1 * 2),
                           QSize(radius1 * 2, radius1 * 2)),
                    270, 90);

        path3.lineTo(rect0.topRight().x(), rect0.topRight().y() + radius1);
        path3.arcTo(
            QRectF(QPoint(rect0.topRight().x() - (radius1 * 2), rect0.topRight().y()), QSize(radius1 * 2, radius1 * 2)),
            0, 90);
        path3.closeSubpath();

        this->verticalScrollBar()->setMask(path3.toFillPolygon().toPolygon());
        break;
    }

    case 2: {
        QPainterPath path1;

        path1.moveTo(rect2.topLeft());
        path1.lineTo(rect2.bottomLeft());
        path1.lineTo(rect2.bottomRight());
        path1.lineTo(rect2.topRight().x(), rect2.topRight().y() + radius1);
        path1.arcTo(
            QRectF(QPoint(rect2.topRight().x() - (radius1 * 2), rect2.topRight().y()), QSize(radius1 * 2, radius1 * 2)),
            0, 90);

        path1.closeSubpath();
        this->horizontalHeader()->viewport()->setMask(path1.toFillPolygon().toPolygon());
        this->horizontalHeader()->setMask(path1.toFillPolygon().toPolygon());

        QPainterPath path2;
        path2.moveTo(rect3.topRight());
        path2.lineTo(rect3.topLeft());
        path2.lineTo(rect3.bottomLeft());
        path2.lineTo(rect3.bottomRight().x() - radius1, rect3.bottomRight().y());
        path2.arcTo(QRectF(QPoint(rect3.bottomRight().x() - radius1 * 2, rect3.bottomRight().y() - radius1 * 2),
                           QSize(radius1 * 2, radius1 * 2)),
                    270, 90);
        path2.closeSubpath();
        this->viewport()->setMask(path2.toFillPolygon().toPolygon());

        QPainterPath path3;
        path3.moveTo(rect1.topLeft());
        path3.lineTo(rect1.bottomLeft().x(), rect1.bottomLeft().y() - radius1);
        path3.arcTo(QRectF(QPoint(rect1.bottomLeft().x(), rect1.bottomLeft().y() - radius1 * 2),
                           QSize(radius1 * 2, radius1 * 2)),
                    180, 90);
        path3.lineTo(rect1.bottomRight());
        path3.lineTo(rect1.topRight());
        path3.closeSubpath();
        this->verticalHeader()->viewport()->setMask(path3.toFillPolygon().toPolygon());
        this->verticalHeader()->setMask(path3.toFillPolygon().toPolygon());

        break;
    }
    case 3: {

        QPainterPath path2;
        path2.moveTo(rect1.topLeft());
        path2.lineTo(rect1.bottomLeft().x(), rect1.bottomLeft().y() - radius1);
        path2.arcTo(QRectF(QPoint(rect1.bottomLeft().x(), rect1.bottomLeft().y() - radius1 * 2),
                           QSize(radius1 * 2, radius1 * 2)),
                    180, 90);
        path2.lineTo(rect1.bottomRight());
        path2.lineTo(rect1.topRight());
        path2.closeSubpath();
        this->verticalHeader()->viewport()->setMask(path2.toFillPolygon().toPolygon());
        this->verticalHeader()->setMask(path2.toFillPolygon().toPolygon());

        QPainterPath path3;
        path3.moveTo(rect0.topLeft());
        path3.lineTo(rect0.bottomLeft());

        path3.lineTo(rect0.bottomRight().x() - radius1, rect0.bottomRight().y());
        path3.arcTo(QRectF(QPoint(rect0.bottomRight().x() - radius1 * 2, rect0.bottomRight().y() - radius1 * 2),
                           QSize(radius1 * 2, radius1 * 2)),
                    270, 90);

        path3.lineTo(rect0.topRight().x(), rect0.topRight().y() + radius1);
        path3.arcTo(
            QRectF(QPoint(rect0.topRight().x() - (radius1 * 2), rect0.topRight().y()), QSize(radius1 * 2, radius1 * 2)),
            0, 90);
        path3.closeSubpath();

        this->verticalScrollBar()->setMask(path3.toFillPolygon().toPolygon());
        break;
    }
    default:
        return;
    }

    return;
}

// zhc add
bool SimpleTableWidget::IsRowSelected(int row)
{
    QItemSelectionModel *selectionModel = this->selectionModel();
    return selectionModel->isRowSelected(row, QModelIndex());
}

bool SimpleTableWidget::eventFilter(QObject *watched, QEvent *event)
{
    QTableWidget::eventFilter(watched, event);
    if (watched == this->horizontalHeader()) {
        if (QEvent::Show == event->type()) {
            AutoSetMask();
            return false;
        }
    }

    if (watched == this->verticalHeader()) {
        if (QEvent::Resize == event->type()) {
            AutoSetMask();
            return false;
        }
    }

    if (watched == this->verticalScrollBar()) {
        if (QEvent::Show == event->type()) {
            AutoSetMask();
            return false;
        }
    }

    return false;
}

void SimpleTableWidget::resizeEvent(QResizeEvent *event)
{
    QTableWidget::resizeEvent(event);

    AutoSetMask();

    return;
}

// zhc add
void SimpleTableWidget::mousePressEvent(QMouseEvent *event)
{
    // 获取鼠标点击位置的行
    QPoint pos = event->pos();
    QTableWidgetItem *item = itemAt(pos);
    if (item) {
        int row = rowAt(pos.y());

        // 检查该行是否已选中
        if (IsRowSelected(row)) {
            // 如果是已选中的行，重新单击则取消选择
            clearSelection();
        } else {
            // 调用基类的 mousePressEvent 以保持原有的选择功能
            QTableWidget::mousePressEvent(event);
        }
    } else {
        // 调用基类的 mousePressEvent以处理未选择单格的点击
        QTableWidget::mousePressEvent(event);
    }
}

void SimpleTableWidget::SetVerticalHeaderHide()
{
    this->verticalHeader()->setVisible(false);
    QRectF rct(this->horizontalHeader()->rect());
    QPainterPath path;

    qreal radius = 10;
    path.moveTo(rct.topLeft().x() + radius, rct.topLeft().y());

    path.arcTo(QRectF(rct.topLeft(), QSize(radius * 2, radius * 2)), 90, 90);
    path.lineTo(rct.bottomLeft());
    path.lineTo(rct.bottomRight());
    path.lineTo(rct.topRight().x(), rct.topRight().y() + radius);

    path.arcTo(QRectF(QPoint(rct.topRight().x() - (radius * 2), rct.topRight().y()), QSize(radius * 2, radius * 2)), 0,
               90);

    path.closeSubpath();
    this->horizontalHeader()->setMask(path.toFillPolygon().toPolygon());

    QRectF rct2(this->verticalHeader()->rect());
    QPainterPath path2;

    qreal radius2 = 10;

    path2.moveTo(rct2.topLeft().x() + radius2, rct2.topLeft().y());
    path2.arcTo(QRectF(rct.topLeft(), QSize(radius * 2, radius * 2)), 90, 90);
    path2.lineTo(rct2.bottomLeft().x(), rct2.bottomLeft().y() - radius2);
    path2.arcTo(
        QRectF(QPoint(rct2.bottomLeft().x(), rct2.bottomLeft().y() - (radius2 * 2)), QSize(radius2 * 2, radius2 * 2)),
        180, 90);

    path2.lineTo(rct2.bottomRight());
    path2.lineTo(rct2.topRight());
    path2.closeSubpath();
    this->verticalHeader()->setMask(path2.toFillPolygon().toPolygon());
}

void SimpleTableWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.setBrush(QColor(235, 243, 255, 255));
    if (verticalHeader()->isHidden()) {
        painter.setPen(QPen(QColor(174, 184, 220), 1));
        QRectF rct3(this->viewport()->rect());
        rct3.adjust(-1, 0, 1, 1);
        QPainterPath pathMask;
        qreal radius3 = 10;
        pathMask.moveTo(rct3.topLeft());

        pathMask.lineTo(rct3.bottomLeft().x(), rct3.bottomLeft().y() - radius3);
        pathMask.arcTo(QRectF(QPoint(rct3.bottomLeft().x(), rct3.bottomLeft().y() - (radius3 * 2)),
                              QSize(radius3 * 2, radius3 * 2)),
                       180, 90);

        pathMask.lineTo(rct3.bottomRight().x() - radius3, rct3.bottomRight().y());
        pathMask.arcTo(QRectF(QPoint(rct3.bottomRight().x() - (radius3 * 2), rct3.bottomRight().y() - (radius3 * 2)),
                              QSize(radius3 * 2, radius3 * 2)),
                       270, 90);

        pathMask.lineTo(rct3.topRight());

        painter.setClipPath(pathMask);
        painter.drawPath(pathMask);
    } else {
        QPainterPath path;
        path.addRect(rect());
        painter.setPen(QPen(QColor(174, 184, 220), 1));
        painter.drawPath(path);
    }

    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);

    QTableWidget::paintEvent(event);
}

//----------------------------------------------------------------------------------------------------------------------
void SimpleTableHeaderProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                              const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (element == QStyle::CE_HeaderLabel) {
        const QHeaderView *headerTmp = qobject_cast<const QHeaderView *>(widget);
        const QStyleOptionHeader *headerOption = qstyleoption_cast<const QStyleOptionHeader *>(option);

        if (headerOption && headerTmp) {
            QRect rect = option->rect;
            painter->setPen(QPen(QColor(174, 184, 220), 1));

            if (rect.x() < 2) {
                const QTableWidget *tble = qobject_cast<const QTableWidget *>(widget->parentWidget());
                if (tble) {
                    if (tble->verticalHeader()->isHidden()) {
                        qreal fR = 10;
                        QPainterPath path;
                        path.moveTo(-1 + fR, -1);
                        path.arcTo(QRectF(-1, -1, 2 * fR, 2 * fR), 90, 90);
                        painter->drawPath(path);
                    }
                }
            }

            painter->drawLine(headerOption->rect.bottomLeft(), headerOption->rect.bottomRight());
            painter->drawLine(headerOption->rect.topRight(), headerOption->rect.bottomRight());

            painter->setPen(QPen(QColor(23, 44, 124), 1));
            QFont tmp = headerTmp->font();
            tmp.setPixelSize(12);
            painter->setFont(tmp);
            painter->drawText(rect, Qt::AlignCenter, headerOption->text);
            return;
        }
    } else if (CE_HeaderEmptyArea == element) {
        painter->save();

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(235, 243, 255));
        painter->drawRect(option->rect);

        painter->restore();
        return;
    } else {
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}
//-------------------------------------------------------------------------------------------------------------

void SimpleTableWidgetStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                           const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (element == PE_Frame) {
        QRect rect = widget->rect();

        QPainterPath path;
        path.addRoundedRect(rect, 10, 10);
        painter->setClipPath(path);
        painter->setPen(QPen(QColor(174, 184, 220), 1));
        painter->setBrush(QColor(235, 243, 255));
        painter->drawPath(path);
    } else if (element == PE_PanelItemViewRow) {

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(235, 243, 255));
        painter->drawRect(option->rect);
        return QProxyStyle::drawPrimitive(element, option, painter, widget);

    } else {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}

void SimpleTableWidgetStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                         const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (element == CE_ShapedFrame) {
        drawPrimitive(PE_Frame, option, painter, widget);
    } else if (element == CE_HeaderSection) {
        const QStyleOptionHeader *headerOption = qstyleoption_cast<const QStyleOptionHeader *>(option);
        if (headerOption && headerOption->position == QStyleOptionHeader::OnlyOneSection) {

            const QTableWidget *tbl = qobject_cast<const QTableWidget *>(widget->parentWidget());
            QPainterPath path;
            path.addRoundedRect(tbl->rect(), 10, 10);
            painter->setClipPath(path);

            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(235, 243, 255));
            painter->drawPath(path);

            painter->setPen(QPen(QColor(174, 184, 220), 1));
            painter->setFont(QFont("Arial", 14));
            painter->setBrush(Qt::NoBrush);
            painter->drawLine(headerOption->rect.bottomLeft(), headerOption->rect.bottomRight());
            painter->drawLine(headerOption->rect.topRight(), headerOption->rect.bottomRight());
            return;
        }
    } else if (element == QStyle::CE_ItemViewItem) {
        const QStyleOptionViewItem *itemOption = qstyleoption_cast<const QStyleOptionViewItem *>(option);
        if (itemOption) {
            const QStyleOptionViewItem *optItem = qstyleoption_cast<const QStyleOptionViewItem *>(option);

            painter->setPen(QPen(QColor(174, 184, 220), 1));
            painter->setBrush(QColor(235, 243, 255));
            QRect rect = optItem->rect;
            rect.adjust(0, 0, 1, 1);
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
            painter->drawLine(rect.topRight(), rect.bottomRight());

            painter->setPen(QPen(QColor(23, 44, 124), 1));
            painter->drawText(rect, Qt::AlignCenter, optItem->text);
            return;
        }
    } else {
        return QProxyStyle::drawControl(element, option, painter, widget);
    }
}

ScrollTableScrollbarStyle::ScrollTableScrollbarStyle()
    : imgScrollSliderOver_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_scrollbar_slider_over.png")
    , imgScrollSlider_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_scrollbar_slider.png")
    , imgScrollSubLine_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_scrollbar_up.png")
    , imgScrollAddLine_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_scrollbar_down.png"){};

void ScrollTableScrollbarStyle::drawComplexControl(ComplexControl element, const QStyleOptionComplex *option,
                                                   QPainter *painter, const QWidget *widget) const

{
    if (element == CC_ScrollBar && option) {
        painter->save();
        QRect rectOption = option->rect;
        qreal radius1 = rectOption.width() / 2;
        QPainterPath path3;
        path3.moveTo(rectOption.topLeft());
        path3.lineTo(rectOption.bottomLeft());

        path3.lineTo(rectOption.bottomRight().x() - radius1, rectOption.bottomRight().y());
        path3.arcTo(
            QRectF(QPoint(rectOption.bottomRight().x() - radius1 * 2, rectOption.bottomRight().y() - radius1 * 2),
                   QSize(radius1 * 2, radius1 * 2)),
            270, 90);

        path3.lineTo(rectOption.topRight().x(), rectOption.topRight().y() + radius1);
        path3.arcTo(QRectF(QPoint(rectOption.topRight().x() - (radius1 * 2), rectOption.topRight().y()),
                           QSize(radius1 * 2, radius1 * 2)),
                    0, 90);
        path3.closeSubpath();
        painter->setPen(QColor(205, 214, 238));
        painter->setBrush(QColor(219, 234, 255));
        painter->drawPath(path3);

        if (option->state & State_MouseOver) {
            if (!imgScrollSliderOver_.isNull()) {
                QRect rcSlider = subControlRect(element, option, SC_ScrollBarSlider, widget);
                rcSlider.adjust(rcSlider.width() / 5, 0, -rcSlider.width() / 5, 0);

                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor(151, 167, 222));
                painter->drawRoundedRect(rcSlider, rcSlider.width() / 2, rcSlider.width() / 2);
            }
            if (!imgScrollSubLine_.isNull()) {
                QRect rcSubLine = subControlRect(element, option, SC_ScrollBarSubLine, widget);

                QImage scaledImage2 = imgScrollSubLine_.scaled(rcSubLine.width(), rcSubLine.height(),
                                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

                painter->drawImage(QRect(rcSubLine.x() + 1, rcSubLine.y(), rcSubLine.width(), rcSubLine.height()),
                                   scaledImage2);
            }
            if (!imgScrollAddLine_.isNull()) {
                QRect rcAddLine = subControlRect(element, option, SC_ScrollBarAddLine, widget);

                QImage scaledImage3 = imgScrollAddLine_.scaled(rcAddLine.width(), rcAddLine.height(),
                                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

                painter->drawImage(QRect(rcAddLine.x() + 1, rcAddLine.y(), rcAddLine.width(), rcAddLine.height()),
                                   scaledImage3);
            }
        } else {
            if (!imgScrollSlider_.isNull()) {
                QRect rcSlider = subControlRect(element, option, SC_ScrollBarSlider, widget);

                rcSlider.adjust(rcSlider.width() / 5, 0, -rcSlider.width() / 5, 0);

                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor(178, 188, 222));
                painter->drawRoundedRect(rcSlider, rcSlider.width() / 2, rcSlider.width() / 2);
            }
        }

        painter->restore();
        return;
    } else {
        return QProxyStyle::drawComplexControl(element, option, painter, widget);
    }
}

} // namespace SortTable
} // namespace Ui
