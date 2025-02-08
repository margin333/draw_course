/**************************************************************************
 * Copyright(C),2024,BNE
 * 模块名称:    自定义样式复选框
 * 模块功能:    通过派生QProxyStyle类修改复选框区域阴影效果，点击效果使用自定义图片
 * 创建者:     jiwei
 * 创建日期:   2024.02.05
 ***************************************************************************/

#ifndef EZ_CHECKBOX_H
#define EZ_CHECKBOX_H
#include <QCheckBox>
#include <QObject>
#include <QProxyStyle>
#include <QStyleOption>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

namespace Ui {
namespace EzCheckBox {

class CustomCheckBoxStyle : public QProxyStyle {
public:
    CustomCheckBoxStyle(QStyle *baseStyle = nullptr);

protected:
    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, // 绘制基础元素图形
                       const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, // 绘制控制元素控件
                     const QWidget *widget = nullptr) const override;

private:
    QImage imageStateOn_;  // 激活状态下显示的图片
    QImage imageStateOff_; // 未激活状态下显示的图片
};

class EzCheckBox : public QCheckBox {
public:
    explicit EzCheckBox(QWidget *parent = nullptr);
    EzCheckBox(const QString &text, QWidget *parent = nullptr);
    virtual ~EzCheckBox();

private:
    CustomCheckBoxStyle style_; // 定义Qt样式表现
};
} // namespace EzCheckBox
} // namespace Ui

#endif // EZ_CHECKBOX_H
