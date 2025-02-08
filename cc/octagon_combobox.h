/**************************************************************************
 * Copyright(C),2024,BNE
 * 模块名称:   自定义下拉框
 * 模块功能:   自绘其中组件文本显示框、点击按钮以及点击弹出窗口；改变点击事件不同状态样式
 * 创建者:     jiwei
 * 创建日期:   2024.02.05
 ***************************************************************************/

#ifndef OCTAGONCOMBOBOX_H
#define OCTAGONCOMBOBOX_H

#include <QComboBox>
#include <QPainter>
#include <QStylePainter>
#include <QProxyStyle>
#include <QListView>
#include <QStandardItemModel>

namespace Ui {
namespace OctagonComboBox {

class OctagonComboBoxListView : public QListView {
public:
    void paintEvent(QPaintEvent *event) override;
};

class OctagonComboBoxStyle : public QProxyStyle {
public:
    OctagonComboBoxStyle();
    void drawComplexControl(ComplexControl which,
                            const QStyleOptionComplex *option, // 自定义控件风格中绘制复杂的控件元素
                            QPainter *painter, const QWidget *widget = nullptr) const override;
    QRect subControlRect(ComplexControl whichControl, const QStyleOptionComplex *option,
                         SubControl whichSubControl, // 获取不同元素的区域
                         const QWidget *widget = nullptr) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, // 绘制基础元素控件
                       const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, // 绘制控制元素控件
                     const QWidget *widget = nullptr) const override;
};

class OctagonComboBox : public QComboBox {
    Q_OBJECT
public:
    OctagonComboBox(QWidget *parent);
    virtual ~OctagonComboBox();

protected:
    void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

    void showPopup() override; // 弹出窗口

private:
    OctagonComboBoxStyle styleCustom_;   // 自定义控件样式
    OctagonComboBoxListView viewCustom_; // 自定义弹出列表样式

    const double fGEN_2_ = 1.4142135; // 根号2
    int nWidthLTBorder_;              // 左上部分阴影边距
    int nWidthRBBorder_;              // 右下部分阴影边距
    int nWidthBorder_;                // 左下，右下部分阴影边距

    // 以原控件所在矩形为底板绘制八边形(大致思路:在矩形的四个直角区域分别去掉一个同样大小的等腰直角三角形)
    int nWidth_;   // 所在矩形宽度
    int nHeight_;  // 所在矩形高度
    int nPadding_; // 等腰直角三角形的边长

    int nTmpX1_;     // 八边形右上部分(也在矩形上边)的顶点坐标
    int nWidthCtrl_; // 八边形宽度

    int nLengthCtrl_; // 八边形高度

    int nHypotenuse_; // 等腰直角三角形的弦长
};
} // namespace OctagonComboBox
} // namespace Ui
#endif // OCTAGONCOMBOBOX_H
