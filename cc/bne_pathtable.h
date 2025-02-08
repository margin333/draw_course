#ifndef PATHTABLE_H
#define PATHTABLE_H

#include <QTableWidget>

#include <QObject>
#include <QTableWidget>
#include <QHeaderView>
#include <QPainter>
#include <QProxyStyle>
#include <QComboBox>
#include <QMouseEvent>
#include <QToolTip>
#include <QDebug>
#include <QScroller>
#include <QGuiApplication>
#include <QStandardItemModel>
#include <QScrollBar>
#include <QList>

namespace Ui {
namespace PathTable {

enum PathMode {
    RHOMBUS,
    DIAGONAL,
};

class SimpleTableHeaderStyle : public QProxyStyle {
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};

class PathTableHeaderStyle : public QProxyStyle {
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};

class PathTableWidgetStyle : public QProxyStyle {
protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};

class PathTableWidget : public QTableWidget {
    Q_OBJECT
public:
    PathTableWidget(PathMode mode, QWidget *parent = nullptr);
    virtual ~PathTableWidget(){};

    void ChangeMode(PathMode mode);

protected:
    bool viewportEvent(QEvent *event) override;

    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
signals:
    void itemsChanged(const QList<int> &items); // 信号:变化的某列

private:
    int nMode_;
    QStringList listHoriHeader_;
    QStringList listVerHeader_;

    QStandardItemModel modelRHOMBUS_;
    QStandardItemModel modelDIAGONAL_;

    SimpleTableHeaderStyle styleHeaderHor_; // 允许排序的列的列表
    PathTableHeaderStyle styleHeaderVer_;

    PathTableWidgetStyle styleTable;
    bool bPress_;
    bool bMoving_;
};
} // namespace PathTable
} // namespace Ui
#endif // PATHTABLE_H
