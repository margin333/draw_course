/**************************************************************************
 * Copyright(C),2024,BNE
 * 模块名称:   自定义表单
 * 模块功能:   改变表单样式，点击表头响应排序，提供信号隐藏或显示指定列
 * 创建者:     jiwei
 * 创建日期:   2024.02.05
 ***************************************************************************/

#ifndef SIMPLE_TABLEWIDGET_H
#define SIMPLE_TABLEWIDGET_H

#include <QObject>
#include <QTableWidget>
#include <QHeaderView>
#include <QPainter>
#include <QProxyStyle>
#include <QComboBox>
#include <QMouseEvent>
#include "style_instance.h"

namespace Ui {
namespace SimpleTableWidget {

class SimpleTableHeaderProxyStyle : public QProxyStyle {
protected:
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};

class SimpleTableWidgetStyle : public QProxyStyle {
protected:
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
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

protected:
    void UpdateHideColumn(int nHideColumn); // 更新需隐藏的某列进入列表

signals:
    void itemsChanged(const QList<int> &items); // 信号:变化的某列

private:
    // SimpleTableHeaderProxyStyle customHeaderStyle; // 允许排序的列的列表
    // SimpleTableWidgetStyle customeWidgetStyle;
    QList<int> listSortHeader_; // 允许排序的列的列表
    QList<int> listHideColumn_; // 影藏的列的列表
    bool bAscend_ = true;       // 记录当前排序列是升序还是降序
    int nLogicalIndex_ = -1;    // 记录当前排序的列
};
} // namespace SimpleTableWidget
} // namespace Ui
#endif // SIMPLE_TABLEWIDGET_H
