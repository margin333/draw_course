#ifndef SIMPLE_EDIT_H
#define SIMPLE_EDIT_H

#include "style_instance.h"
#include <QToolTip>
#include <QLineEdit>
#include <QPainter>
#include <QProxyStyle>
#include <QDebug>
#include <QStyleOption>
#include <QMessageBox>

namespace Ui {
namespace SimpleEdit {

const double EPS = 1e-10;

class EditStyle : public QProxyStyle {
public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;
};

class SimpleEdit : public QLineEdit {
    Q_OBJECT
public:
    enum INPUT_TYPE { DOUBLE, INT, STRING };

    SimpleEdit(QWidget *parent);

    void SetInputType(INPUT_TYPE type, unsigned int nPrecision = 3);
    INPUT_TYPE GetInputType() const;
    unsigned int GetPrecision() const;

    bool GetWarningState() const;

    bool SetRange(int nRangeMin, int nRangeMax);
    bool SetRange(double fRangeMin, double fRangeMax);

    void SetEditFinishedEvent(const std::function<void()> &func);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

    unsigned int CheckOnInputType(const QString &strText);
    bool Notice(const QString &strText);

public Q_SLOTS:
    void setText(const QString &str);

private:
    EditStyle style_;

    QRegExpValidator *pValidator_;
    std::function<void()> func_;

    INPUT_TYPE typeCurrent_;
    unsigned int nPrecision_;

    bool bInputWarning_;

    int nRangeMin_;
    int nRangeMax_;

    double fRangeMin_;
    double fRangeMax_;
};

// class SimpleEdit : public QLineEdit {
//     Q_OBJECT
// public:
//     SimpleEdit(QWidget *parent);
//     // EditStyle style_;

// protected:
//     virtual void paintEvent(QPaintEvent *event) override;
//     virtual void resizeEvent(QResizeEvent *event) override;
// };
} // namespace SimpleEdit
} // namespace Ui
#endif // SIMPLE_EDIT_H
