#include "sort_tablewidget_temp.h"

namespace Ui {
namespace SortTableTemp {
SimpleTableTempWidget::SimpleTableTempWidget(QWidget *parent)
    : QTableWidget(parent)
    , bAscend_(true)
    , nLogicalIndex_(-1)
    , pointLast_(0, 0)
    , nStepTmp_(0)
    , nValueTmp_(0)
    , bMoving_(false)
    , bPressed_(false)
{
    this->setGridStyle(Qt::NoPen);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    // QScroller::grabGesture(viewport(), QScroller::LeftMouseButtonGesture);
    //  this->setFlow(QTableWidget::TopToBottom);
    //     horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //     verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setSortIndicatorShown(true);

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
    this->setStyle(&customeWidgetStyle);
    this->horizontalHeader()->setStyle(&customHeaderStyle);
    this->verticalHeader()->setStyle(&customHeaderStyle);

    // QPalette palette = this->viewport()->palette();
    // // palette.setColor(QPalette::Background, QColor(165, 184,
    // 224).lighter(120)); palette.setColor(QPalette::Base, QColor(235, 243,
    // 255));

    // this->viewport()->setPalette(palette);

    connect(this->horizontalHeader(), &QHeaderView::sectionClicked, this, [&](int logicalIndex) {
        // 使用sortItems方法对点击的列进行排序
        int index = listSortHeader_.indexOf(logicalIndex);
        if (index == -1) {
            return;
        }
        if (logicalIndex != nLogicalIndex_) {
            bAscend_ = true;
            nLogicalIndex_ = logicalIndex;
        } else {
            bAscend_ = !bAscend_;
        }
        if (bAscend_) {
            this->horizontalHeader()->setSortIndicator(logicalIndex, Qt::AscendingOrder);
            this->CustomSort(logicalIndex, Qt::AscendingOrder);
        } else {
            this->horizontalHeader()->setSortIndicator(logicalIndex, Qt::DescendingOrder);
            this->CustomSort(logicalIndex, Qt::DescendingOrder);
        }
    });

    // connect(verticalScrollBar(), &QScrollBar::valueChanged, this,
    // [&]([[maybe_unused]] int value) {
    //     this->update(this->rect());
    //     this->viewport()->update(this->viewport()->rect()); // 更新视图
    //     this->horizontalHeader()->update(this->horizontalHeader()->rect());
    //     this->verticalHeader()->update(this->verticalHeader()->rect());
    //     this->horizontalHeader()->viewport()->update(this->horizontalHeader()->rect());
    //     this->verticalHeader()->viewport()->update(this->verticalHeader()->rect());
    // });
}

void SimpleTableTempWidget::SetSortHeaderIndex(QList<int> &&listIndex) { listSortHeader_ = listIndex; }

void SimpleTableTempWidget::UpdateHideColumn(int nHideColumn) { listHideColumn_.append(nHideColumn); }

void SimpleTableTempWidget::SetVerticalHeaderHide()
{
    this->verticalHeader()->setVisible(false); // 隐藏垂直表头
    QRectF rct(this->horizontalHeader()->rect());
    QPainterPath path;
    // 设置圆角半径
    qreal radius = rect().width() / 30;
    // 设置起点为矩形左上圆角圆心
    path.moveTo(rct.topLeft().x() + radius, rct.topLeft().y());
    // 绘制圆角
    // 圆弧以外切圆的90度位置为起点，逆时针画圆弧运行90度结束（从12点钟方向 -
    // 9点钟方向）
    path.arcTo(QRectF(rct.topLeft(), QSize(radius * 2, radius * 2)), 90, 90);
    path.lineTo(rct.bottomLeft());
    path.lineTo(rct.bottomRight());
    path.lineTo(rct.topRight().x(), rct.topRight().y() + radius);
    // 画圆弧 (3点钟方向 - 12点钟方向)
    path.arcTo(QRectF(QPoint(rct.topRight().x() - (radius * 2), rct.topRight().y()), QSize(radius * 2, radius * 2)), 0,
               90);
    // path.lineTo(rct.topLeft().x() + radius, rct.topLeft().y());
    path.closeSubpath();
    this->horizontalHeader()->setMask(path.toFillPolygon().toPolygon());

    QRectF rct2(this->verticalHeader()->rect());
    QPainterPath path2;
    // 设置圆角半径
    qreal radius2 = rect().width() / 30;
    // 设置起点为矩形左上圆角圆心
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

void SimpleTableTempWidget::CustomSort(int column, Qt::SortOrder order)
{
    // 获取要排序的行数
    int rowCount = this->rowCount();

    // 为表格中的所有行创建一个临时的 QList，用来存储每行的内容
    QList<QList<QTableWidgetItem *>> rowData;
    for (int i = 0; i < rowCount; i++) {
        QList<QTableWidgetItem *> currentRow;
        for (int j = 0; j < this->columnCount(); j++) {
            currentRow.append(this->takeItem(i, j));
        }
        rowData.append(currentRow);
    }

    // 根据指定列column，以及排序顺序order，对rowData进行自定义排序
    // 以下示例只演示了简单的整数排序
    if (order == Qt::AscendingOrder) {
        std::stable_sort(rowData.begin(), rowData.end(),
                         [column](const QList<QTableWidgetItem *> &a, const QList<QTableWidgetItem *> &b) {
                             if (a[column] && b[column]) {
                                 return a[column]->text().toInt() < b[column]->text().toInt();
                             } else {
                                 return false;
                             };
                         });
    } else {
        std::stable_sort(rowData.begin(), rowData.end(),
                         [column](const QList<QTableWidgetItem *> &a, const QList<QTableWidgetItem *> &b) {
                             if (a[column] && b[column]) {
                                 return a[column]->text().toInt() > b[column]->text().toInt();
                             } else {
                                 return false;
                             }
                         });
    }

    // 根据排序后的rowData，重新填充表格中的数据
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < this->columnCount(); j++) {
            this->setItem(i, j, rowData[i][j]);
        }
    }
}

void SimpleTableTempWidget::ShowColumn(const int &item)
{
    listHideColumn_.removeOne(item);
    emit itemsChanged(listHideColumn_);
}

void SimpleTableTempWidget::HideColumn(const int &item)
{
    if (this->columnCount() > listHideColumn_.count()) {
        listHideColumn_.append(item);
        emit itemsChanged(listHideColumn_);
    }
}

bool SimpleTableTempWidget::viewportEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Scroll:
        this->viewport()->update();
        return true;
        break;
    case QEvent::Gesture:
        this->viewport()->update();
        return true;
        break;
    case QEvent::MouseButtonPress: {
        QTableWidget::viewportEvent(event);
        bPressed_ = true;
        QMouseEvent *tmp = ((QMouseEvent *)event);
        pointLast_ = tmp->pos();
        nValueTmp_ = verticalScrollBar()->value();

        this->update();
        this->viewport()->update();
        this->horizontalHeader()->update();
        this->verticalHeader()->update();
        break;
    }
    case QEvent::MouseMove: {
        QTableWidget::viewportEvent(event);
        QMouseEvent *tmp = ((QMouseEvent *)event);
        if (bPressed_) {
            bMoving_ = true;

            this->setSelectionMode(QTableWidget::NoSelection);

            nStepTmp_ = (pointLast_.y() - tmp->y()) / 10;
            // int nValue = nStepTmp_ / this->verticalScrollBar()->maximumHeight();
            int nValue = nStepTmp_;

            this->verticalScrollBar()->setValue(nValueTmp_ + nValue);
            this->update();
            this->viewport()->update();
            this->horizontalHeader()->update();
            this->verticalHeader()->update();
        }
        break;
    }
    case QEvent::MouseButtonRelease: {
        QTableWidget::viewportEvent(event);
        bPressed_ = false;
        if (bMoving_) {
            this->setSelectionMode(QTableView::ExtendedSelection);
            nStepTmp_ = 0;
            nValueTmp_ = verticalScrollBar()->value();
        }
        // QMouseEvent *tmp = ((QMouseEvent *)event);

        this->update();
        this->viewport()->update();
        this->horizontalHeader()->update();
        this->verticalHeader()->update();
        break;
    }
    default:
        //        this->viewport()->update();
        return QTableWidget::viewportEvent(event);
    }
}

void SimpleTableTempWidget::paintEvent(QPaintEvent *event)
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
        // pathMask.closeSubpath();
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

void SimpleTableTempWidget::resizeEvent(QResizeEvent *event)
{
    QTableWidget::resizeEvent(event);

    //-----------------horizontalHeader---------------------------
    QRectF rct(this->horizontalHeader()->rect());
    QPainterPath path;
    // 设置圆角半径
    qreal radius = rect().width() / 30;
    // 设置起点为矩形左上圆角圆心
    path.moveTo(rct.topLeft().x(), rct.topLeft().y());
    // 绘制圆角
    // 圆弧以外切圆的90度位置为起点，逆时针画圆弧运行90度结束（从12点钟方向 -
    // 9点钟方向） path.arcTo(QRect(rct.topLeft(), QSize(radius * 2, radius * 2)),
    // 90, 90);
    path.lineTo(rct.bottomLeft());
    path.lineTo(rct.bottomRight());
    path.lineTo(rct.topRight().x(), rct.topRight().y() + radius);
    // 画圆弧 (3点钟方向 - 12点钟方向)
    path.arcTo(QRectF(QPoint(rct.topRight().x() - (radius * 2), rct.topRight().y()), QSize(radius * 2, radius * 2)), 0,
               90);
    path.lineTo(rct.topLeft().x() + radius, rct.topLeft().y());
    path.arcTo(QRectF(QPoint(rct.topLeft().x() - (radius * 2), rct.topLeft().y()), QSize(radius * 2, radius * 2)), 270,
               90);
    path.closeSubpath();
    this->horizontalHeader()->setMask(path.toFillPolygon().toPolygon());

    //-----------------verticalHeader---------------------------
    QRectF rct2(this->verticalHeader()->rect());
    QPainterPath path2;
    // 设置圆角半径
    qreal radius2 = rect().width() / 30;
    // 设置起点为矩形左上圆角圆心
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
    path3.arcTo(
        QRectF(QPoint(rct3.bottomLeft().x(), rct3.bottomLeft().y() - (radius3 * 2)), QSize(radius3 * 2, radius3 * 2)),
        180, 90);

    path3.lineTo(rct3.bottomRight().x() - radius3, rct3.bottomRight().y());

    path3.arcTo(QRectF(QPoint(rct3.bottomRight().x() - (radius3 * 2), rct3.bottomRight().y() - (radius3 * 2)),
                       QSize(radius3 * 2, radius3 * 2)),
                270, 90);

    path3.lineTo(rct3.topRight());
    path3.closeSubpath();
    this->viewport()->setMask(path3.toFillPolygon().toPolygon());
}

void SimpleTableTempWidget::wheelEvent(QWheelEvent *event)
{
    this->update();
    this->viewport()->update();
    this->horizontalHeader()->update();
    this->horizontalHeader()->viewport()->update();
    QTableWidget::wheelEvent(event);
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
            painter->setPen(QPen(QColor(172, 182, 219), 1));
            // painter->setBrush(QColor(235, 243, 255));

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

void SimpleTableTempWidgetStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
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

void SimpleTableTempWidgetStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                             const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (element == CE_ShapedFrame) {
        return drawPrimitive(PE_Frame, option, painter, widget);
    } else if (element == CE_HeaderSection) {
        const QStyleOptionHeader *headerOption = qstyleoption_cast<const QStyleOptionHeader *>(option);
        if (headerOption && headerOption->position == QStyleOptionHeader::OnlyOneSection) {

            const QTableWidget *tbl = qobject_cast<const QTableWidget *>(widget->parentWidget());
            QPainterPath path;
            path.addRoundedRect(tbl->rect(), tbl->width() / 30, tbl->width() / 30);
            painter->setClipPath(path);

            // QRect rect = headerOption->rect;
            //  绘制自定义样式
            //  painter->fillRect(rect, QColor(0, 250, 0)); // 设置背景色为
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(235, 243, 255));
            painter->drawPath(path); // 绘制边框

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
            // QStyleOptionViewItem modifiedItemOption = *itemOption; //
            // 克隆原始itemOption modifiedItemOption.displayAlignment =
            // Qt::AlignCenter; // 修改对齐方式
            const QStyleOptionViewItem *optItem = qstyleoption_cast<const QStyleOptionViewItem *>(option);
            QStyleOptionViewItem optItmTemp(*optItem);
            painter->setPen(QPen(QColor(172, 182, 219), 1));
            painter->setBrush(QColor(235, 243, 255));
            QRect rect = optItem->rect;
            rect.adjust(0, 0, 1, 1);
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
            painter->drawLine(rect.topRight(), rect.bottomRight());

            painter->setPen(QPen(QColor(23, 44, 124), 2));
            optItmTemp.displayAlignment = Qt::AlignCenter;
            QProxyStyle::drawControl(element, &optItmTemp, painter, widget);
            // painter->drawText(rect, Qt::AlignCenter, optItem->text);
            return;
        }
    } else {
        return QProxyStyle::drawControl(element, option, painter, widget);
    }
}
} // namespace SortTableTemp
} // namespace Ui
