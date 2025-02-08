#include "bne_pathtable.h"

namespace Ui {
namespace PathTable {
PathTableWidget::PathTableWidget(PathMode mode, QWidget *parent)
    : QTableWidget(parent)
    , nMode_(mode)
    , bPress_(false)
    , bMoving_(false)
{
    this->setGridStyle(Qt::NoPen);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    QScroller::grabGesture(viewport(), QScroller::LeftMouseButtonGesture);

    this->setStyleSheet("QTableWidget QTableCornerButton {"
                        "background-color: rgb(235, 243, 255);"
                        "}");
    this->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                            "border-top:0px solid rgb(172, 182, 219);"
                                            "border-left:0px solid rgb(172, 182, 219);"
                                            "border-right:0px solid rgb(172, 182, 219);"
                                            "border-bottom: -0.5px solid rgb(172, 182, 219);"
                                            "background-color: rgb(235, 243, 255);"
                                            "}");
    this->verticalHeader()->setStyleSheet("QHeaderView::section{"
                                          "border-top:0px solid rgb(172, 182, 219);"
                                          "border-left:0px solid rgb(172, 182, 219);"
                                          "border-right:0px solid rgb(172, 182, 219);"
                                          "border-bottom: 0px solid rgb(172, 182, 219);"
                                          "background-color: rgb(235, 243, 255);"
                                          "}");

    // 1.
    this->setStyle(&styleTable);

    // 2.
    this->horizontalHeader()->setStyle(&styleHeaderHor_);
    this->verticalHeader()->setStyle(&styleHeaderVer_);
    listHoriHeader_ << "xCoor."
                    << "yCoor."
                    << "xOffSet"
                    << "yOffSet";
    this->setColumnCount(listHoriHeader_.count());
    this->setHorizontalHeaderLabels(listHoriHeader_);

    this->setRowCount(4);

    for (int i = 0; i < 4; i++) {
        QStandardItem *item1 = new QStandardItem("QString::number(i)");
        item1->setData(i, 1);
        modelDIAGONAL_.appendRow(item1);
    }

    for (int i = 0; i < 4; i++) {
        QStandardItem *item1 = new QStandardItem("QString::number(i)");
        item1->setData(i + 4, 1);
        modelRHOMBUS_.appendRow(item1);
    }

    switch (mode) {
    case RHOMBUS:
        this->verticalHeader()->setModel(&modelRHOMBUS_);
        break;
    case DIAGONAL:
        this->verticalHeader()->setModel(&modelDIAGONAL_);
        break;
    }

    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void PathTableWidget::ChangeMode(PathMode mode)
{
    switch (mode) {
    case RHOMBUS:
        this->verticalHeader()->setModel(&modelRHOMBUS_);
        break;
    case DIAGONAL:
        this->verticalHeader()->setModel(&modelDIAGONAL_);
        break;
    }
    this->setHorizontalHeaderLabels(listHoriHeader_);
    this->update();
    this->viewport()->update();
    this->horizontalHeader()->update();
    this->horizontalHeader()->viewport()->update();
}

bool PathTableWidget::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::Scroll) {
        this->update();
        this->viewport()->update();
        this->horizontalHeader()->update();
        this->horizontalHeader()->viewport()->update();
    }
    if (event->type() == QEvent::Gesture) {
        this->update();
        this->viewport()->update();
        this->horizontalHeader()->update();
        this->horizontalHeader()->viewport()->update();
        bMoving_ = false;
        bPress_ = false;
    }

    return QTableWidget::viewportEvent(event);
}

void PathTableWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.setBrush(QColor(235, 243, 255, 255));
    if (verticalHeader()->isHidden()) {
        painter.setPen(QPen(QColor(172, 182, 219), 1));
        QRectF rct3(this->viewport()->rect());
        rct3.adjust(-1, 0, 1, 1);
        QPainterPath pathMask;
        qreal radius3 = rect().width() / 30;
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
        painter.setPen(Qt::NoPen);
        painter.drawPath(path);
    }

    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);

    QTableWidget::paintEvent(event);
}

void PathTableWidget::resizeEvent(QResizeEvent *event)
{
    this->verticalHeader()->setFixedWidth(width() / 5);
    QTableWidget::resizeEvent(event);

    //-----------------horizontalHeader---------------------------
    QRectF rct(this->horizontalHeader()->rect());
    QPainterPath path;

    qreal radius = rect().width() / 30;

    path.moveTo(rct.topLeft().x(), rct.topLeft().y());

    path.lineTo(rct.bottomLeft());
    path.lineTo(rct.bottomRight());
    path.lineTo(rct.topRight().x(), rct.topRight().y() + radius);

    path.arcTo(QRectF(QPoint(rct.topRight().x() - (radius * 2), rct.topRight().y()), QSize(radius * 2, radius * 2)), 0,
               90);
    path.lineTo(rct.topLeft().x() + radius, rct.topLeft().y());
    path.closeSubpath();
    this->horizontalHeader()->setMask(path.toFillPolygon().toPolygon());

    //-----------------verticalHeader---------------------------
    QRectF rct2(this->verticalHeader()->rect());
    QPainterPath path2;

    qreal radius2 = rect().width() / 30;

    path2.moveTo(rct2.topLeft().x(), rct2.topLeft().y());

    path2.lineTo(rct2.bottomLeft().x(), rct2.bottomLeft().y() - radius2);
    path2.arcTo(
        QRectF(QPoint(rct2.bottomLeft().x(), rct2.bottomLeft().y() - (radius2 * 2)), QSize(radius2 * 2, radius2 * 2)),
        180, 90);

    path2.lineTo(rct2.bottomRight());
    path2.lineTo(rct2.topRight());
    path2.closeSubpath();
    this->verticalHeader()->setMask(path2.toFillPolygon().toPolygon());

    //------------------viewport--------------------------------
    QRectF rct3(this->viewport()->rect());
    QPainterPath path3;
    qreal radius3 = rect().width() / 30;
    path3.moveTo(rct3.topLeft());

    path3.lineTo(rct3.bottomLeft());

    path3.lineTo(rct3.bottomRight().x() - radius3, rct3.bottomRight().y());
    path3.arcTo(QRectF(QPoint(rct3.bottomRight().x() - (radius3 * 2), rct3.bottomRight().y() - (radius3 * 2)),
                       QSize(radius3 * 2, radius3 * 2)),
                270, 90);

    path3.lineTo(rct3.topRight());
    path3.closeSubpath();
    this->viewport()->setMask(path3.toFillPolygon().toPolygon());
}
//---------------------------------------------------------------------------------------------------------------------
void PathTableHeaderStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                       const QWidget *widget) const
{

    if (element == QStyle::CE_HeaderLabel) {
        const QHeaderView *headerTmp = qobject_cast<const QHeaderView *>(widget);
        const QStyleOptionHeader *headerOption = qstyleoption_cast<const QStyleOptionHeader *>(option);

        if (headerOption && headerTmp) {
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            painter->setPen(QPen(QColor(172, 182, 219), 1));

            painter->drawLine(headerOption->rect.bottomLeft(), headerOption->rect.bottomRight());
            painter->drawLine(headerOption->rect.topRight(), headerOption->rect.bottomRight());

            QModelIndex indexTmp = headerTmp->model()->index(headerOption->section, 0);
            const QAbstractItemModel *modelTmp = indexTmp.model();
            QMap mapTmp = modelTmp->itemData(indexTmp);
            auto it = mapTmp.find(1);
            if (it != mapTmp.end()) {
                QRect rect = option->rect;
                rect.adjust(5, 10, -5, -10);
                painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
                painter->drawRect(rect);

                if (it.value().toInt() >= 0 && it.value().toInt() <= 3) {

                    painter->drawLine(rect.topLeft(), rect.bottomRight());
                    painter->drawLine(rect.bottomLeft(), rect.topRight());

                    painter->setPen(QPen(Qt::red, 3));

                    switch (it.value().toInt()) {
                    case 0: {
                        painter->drawPoint(rect.topLeft());
                        break;
                    }
                    case 1: {
                        painter->drawPoint(rect.topRight());
                        break;
                    }
                    case 2: {
                        painter->drawPoint(rect.bottomRight());
                        break;
                    }
                    case 3: {
                        painter->drawPoint(rect.bottomLeft());
                        break;
                    }
                    }
                } else if (it.value().toInt() >= 4 && it.value().toInt() <= 7) {
                    QPoint p1 = (rect.topLeft() + rect.bottomLeft()) / 2;
                    QPoint p2 = (rect.bottomLeft() + rect.bottomRight()) / 2;
                    QPoint p3 = (rect.bottomRight() + rect.topRight()) / 2;
                    QPoint p4 = (rect.topRight() + rect.topLeft()) / 2;

                    QPainterPath pathRhombus;
                    pathRhombus.moveTo(p1);
                    pathRhombus.lineTo(p2);
                    pathRhombus.lineTo(p3);
                    pathRhombus.lineTo(p4);
                    pathRhombus.closeSubpath();

                    painter->drawPath(pathRhombus);

                    painter->setPen(QPen(Qt::red, 3));
                    switch (it.value().toInt()) {
                    case 4: {
                        painter->drawPoint(p1);
                        break;
                    }
                    case 5: {
                        painter->drawPoint(p4);
                        break;
                    }
                    case 6: {
                        painter->drawPoint(p3);
                        break;
                    }
                    case 7: {
                        painter->drawPoint(p2);
                        break;
                    }
                    }
                }
                return;
            }
        } else if (CE_HeaderEmptyArea == element) {
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            painter->save();

            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(235, 243, 255));
            painter->drawRect(option->rect);

            painter->restore();
            return;
        } else {
            return QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
void SimpleTableHeaderStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                         const QWidget *widget) const
{
    if (element == QStyle::CE_HeaderLabel) {
        const QHeaderView *headerTmp = qobject_cast<const QHeaderView *>(widget);
        const QStyleOptionHeader *headerOption = qstyleoption_cast<const QStyleOptionHeader *>(option);

        if (headerOption && headerTmp) {
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            QRect rect = option->rect;
            painter->setPen(QPen(QColor(172, 182, 219), 1));

            if (rect.x() < 2) {
                const QTableWidget *tble = qobject_cast<const QTableWidget *>(widget->parentWidget());
                if (tble) {
                    if (tble->verticalHeader()->isHidden()) {
                        qreal fR = tble->width() / 30;
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
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
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

void PathTableWidgetStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                         const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (element == PE_Frame) {
        QRect rect = widget->rect();

        QPainterPath path;
        path.addRoundedRect(rect, rect.width() / 30, rect.width() / 30);
        painter->setClipPath(path);
        painter->setPen(QPen(QColor(172, 182, 219), 1));
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

void PathTableWidgetStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
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
            path.addRoundedRect(tbl->rect(), tbl->width() / 30, tbl->width() / 30);
            painter->setClipPath(path);

            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(235, 243, 255));
            painter->drawPath(path);

            painter->setPen(QPen(QColor(172, 182, 219), 1));
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

            painter->setPen(QPen(QColor(172, 182, 219), 1));
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
} // namespace PathTable
} // namespace Ui
