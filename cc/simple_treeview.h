#ifndef SIMPLETREEVIEW_H
#define SIMPLETREEVIEW_H
#include <QDebug>

#include <QPainter>
#include <QProxyStyle>
#include <QTreeView>
#include <QScrollBar>
#include <QTextLayout>

namespace Ui {
namespace SmpTree {
class ComboScrollbarStyle : public QProxyStyle {
public:
    ComboScrollbarStyle();

    ~ComboScrollbarStyle(){};

    void drawComplexControl(ComplexControl element, const QStyleOptionComplex *option, QPainter *painter,
                            const QWidget *widget = nullptr) const override;

    QRect subControlRect(ComplexControl element, const QStyleOptionComplex *option, SubControl whichSubControl,
                         const QWidget *widget = nullptr) const override;

private:
};

class CommonTreeStyle : public QProxyStyle {
public:
    CommonTreeStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;

    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;

private:
    QIcon iconAll_;
    QIcon iconDatabase_;
    QIcon iconFile_;
    QIcon iconTxt_;
};

//-------------------------------------------------------------
class SimpleTreeView : public QTreeView {
    Q_OBJECT
public:
    SimpleTreeView(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    CommonTreeStyle style_;
    ComboScrollbarStyle styleBar_;
};

} // namespace SmpTree
} // namespace Ui

#endif // SIMPLETREEVIEW_H
