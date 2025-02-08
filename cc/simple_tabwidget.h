#ifndef SIMPLE_TABWIDGET_H
#define SIMPLE_TABWIDGET_H

#include "style_instance.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QProxyStyle>
#include <QStyleOption>
#include <QTabWidget>

namespace Ui {
namespace SmplTabWdt {

class TabBarStyle : public QProxyStyle {
public:
    TabBarStyle();

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget) const override;

private:
    QImage imgBarSelected_;
    QImage imgBarEnable_;
};

class SimpleTabBar : public QTabBar {
    Q_OBJECT
public:
    SimpleTabBar(QWidget *parent = nullptr);
    virtual ~SimpleTabBar();
    void AddClickedCheck(const std::function<bool()> &func);

protected:
    void mousePressEvent(QMouseEvent *e) override;

private:
    std::vector<std::function<bool()>> vecFunc_;
};
//--------------------------------------------------------------------------------------------------
class TabProxy : public QProxyStyle {
public:
    TabProxy(QStyle *style = nullptr);

protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;

    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;

    QRect rectBar_;

private:
    QColor colorPane_;
};

class SimpleTabWidget : public QTabWidget {
    Q_OBJECT
public:
    SimpleTabWidget(QWidget *parent = nullptr);

    bool GetChangeFlag();
public slots:
    void SetChangeFlag(bool b);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    bool bChange_;
    SimpleTabBar tabBar_;
    QGraphicsDropShadowEffect *shadowEffect_;
};

class SimpleTabWidgetSidebr : public Ui::SmplTabWdt::SimpleTabWidget {
    Q_OBJECT
public:
    SimpleTabWidgetSidebr(QWidget *parent = nullptr);
protected:
    virtual void paintEvent(QPaintEvent *event) override
    {
        Ui::SmplTabWdt::SimpleTabWidget::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        QPainterPath path;
        QRect rct = this->rect();
        rct.adjust(1, 1, -1, -1);
        path.addRoundedRect(rct, this->width() / 40, this->width() / 40);

        painter.setPen(QPen(QColor(178, 205, 243), 2));
        painter.setBrush(Qt::white);
        painter.drawPath(path);

    };

    virtual void resizeEvent(QResizeEvent *event) override
    {

        Ui::SmplTabWdt::SimpleTabWidget::resizeEvent(event);
        QPainterPath path;
        path.addRoundedRect(this->rect(), this->width() / 40, this->width() / 40);
        this->setMask(path.toFillPolygon().toPolygon());
    };
};


} // namespace SmplTabWdt
} // namespace Ui
#endif // SIMPLE_TABWIDGET_H
