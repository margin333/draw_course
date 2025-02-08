#ifndef COMPOSITE_EDIT_H
#define COMPOSITE_EDIT_H

#include <QProxyStyle>
#include <QLineEdit>
#include <QDebug>
#include <QMouseEvent>
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QHelpEvent>
#include <QToolTip>
#include <QLabel>
#include <QStyleOption>
#include "style_instance.h"

namespace Ui {
namespace SmplEdit {
class PopTipLabel : public QLabel {
public:
    PopTipLabel(QWidget *parent);
    PopTipLabel(QString strText, QWidget *parent);

    virtual ~PopTipLabel() override;

public:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

class EditStyle : public QProxyStyle {
public:
    EditStyle();

    ~EditStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;

    void drawPanelLineEdit(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;

    void drawFrameLineEdit(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
};

class CompositeEdit : public QLineEdit {
    Q_OBJECT
public:
    CompositeEdit(QWidget *parent);

protected:
    // EditStyle styleEdit_;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    PopTipLabel labelHint_;

    QTimer *timerJudgeClick_;
    QTimer *timerDraw_;
    bool bIsDrawPressed_;
    bool bIsPressed_;
    bool bIsReleased_;

    bool bIsDoubleClick_;
    bool bIsMoving_ = false;

    int nStep_;

    int nVal_;

    bool bIsLeftPressed_;  // 鼠标是否按下
    bool bIsRightPressed_; // 鼠标是否按下

    QPoint pointLast_; // 鼠标按下最后坐标(非双击按下)
};
} // namespace SmplEdit
} // namespace Ui
#endif // COMPOSITE_EDIT_H
