#include "composite_edit.h"
namespace Ui {
namespace SmplEdit {
PopTipLabel::PopTipLabel(QWidget *parent)
    : QLabel(parent){};
PopTipLabel::PopTipLabel(QString strText, QWidget *parent)
    : QLabel(strText, parent){};
PopTipLabel::~PopTipLabel(){};

void PopTipLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath pathRoundRect;
    pathRoundRect.addRoundedRect(0, 0, width(), height() * 6 / 7, height() * 3 / 7, height() * 3 / 7);

    QPainterPath pathTriangle;
    pathTriangle.moveTo(width() / 2 - height() / 7, height() * 6 / 7);
    pathTriangle.lineTo(width() / 2 + height() / 7, height() * 6 / 7);
    pathTriangle.lineTo(width() / 2, height());
    pathTriangle.closeSubpath();

    QPainterPath pathCombined = pathRoundRect.united(pathTriangle);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(23, 44, 124));
    painter.drawPath(pathCombined);

    if (!text().isEmpty()) {
        painter.setPen(QPen(QColor(231, 233, 241), 1));
        painter.setBrush(Qt::NoBrush);
        QRectF rect(0, 0, width(), height() * 6 / 7);
        painter.drawText(rect, Qt::AlignCenter, text());
    }
};

void PopTipLabel::resizeEvent(QResizeEvent *event) { QLabel::resizeEvent(event); };

CompositeEdit::CompositeEdit(QWidget *parent)
    : QLineEdit(parent)
    , labelHint_(parent)
    , timerJudgeClick_(nullptr)
    , timerDraw_(nullptr)
    , bIsDrawPressed_(false)
    , bIsPressed_(false)
    , bIsReleased_(true)
    , bIsDoubleClick_(false)
    , bIsMoving_(false)
    , nStep_(0)
    , nVal_(0)
    , bIsLeftPressed_(false)
    , bIsRightPressed_(false)

{
    this->setContentsMargins(QMargins(2, 2, 2, 2));
    this->setFocusPolicy(Qt::ClickFocus);
    timerJudgeClick_ = new QTimer(this);
    timerJudgeClick_->setSingleShot(true);
    timerJudgeClick_->setInterval(250);

    timerDraw_ = new QTimer(this);
    timerDraw_->setSingleShot(true);
    timerDraw_->setInterval(100);

    connect(timerDraw_, &QTimer::timeout, this, [&]() {
        if (!bIsDoubleClick_) {
            bIsDrawPressed_ = false;
            bIsLeftPressed_ = false;
            bIsRightPressed_ = false;
            update();
        }
    });

    connect(timerJudgeClick_, &QTimer::timeout, this, [&]() {
        if (!bIsDoubleClick_ && bIsReleased_) {
            bIsDrawPressed_ = true;

            QVariant value = timerJudgeClick_->property("pointXClick");
            if (value.isValid()) {

                int nPosX = QVariant(timerJudgeClick_->property("pointXClick")).toInt();

                if (nPosX >= width() / 2) {
                    --nVal_;
                    bIsRightPressed_ = true;
                } else {
                    ++nVal_;
                    bIsLeftPressed_ = true;
                }

            } else {
                bIsLeftPressed_ = false;
                bIsRightPressed_ = false;
            }

            this->setText(QStringLiteral("%0").arg(nVal_));
            update();
            if (!timerDraw_->isActive()) {
                timerDraw_->start();
            }
        }
    });
    // 1.
    EditStyle *pStyle = &StyleInstance<EditStyle>::GetIntance();
    this->setStyle(pStyle);

    this->setAlignment(Qt::AlignCenter);
    this->setText("0");

    labelHint_.setParent(parent);
    labelHint_.hide();
}

void CompositeEdit::resizeEvent(QResizeEvent *event)
{
    QLineEdit::resizeEvent(event);
    labelHint_.resize(3 * this->height(), this->height());
    QPainterPath path;
    path.addRoundedRect(0, 0, rect().width(), rect().height(), rect().height() / 6, rect().height() / 6);
    this->setMask(path.toFillPolygon().toPolygon());
}

void CompositeEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {

        this->clearFocus();
        return;
    } else {
        return QLineEdit::keyPressEvent(event);
    }
}

void CompositeEdit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pointLast_ = event->pos();
        nVal_ = text().toInt();
        bIsDoubleClick_ = false;
        bIsPressed_ = true;
        bIsReleased_ = false;
        setReadOnly(true);

        if (!timerJudgeClick_->isActive()) {
            timerJudgeClick_->setProperty("pointXClick", QVariant(event->pos().x()));
            timerJudgeClick_->start();
        }
    }

    return QLineEdit::mousePressEvent(event);
};

void CompositeEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if (!bIsDoubleClick_) {
        setReadOnly(true);
    }

    if (!labelHint_.isHidden() && bIsMoving_) {
        labelHint_.hide();
    }

    bIsMoving_ = false;
    bIsPressed_ = false;
    bIsReleased_ = true;

    return QLineEdit::mouseReleaseEvent(event);
};

void CompositeEdit::mouseMoveEvent(QMouseEvent *event)
{
    bIsMoving_ = true;

    if (rect().contains(event->pos()) && (2 * abs(event->pos().x() - pointLast_.x()) >= 1)) {
        if (bIsPressed_ && bIsMoving_) {
            if (timerJudgeClick_->isActive()) {
                timerJudgeClick_->stop();
            }

            if (labelHint_.isHidden()) {
                QPoint pointParent = mapTo((QWidget *)this->parent(), this->rect().topLeft());
                labelHint_.move(pointParent.x() + this->width() / 2 - labelHint_.width() / 2,
                                pointParent.y() - labelHint_.height() - 2);
                labelHint_.show();
                labelHint_.raise();
            }

            nStep_ = event->pos().x() - pointLast_.x();

            labelHint_.setText(QStringLiteral("Tip:%0").arg(nVal_ + nStep_));

            this->setText(QStringLiteral("%0").arg(nVal_ + nStep_));
        }
    }

    return QLineEdit::mouseMoveEvent(event);
};

void CompositeEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setReadOnly(false);
        bIsDoubleClick_ = true;
    }

    return QLineEdit::mouseDoubleClickEvent(event);
};

void CompositeEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    QRectF rectHis(0 - rect().height() * 9 / 26, 0 - rect().height() * 1 / 13, rect().height() * 18 / 26,
                   rect().height() * 2 / 13);
    QRectF rectVer(0 - rect().height() * 1 / 13, 0 - rect().height() * 9 / 26, rect().height() * 2 / 13,
                   rect().height() * 18 / 26);

    QRectF rectBtn(0, 0, rect().height(), rect().height());
    QPainterPath pathBtn;
    pathBtn.addRoundedRect(rectBtn, rect().height() / 4, rect().height() / 4);
    QLinearGradient backgroundBtn = QLinearGradient(0, 0, 0, rect().height()); // 上下
    backgroundBtn.setColorAt(0, QColor(47, 104, 212));
    backgroundBtn.setColorAt(1, QColor(33, 51, 123));

    painter.save();
    if (bIsDrawPressed_ && bIsLeftPressed_) {
        painter.fillPath(pathBtn, QBrush(backgroundBtn));
    } else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(pathBtn);
    }

    painter.save();
    painter.translate(rect().height() / 2, rect().height() / 2);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(rectHis, rectHis.height() / 2, rectHis.height() / 2);
    painter.drawRoundedRect(rectVer, rectVer.width() / 2, rectVer.width() / 2);
    painter.restore();
    //------------------------------------------------------------------------
    painter.restore();

    painter.save();
    painter.translate(rect().width() - rect().height(), 0);

    if (bIsDrawPressed_ && bIsRightPressed_) {
        painter.fillPath(pathBtn, QBrush(backgroundBtn));
    } else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(pathBtn);
    }

    painter.save();
    painter.translate(rect().height() / 2, rect().height() / 2);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(rectHis, rectHis.height() / 2, rectHis.height() / 2);
    painter.restore();
    //------------------------------------------------------------------------
    painter.restore();
    //------------------------------------------------------------------------
}

EditStyle::EditStyle() { }

EditStyle::~EditStyle() { }

void EditStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                              const QWidget *widget) const
{
    if (element == QStyle::PE_PanelLineEdit || element == QStyle::PE_FrameLineEdit) {
        QStyleOption opt(*option);
        opt.initFrom(widget);

        if (element == QStyle::PE_PanelLineEdit) {
            drawPanelLineEdit(&opt, painter, widget);
        } else if (element == QStyle::PE_FrameLineEdit) {
            drawFrameLineEdit(&opt, painter, widget);
        }
    } else {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}

void EditStyle::drawPanelLineEdit(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    Q_UNUSED(option);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    QRect rectTmp(widget->rect());
    //
    path.addRoundedRect(0, 0, rectTmp.width(), rectTmp.height(), rectTmp.height() / 5, rectTmp.height() / 5);
    painter->setClipPath(path);
    painter->setPen(QPen(QColor(23, 44, 124), 1));
    painter->setBrush(QColor(239, 242, 249));
    painter->drawRoundedRect(rectTmp, rectTmp.height() / 4, rectTmp.height() / 4);
}

void EditStyle::drawFrameLineEdit(const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    return QProxyStyle::drawPrimitive(QStyle::PE_FrameLineEdit, option, painter, widget);
}

} // namespace SmplEdit
} // namespace Ui
