#ifndef SIMPLE_RADIOBUTTON_H
#define SIMPLE_RADIOBUTTON_H

#include "qdebug.h"
#include <QRadioButton>
#include <QProxyStyle>
#include <QPainter>
#include <QStyleOption>
#include "style_instance.h"

namespace Ui {
namespace SimpleRadio {

class SimpleRadioStyle : public QProxyStyle {
protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;
};

class SimpleRadioButton : public QRadioButton {
    Q_OBJECT
public:
    explicit SimpleRadioButton(QWidget *parent = nullptr);
    explicit SimpleRadioButton(const QString &text, QWidget *parent = nullptr);

private:
    // SimpleRadioStyle style_;
};
} // namespace SimpleRadio
} // namespace Ui
#endif // SIMPLE_RADIOBUTTON_H
