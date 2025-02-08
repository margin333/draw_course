#ifndef BNE_GROUPBOX_H
#define BNE_GROUPBOX_H

#include <QGroupBox>
#include <QDebug>
#include <QProxyStyle>
#include <QPainter>
#include "qdrawutil.h"

class BNEGroupBoxStyle : public QProxyStyle {
protected:
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc,
                         const QWidget *widget) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;
};

class BNEGroupBox : public QGroupBox {
    Q_OBJECT
public:
    BNEGroupBox(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e) override { return QGroupBox::paintEvent(e); };
    void resizeEvent(QResizeEvent *e) override { return QGroupBox::resizeEvent(e); };

private:
    BNEGroupBoxStyle style_;
};

#endif // BNE_GROUPBOX_H
