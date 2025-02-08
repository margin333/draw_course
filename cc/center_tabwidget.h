#ifndef CENTER_TABWIDGET_H
#define CENTER_TABWIDGET_H

#include <QPainter>
#include <QDebug>
#include <QProxyStyle>
#include <QStyleOption>
#include <QTabWidget>
#include <QGraphicsDropShadowEffect>
#include "simple_tabwidget.h"
#include "paint_function.h"
#include "style_instance.h"

namespace Ui {
namespace TabBar {

// const QImage g_imgBarBeginPress_(":/MainMaskAlign/build/bin/image/bkg/tab_Begin_btn_press.png");
// const QImage g_imgBarBeginEnable_(":/MainMaskAlign/build/bin/image/bkg/tab_Begin_btn.png");
// const QImage g_imgBarMidPress_(":/MainMaskAlign/build/bin/image/bkg/tab_Mid_btn_press.png");
// const QImage g_imgBarMidEnable_(":/MainMaskAlign/build/bin/image/bkg/tab_Mid_btn.png");
// const QImage g_imgBarEndPress_(":/MainMaskAlign/build/bin/image/bkg/tab_End_btn_press.png");
// const QImage g_imgBarEndEnable_(":/MainMaskAlign/build/bin/image/bkg/tab_End_btn.png");

class CenterTabBar : public Ui::SmplTabWdt::SimpleTabBar {
public:
    CenterTabBar(QWidget *parent = nullptr);
    virtual ~CenterTabBar();

    void SetTabHeight(int nHeight);
    void SetTabWidth(int nWidth);

protected:
    QSize tabSizeHint(int index) const override;

private:
    int nWidth_;
    int nHeight_;
};

class CenterTabBarStyle : public QProxyStyle {
public:
    CenterTabBarStyle();

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget) const override;

private:
    QImage imgBarBeginPress_;
    QImage imgBarBeginEnable_;
    QImage imgBarMidPress_;
    QImage imgBarMidEnable_;
    QImage imgBarEndPress_;
    QImage imgBarEndEnable_;

    //    static QPixmap pixBarBeginPress_;
    //    static QPixmap pixBarBeginEnable_;
    //    static QPixmap pixBarMidPress_;
    //    static QPixmap pixBarMidEnable_;
    //    static QPixmap pixBarEndPress_;
    //    static QPixmap pixBarEndEnable_;
};

class CenterTabProxy : public QProxyStyle {
public:
    CenterTabProxy();

    //    inline void SetPaneColor(const QColor &color) { colorPane_ = color; };

protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;

    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;
};

class CenterTabWidget : public QTabWidget {
    Q_OBJECT
public:
    CenterTabWidget(QWidget *parent = nullptr);

protected:
    CenterTabBar tabBar_;

private:
    QGraphicsDropShadowEffect *shadowEffect_;

    //    CenterTabProxy styleTab_;
    //    CenterTabBarStyle styleTabBar_;
};
} // namespace TabBar
} // namespace Ui
#endif // CENTER_TABWIDGET_H
