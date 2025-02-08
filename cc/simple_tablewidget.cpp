#include "simple_tablewidget.h"
namespace Ui {
namespace SimpleTableWidget {
SimpleTableWidget::SimpleTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    SimpleTableWidgetStyle *pStyleRable = &StyleInstance<SimpleTableWidgetStyle>::GetIntance();
    this->setStyle(pStyleRable);

    SimpleTableHeaderProxyStyle *pStyleHeader = &StyleInstance<SimpleTableHeaderProxyStyle>::GetIntance();
    this->horizontalHeader()->setStyle(pStyleHeader);
    this->verticalHeader()->setStyle(pStyleHeader);

    this->horizontalHeader()->setSortIndicatorShown(true);

    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, QColor(207, 207, 207));
    pal.setColor(QPalette::Background, QColor(207, 207, 207));
    pal.setColor(QPalette::Inactive, QPalette::Highlight, QColor(207, 207, 207));

    this->setPalette(pal);

    this->setStyleSheet("QTableWidget::QTableCornerButton::section {"
                        "background-color: rgb(207,207,207);"
                        "border-top:0px solid rgb(170,170,170);"
                        "border-left:0px solid rgb(170,170,170);"
                        "border-right:0.5px solid rgb(170,170,170);"
                        "border-bottom: 0.5px solid rgb(170,170,170);"
                        "};"
                        "QTableWidget::item { text-align: center; }");

    this->horizontalHeader()->setStyleSheet("QHeaderView::section{"
                                            "border-top:0px solid rgb(170,170,170);"
                                            "border-left:0px solid rgb(170,170,170);"
                                            "border-right:0.5px solid rgb(170,170,170);"
                                            "border-bottom: 0.5px solid rgb(170,170,170);"
                                            "background-color: rgba(207,207,207,255);"
                                            "}");

    this->verticalHeader()->setStyleSheet("QHeaderView::section{"
                                          "border-top:0px solid rgb(170,170,170);"
                                          "border-left:0px solid rgb(170,170,170);"
                                          "border-right:0.5px solid rgb(170,170,170);"
                                          "border-bottom: 0.5px solid rgb(170,170,170);"
                                          "background-color: rgba(207,207,207,255);"
                                          "}");

    //    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(this->horizontalHeader(), &QHeaderView::sectionClicked, [&](int logicalIndex) {
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
}

void SimpleTableWidget::SetSortHeaderIndex(QList<int> &&listIndex) { listSortHeader_ = listIndex; }

void SimpleTableWidget::UpdateHideColumn(int nHideColumn) { listHideColumn_.append(nHideColumn); }

void SimpleTableWidget::CustomSort(int column, Qt::SortOrder order)
{
    int rowCount = this->rowCount();
    QList<QList<QTableWidgetItem *>> rowData;
    for (int i = 0; i < rowCount; i++) {
        QList<QTableWidgetItem *> currentRow;
        for (int j = 0; j < this->columnCount(); j++) {
            currentRow.append(this->takeItem(i, j));
        }
        rowData.append(currentRow);
    }

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

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < this->columnCount(); j++) {
            this->setItem(i, j, rowData[i][j]);
        }
    }
}

void SimpleTableWidget::ShowColumn(const int &item)
{
    listHideColumn_.removeOne(item);
    emit itemsChanged(listHideColumn_);
}

void SimpleTableWidget::HideColumn(const int &item)
{
    if (this->columnCount() > listHideColumn_.count()) {
        listHideColumn_.append(item);
        emit itemsChanged(listHideColumn_);
    }
}

void SimpleTableHeaderProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                              const QWidget *widget) const
{
    if (CE_HeaderEmptyArea == element) {
        painter->save();

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(207, 207, 207, 255));
        painter->drawRect(option->rect);

        painter->restore();
    } else {
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}

void SimpleTableWidgetStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                         const QWidget *widget) const
{
    return QProxyStyle::drawControl(element, option, painter, widget);
}

} // namespace SimpleTableWidget
} // namespace Ui
