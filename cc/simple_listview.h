#ifndef SIMPLELISTVIEW_H
#define SIMPLELISTVIEW_H

#include <QDebug>
#include <QPainter>
#include <QProxyStyle>
#include <QListView>
#include <QScrollBar>

namespace Ui {
namespace ListView {

class ComboScrollbarStyle : public QProxyStyle {
public:
    void drawComplexControl(ComplexControl element, const QStyleOptionComplex *option, QPainter *painter,
                            const QWidget *widget = nullptr) const override;

private:
};

class CommonListStyle : public QProxyStyle {
public:
    CommonListStyle();
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;

private:
    QImage imgCheckTrue_;
    QImage imgCheckFalse_;
};

class SimpleListView : public QListView {
    Q_OBJECT
public:
    ~SimpleListView();
    SimpleListView(QWidget *parent = nullptr);

    virtual void setSelectionMode(QAbstractItemView::SelectionMode mode);

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

    void updateGeometries() override { QListView::updateGeometries(); };

    void dropEvent(QDropEvent *event) override { return QListView::dropEvent(event); };

private:
    CommonListStyle style_;
    ComboScrollbarStyle styleBar_;

    QColor colorNoSele_;
    QColor colorNormal_;
};
} // namespace ListView
} // namespace Ui
#endif // SIMPLELISTVIEW_H
