/**************************************************************************
 * Copyright(C),2024,BNE
 * 模块名称:   八边形按钮
 * 模块功能:   自定义八边形外框，带边框阴影光亮效果
 * 创建者:     jiwei
 * 创建日期:   2024.02.05
 ***************************************************************************/

#ifndef OCTAGONBUTTON_H
#define OCTAGONBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QPainter>
#include <QPolygonF>
#include <QtMath>
#include <QGraphicsDropShadowEffect>
namespace Ui {
namespace OctagonButton {
class OctagonButton : public QPushButton {
    Q_OBJECT
public:
    OctagonButton(const QString &text, QWidget *parent = 0);
    OctagonButton(QWidget *parent = 0);

    void SetBorderWidth(int iWidthLTBorder, int iWidthRBBorder, int iWidthBorder); // 设置阴影厚度

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    virtual void resizeEvent(QResizeEvent *event) override;

    virtual void mousePressEvent(QMouseEvent *evt) override; // 鼠标按下事件

    virtual void mouseReleaseEvent(QMouseEvent *evt) override; // 鼠标释放事件

    const double fGEN_2 = 1.4142135;
    // 以原控件所在矩形为底板绘制八边形(大致思路:在矩形的四个直角区域分别去掉一个同样大小的等腰直角三角形)
    //--八边形，以“上边”为起始边顺时针排序-------------
    int nWidthLeftTopBorder_;     // 边（1,7,8）阴影效果宽度
    int nWidthRightBottomBorder_; // 边（3，4，5）阴影效果宽度
    int nWidthBorder_;            // 边（2，6）阴影效果宽度

    int nAngleRotation_; // 旋转角度

    int nWidth_;     // 矩形宽度
    int nHeight_;    // 矩形长度
    int nLeftX1_;    // 相对于窗口坐标，八边形边1线段从左往右画的起点
    int nRightX1_;   // 相对于窗口坐标，八边形边1线段从左往右画的终点
    int nWidthCtrl_; // 八边形边1线段长度

    int nY3_;         // 八边形边3线段从上往下画的起点
    int nHeightCtrl_; // 八边形边3线段长度

    int nHypotenuse_; // nLeftX1与nY构成的直角三角形弦边长度

    bool bPress_; // 记录鼠标按下状态

    QColor colorBorder_;           // 边线的颜色
    QColor colorBackground_;       // 背景颜色
    QColor colorPressLeftTop_;     // 按下状态左上部分边距颜色
    QColor colorPressRightBottom_; // 按下状态右下部分边距颜色
};

class CheckButton : public OctagonButton {
    Q_OBJECT
public:
    CheckButton(const QString &text, QWidget *parent = 0)
        : OctagonButton(text, parent)
    {
    }
    CheckButton(QWidget *parent = 0)
        : OctagonButton(parent){};
        //获取当前按钮是否被按下
    inline bool GetPressState(){
        return bPress_;
    };
    //设置按钮按下状态
    inline void SetPressState(bool bPress){
        bPress_ = bPress;
        update();
    };

protected:
    virtual void mousePressEvent(QMouseEvent *event) override
    {
        bPress_ = !bPress_;
        update();
        QPushButton::mousePressEvent(event);
    };

    virtual void mouseReleaseEvent(QMouseEvent *event) override
    {
        QPushButton::mouseReleaseEvent(event);
        return;
    };
};
} // namespace OctagonButton
} // namespace Ui
#endif // OCTAGONBUTTON_H
