#ifndef SMPL_SCROLLEDIT_H
#define SMPL_SCROLLEDIT_H

#include <QListView>
#include <QProxyStyle>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QLineEdit>
#include <QScroller>
#include <QGraphicsDropShadowEffect>
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include "composite_edit.h"
#include "style_instance.h"

//-----------------------------------------------------------------------------------------------------

namespace Ui {
namespace ScrollEdit {
class ScrollViewStyle : public QProxyStyle {
    Q_OBJECT
public:
    ScrollViewStyle()
        : QProxyStyle(){

          };

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};
//-----------------------------------------------------------------------------------------------------
class ScrollEditList : public QListView {
    Q_OBJECT
public:
    ScrollEditList();
    ScrollEditList(QWidget *parent);

signals:
    void CurrentItem(const QModelIndex &current);

protected:
    void resizeEvent(QResizeEvent *event) override;

    virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

private:
    // ScrollViewStyle style_;

    QGraphicsDropShadowEffect *shadowLeft_;

    QGraphicsDropShadowEffect *shadowRight_;
};
//-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
//---滚动选择显示框-----------------------------------------------------------------------------------------
class SmplScrollEdit : public QLineEdit {
    Q_OBJECT
public:
    SmplScrollEdit(QWidget *parent);

    ScrollEditList *GetListView() const;
    void SetModel(QAbstractItemModel *model);
    int GetCurrentIndex();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::SmplEdit::EditStyle style_;

    ScrollEditList *pListView_;

    int nValueResult_;
    int nValueProcess_;
    int nStep_;

    bool bIsLeftPressed_;
    bool bIsRightPressed_;

    bool bIsPressed_;
    bool bIsMoving_;
    bool bIsReleased_;

    QPoint pointLast_;
};
} // namespace ScrollEdit
} // namespace Ui
#endif // SMPL_SCROLLEDIT_H
