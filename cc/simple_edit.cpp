#include "simple_edit.h"

namespace Ui {
namespace SimpleEdit {

SimpleEdit::SimpleEdit(QWidget *parent)
    : QLineEdit(parent)
    , pValidator_(nullptr)
    , func_(nullptr)
    , typeCurrent_(DOUBLE)
    , nPrecision_(3)
    , bInputWarning_(false)
    , nRangeMin_(std::numeric_limits<int>::min())
    , nRangeMax_(std::numeric_limits<int>::max())
    , fRangeMin_(std::numeric_limits<double>::max() * -1 + 1)
    , fRangeMax_(std::numeric_limits<double>::max())
{
    // 1.
    this->setStyle(&style_);

    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, Qt::transparent);
    this->setPalette(pal);

    this->setAlignment(Qt::AlignCenter);

    pValidator_ = new QRegExpValidator(this);
    this->SetInputType(typeCurrent_, nPrecision_);

    connect(this, &SimpleEdit::editingFinished, this, [&]() -> void {
        this->setText(text());

        if (func_) {
            func_();
        }

        return;
    });

    connect(this, &SimpleEdit::textChanged, this, [&](const QString &text) -> void {
        Notice(text);
        return;
    });
}

void SimpleEdit::setText(const QString &str)
{

    QString strText = str;
    switch (typeCurrent_) {
    case DOUBLE: {
        double fTemp = strText.toDouble();
        strText = QString::number(fTemp, 'f', nPrecision_);
        break;
    }
    case INT: {
        //        if (!strText.isEmpty()) {
        //            long long nTemp = strText.toLongLong();

        //            if (nTemp > std::numeric_limits<int>::max()) {
        //                strText = QString::number(std::numeric_limits<int>::max());
        //            } else if (nTemp < std::numeric_limits<int>::min()) {
        //                strText = QString::number(std::numeric_limits<int>::min());
        //            } else {
        //                strText = QString::number(nTemp);
        //            }
        //        }
        if (!strText.isEmpty()) {
            bool bChange = false;
            qDebug() << strText << endl;
            int nTemp = strText.toInt(&bChange);
            qDebug() << nTemp << endl;
            if (bChange) {
                strText = QString::number(nTemp);
                qDebug() << "bChange" << endl;
            }
        }
        break;
    }
    case STRING:
        break;
    default:
        break;
    };

    Notice(strText);

    this->QLineEdit::setText(strText);
    return;
};

void SimpleEdit::SetInputType(INPUT_TYPE type, unsigned int nPrecision)
{
    QRegExp regex;
    QString str = text();
    switch (type) {
    case STRING: {
        regex.setPattern(QString(R"(.*)"));
        nPrecision_ = 0;

        break;
    }
    case INT: {
        regex.setPattern(QString(R"(^-?\d+$)"));
        nPrecision_ = 0;

        if (!str.isEmpty()) {
            if (typeCurrent_ == DOUBLE) {
                str = QString::number((int)str.toDouble());
            } else if (typeCurrent_ == STRING) {

                str = QString::number(str.toInt());
            }
        }

        break;
    }

    case DOUBLE: {
        regex.setPattern(QString(R"(^-?\d+(\.\d{0,%1})?$)").arg(nPrecision));
        nPrecision_ = nPrecision;

        if (typeCurrent_ == INT) {
            str = QString::number((double)str.toInt(), 'f', nPrecision_);
        } else if (typeCurrent_ == STRING) {
            str = QString::number(str.toDouble(), 'f', nPrecision_);
        }

        break;
    }
    default:
        return;
    }
    typeCurrent_ = type;
    pValidator_->setRegExp(regex);
    this->setValidator(pValidator_);

    this->QLineEdit::setText(str);
}

bool SimpleEdit::Notice(const QString &strText)
{
    QString strWarning = "";
    switch (CheckOnInputType(strText)) {
    case 0: {
        break;
    }
    case 1: {
        strWarning = QStringLiteral("< %0 !").arg(fRangeMin_);
        break;
    }
    case 3: {
        strWarning = QStringLiteral("< %0 !").arg(nRangeMin_);
        break;
    }
    case 2: {
        strWarning = QStringLiteral("> %0 !").arg(fRangeMax_);
        break;
    }
    case 4: {
        strWarning = QStringLiteral("> %0 !").arg(nRangeMax_);
        break;
    }
    case 5: {
        strWarning = QStringLiteral("out of type range!");
        break;
    }
    default:
        break;
    }
    if (!strWarning.isEmpty()) {
        QPalette pal = QToolTip::palette();

        pal.setColor(QPalette::Inactive, QPalette::ToolTipBase, QColor(240, 255, 240));
        QToolTip::setPalette(pal);
        QToolTip::showText(this->mapToGlobal(QPoint(0, this->height() / 2)), strWarning);
        return true;
    } else {
        return false;
    }
}

unsigned int SimpleEdit::CheckOnInputType(const QString &strText)
{
    switch (typeCurrent_) {
    case DOUBLE: {
        double fTemp = strText.toDouble();
        //        if (fRangeMin_ == std::numeric_limits<double>::min() && fRangeMax_ ==
        //        std::numeric_limits<double>::max() &&
        //            fTemp < 0) {
        //            fTemp = -1.0 * fTemp;
        //        }

        if (fTemp + EPS < fRangeMin_) {
            bInputWarning_ = true;
            return 1;
        } else if (fTemp > EPS + fRangeMax_) {
            bInputWarning_ = true;
            return 2;
        } else {
            bInputWarning_ = false;
        }
    } break;
    case INT: {
        if (!text().isEmpty()) {
            bool bChange = false;
            int nTemp = strText.toInt(&bChange);
            if (bChange) {
                if (nTemp < nRangeMin_) {
                    bInputWarning_ = true;
                    return 3;
                } else if (nTemp > nRangeMax_) {
                    bInputWarning_ = true;
                    return 4;
                } else {
                    bInputWarning_ = false;
                    break;
                }
            } else {
                bInputWarning_ = true;
                return 5;
            }
        }
    } break;
    case STRING:
        break;
    default:
        break;
    };
    return 0;
}

SimpleEdit::INPUT_TYPE SimpleEdit::GetInputType() const { return typeCurrent_; }

unsigned int SimpleEdit::GetPrecision() const { return nPrecision_; };

bool SimpleEdit::GetWarningState() const { return bInputWarning_; }

bool SimpleEdit::SetRange(int nRangeMin, int nRangeMax)
{
    if (nRangeMin - nRangeMax >= 0) {
        return false;
    }
    nRangeMin_ = nRangeMin;
    nRangeMax_ = nRangeMax;
    return true;
}

bool SimpleEdit::SetRange(double fRangeMin, double fRangeMax)
{
    if (std::abs(fRangeMin - fRangeMax) < EPS) {
        return false;
    } else if (fRangeMin > fRangeMax + EPS) {
        return false;
    }

    fRangeMin_ = fRangeMin;
    fRangeMax_ = fRangeMax;
    return true;
}

void SimpleEdit::SetEditFinishedEvent(const std::function<void()> &func)
{
    func_ = func;
    return;
}

void SimpleEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    return;
}

void SimpleEdit::resizeEvent(QResizeEvent *event)
{
    QLineEdit::resizeEvent(event);
    QPainterPath pathMask;
    pathMask.addRoundedRect(this->rect(), this->height() / 4, this->height() / 4);
    this->setMask(pathMask.toFillPolygon().toPolygon());
    return;
}

//-------------------------------------------------------------------------------------------
void EditStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                              const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing);
    const QLineEdit *lineEdit = qobject_cast<const QLineEdit *>(widget);
    if (PE_FrameLineEdit == element) {

        QRect rct = subElementRect(SE_LineEditContents, option, widget);
        if (lineEdit) {
            QPainterPath path;
            path.addRoundedRect(rct, rct.height() / 4, rct.height() / 4);

            if (lineEdit->isReadOnly()) {
                painter->setPen(QPen(QColor(184, 194, 227), 1));
                painter->setBrush(QColor(245, 249, 255));
            } else {
                painter->setPen(QPen(QColor(100, 163, 227), 1));
                painter->setBrush(QColor(219, 234, 255));

                const SimpleEdit *pSEdit = qobject_cast<const SimpleEdit *>(lineEdit);
                if (pSEdit->GetWarningState() && !pSEdit->text().isEmpty()) {
                    painter->setBrush(Qt::red);
                } else {
                    painter->setBrush(QColor(219, 234, 255));
                }
            }
            painter->drawPath(path);
        }

    } else if (PE_PanelLineEdit == element) {
        QPainterPath path;
        path.addRoundedRect(option->rect, option->rect.height() / 4, option->rect.height() / 4);

        if (lineEdit->isReadOnly()) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(245, 249, 255));
        } else {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(219, 234, 255));
        }
        painter->drawPath(path);

        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    } else {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

} // namespace SimpleEdit
} // namespace Ui
