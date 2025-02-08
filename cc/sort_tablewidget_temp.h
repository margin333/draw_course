#ifndef SORT_TABLEWIDGET_TEMP_H
#define SORT_TABLEWIDGET_TEMP_H

#include <QComboBox>
#include <QDebug>
#include <QGuiApplication>
#include <QHeaderView>
#include <QList>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QProxyStyle>
#include <QScrollBar>
#include <QScroller>
#include <QTableWidget>
#include <QToolTip>

namespace Ui {
namespace SortTableTemp {

class SimpleTableHeaderProxyStyle : public QProxyStyle {
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};

class SimpleTableTempWidgetStyle : public QProxyStyle {
protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};

class SimpleTableTempWidget : public QTableWidget {
    Q_OBJECT
public:
    SimpleTableTempWidget(QWidget *parent = nullptr);
    void CustomSort(int column, // 由于QTableWidget自带排序是以字符串形式排序，所以自定义一个以int排序的函数接口
                    Qt::SortOrder order);
    void SetSortHeaderIndex(QList<int> &&listIndex); // 设置允许排序的列
    void ShowColumn(const int &item);                // 显示某列
    void HideColumn(const int &item);                // 隐藏某列
    void SetVerticalHeaderHide();

protected:
    void UpdateHideColumn(int nHideColumn); // 更新需隐藏的某列进入列表
    bool viewportEvent(QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

signals:
    void itemsChanged(const QList<int> &items); // 信号:变化的某列

private:
    SimpleTableHeaderProxyStyle customHeaderStyle; // 允许排序的列的列表
    SimpleTableTempWidgetStyle customeWidgetStyle;
    QList<int> listSortHeader_; // 允许排序的列的列表
    QList<int> listHideColumn_; // 影藏的列的列表
    bool bAscend_;              // 记录当前排序列是升序还是降序
    int nLogicalIndex_;         // 记录当前排序的列

    //------------------------------------------------
    QPoint pointLast_;
    int nStepTmp_;
    int nValueTmp_;
    bool bMoving_;
    bool bPressed_;
};
} // namespace SortTableTemp
} // namespace Ui
#endif // SIMPLE_TABLEWIDGET_H
