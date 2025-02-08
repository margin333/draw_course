#ifndef SORT_TABLEWIDGET_H
#define SORT_TABLEWIDGET_H

#include <QObject>
#include <QTableWidget>
#include <QHeaderView>
#include <QPainter>
#include <QProxyStyle>
#include <QComboBox>
#include <QMouseEvent>
#include <QToolTip>
#include <QDebug>
#include <QScroller>
#include <QGuiApplication>
#include <QScrollBar>
#include <QList>
#include <QItemSelectionModel>
#include "style_instance.h"

namespace Ui {
namespace SortTable {

class SimpleTableHeaderProxyStyle : public QProxyStyle {
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};

class SimpleTableWidgetStyle : public QProxyStyle {
protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};

class ScrollTableScrollbarStyle : public QProxyStyle {
public:
    ScrollTableScrollbarStyle();

    ~ScrollTableScrollbarStyle(){};

    void drawComplexControl(ComplexControl element, const QStyleOptionComplex *option, QPainter *painter,
                            const QWidget *widget = nullptr) const override;

private:
    QImage imgScrollSliderOver_;
    QImage imgScrollSlider_;
    QImage imgScrollSubLine_;
    QImage imgScrollAddLine_;
};

class SimpleTableWidget : public QTableWidget {
    Q_OBJECT
public:
    SimpleTableWidget(QWidget *parent = nullptr);
    void CustomSort(int column, // 由于QTableWidget自带排序是以字符串形式排序，所以自定义一个以int排序的函数接口
                    Qt::SortOrder order);
    void SetSortHeaderIndex(QList<int> &&listIndex); // 设置允许排序的列
    void ShowColumn(const int &item);                // 显示某列
    void HideColumn(const int &item);                // 隐藏某列
    void SetVerticalHeaderHide();

protected:
    void UpdateHideColumn(int nHideColumn); // 更新需隐藏的某列进入列表
    void AutoSetMask();
    bool IsRowSelected(int row);
    bool viewportEvent(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
signals:
    void itemsChanged(const QList<int> &items); // 信号:变化的某列

private:
    ScrollTableScrollbarStyle styleScrollbar_;
    QList<int> listSortHeader_; // 允许排序的列的列表
    QList<int> listHideColumn_; // 影藏的列的列表
    bool bAscend_ = true;       // 记录当前排序列是升序还是降序
    int nLogicalIndex_ = -1;    // 记录当前排序的列

    QPoint pointLast_;
    int nStepTmp_;
    int nValueTmp_;
    bool bMoving_;
    bool bPressed_;

    int nHeightDefault_;
};
} // namespace SortTable
} // namespace Ui
#endif // SIMPLE_TABLEWIDGET_H
