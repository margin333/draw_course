/**************************************************************************
 * Copyright(C),2024,BNE
 * 模块名称:   自定义样式进度条
 * 模块功能:   八边形外框，带阴影光亮，红色方块进度指示器
 * 创建者:     jiwei
 * 创建日期:   2024.02.05
 ***************************************************************************/
#ifndef OCTAGONPROGRESSBAR_H
#define OCTAGONPROGRESSBAR_H

#include <QObject>
#include <QProgressBar>
#include <QGraphicsDropShadowEffect>
#include <QStyleOption>
#include <QPainter>
#include <QStylePainter>
#include <QProxyStyle>

namespace Ui {
namespace OctagonProgressBar {
class StyleProgress : public QProxyStyle {
    Q_OBJECT

public:
    StyleProgress();
    ~StyleProgress();
    QImage imgBlock_; // 进度条提示(一块)

public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = 0) const override; // 绘制基本元素控件
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = 0) const override; // 绘制处理控件
    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;
    // 获取不同元素的区域
    void DrawLabel(ControlElement element, const QStyleOption *option, QPainter *painter,
                   const QWidget *widget = 0) const; // 绘制文本
    void DrawContains(ControlElement element, const QStyleOption *option, QPainter *painter,
                      const QWidget *widget = 0) const; // 绘制进度条控件的内容区域
};

class OctagonProgressBar : public QProgressBar {
    Q_OBJECT
public:
    OctagonProgressBar(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    const double fGEN_2 = 1.4142135;

    //-----------------------------
    // 以原控件所在矩形为底板绘制八边形(大致思路:在矩形的四个直角区域分别去掉一个同样大小的等腰直角三角形)
    int nWidthLTBorder_; // 获得左上部分边距
    int nWidthRBBorder_; // 获得右下部分边距
    int nWidthBorder_;   // 左下和右上部分边距

    int nWidth_;  // 所在矩形宽度
    int nHeight_; // 所在矩形高度
    int nLeftX_;  // 八边形左上部分(也在矩形上边)的顶点横坐标

    int nRightX1_;   // 八边形右上部分(也在矩形上边)的顶点横坐标
    int nWidthCtrl_; // 八边形宽度

    int nY3_;         // 八边形左下部分(也在矩形左边)的顶点纵坐标
    int nHeightCtrl_; // 八边形高度

    int nHypotenuse_;    // 等腰直角三角形弦边长
    int nAngleRotation_; //(绘制时)旋转角度

    QColor colorBorder_;        // 控件边距颜色
    QColor colorBackground_;    // 控件背景颜色
    QColor colorBorderLeftTop_; // 控件左上部分边距颜色
    QColor colorLine_;          // 控件边线颜色

    QGraphicsDropShadowEffect *shadow_; // 控件外部阴影效果

    StyleProgress style_; // 进度条自定义样式
};
} // namespace OctagonProgressBar
} // namespace Ui
#endif // OCTAGONPROGRESSBAR_H
