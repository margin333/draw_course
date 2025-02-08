/**************************************************************************
 * Copyright(C),2024,BNE
 * 模块名称:    八边形编辑框
 * 模块功能:    直接继承QLineEdit，八边形外框，带阴影光亮效果
 * 创建者:     jiwei
 * 创建日期:   2024.02.05
 ***************************************************************************/

#ifndef OCTAGONEDIT_H
#define OCTAGONEDIT_H

#include <QLineEdit>
#include <QWidget>
#include <QPainter>
namespace Ui {
namespace OctagonEdit {
class OctagonEdit : public QLineEdit {
    Q_OBJECT
public:
    OctagonEdit(QWidget *parent = 0);
    virtual ~OctagonEdit();

    int GetLTBorderWidth(); // 获得左上部分边距
    int GetRBBorderWidth(); // 获得右下部分边距
    int GetBorderWidth();   // 获得左下和右上部分边距
    int getPaddingWidth();  // 八边形左上部分的顶点和所在矩形左上角顶点的距离

public:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    const double fGEN_2 = 1.4142135;
    int nWidthLeftTopBorder_;     // 获得左上部分边距
    int nWidthRightBottomBorder_; // 获得右下部分边距
    int nWidthBorder_;            // 左下和右上部分边距
    //-----------------------------
    // 以原控件所在矩形为底板绘制八边形(大致思路:在矩形的四个直角区域分别去掉一个同样大小的等腰直角三角形)
    int nWidth_;                        // 所在矩形宽度
    int nHeight_;                       // 所在矩形高度
    int nLeftX1_;                       // 八边形左上部分(也在矩形上边)的顶点横坐标
    int nRightX1_;                      // 八边形右上部分(也在矩形上边)的顶点横坐标
    int nWidthCtrl_;                    // 八边形宽度
    int nY3_;                           // 八边形左下部分(也在矩形左边)的顶点纵坐标
    int nHeightCtrl_;                   // 八边形高度
    int nHypotenuse_;                   // 等腰直角三角形弦边长
    int nAngleRotation_;                //(绘制时)旋转角度
    QColor colorBackground_;            // 控件背景颜色
    QColor colorBorderLeftTop_;         // 控件左上部分边距颜色
    QColor colorBorderRightBottom_;     // 控件右下部分边距颜色
    QColor colorLine_;                  // 控件边线颜色
    QColor colorBackgroundTransparent_; // 控件背景颜色(带透明色)
};
} // namespace OctagonEdit
} // namespace Ui
#endif // OCTAGONEDIT_H
