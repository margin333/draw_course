#include "smpl_scrolledit.h"

//----------------------------------------------------------------------------------
namespace Ui {
namespace ScrollEdit {
ScrollEditList::ScrollEditList()
    : QListView()
{
    this->setEditTriggers(QListView::NoEditTriggers);
    this->setFlow(QListView::LeftToRight);

    ScrollViewStyle *pStyle = &StyleInstance<ScrollViewStyle>::GetIntance();
    this->setStyle(pStyle);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::transparent);
    this->viewport()->setBackgroundRole(QPalette::Background);
    this->viewport()->setPalette(palette);

    this->setResizeMode(QListView::Adjust);

    shadowLeft_ = new QGraphicsDropShadowEffect(this);
    if (shadowLeft_) {
        shadowLeft_->setColor(QColor(190, 190, 190));

        shadowLeft_->setOffset(-2, 2);

        shadowLeft_->setBlurRadius(5);
        this->setGraphicsEffect(shadowLeft_);
    }
};

ScrollEditList::ScrollEditList(QWidget *parent)
    : QListView(parent)
{
    this->setEditTriggers(QListView::NoEditTriggers);
    this->setFlow(QListView::LeftToRight);

    ScrollViewStyle *pStyle = &StyleInstance<ScrollViewStyle>::GetIntance();
    this->setStyle(pStyle);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::transparent);
    this->viewport()->setBackgroundRole(QPalette::Background);
    this->viewport()->setPalette(palette);

    this->setResizeMode(QListView::Adjust);

    shadowLeft_ = new QGraphicsDropShadowEffect(this);
    if (shadowLeft_) {
        shadowLeft_->setColor(QColor(190, 190, 190));

        shadowLeft_->setOffset(-2, 2);

        shadowLeft_->setBlurRadius(5);
        this->setGraphicsEffect(shadowLeft_);
    }
};

void ScrollEditList::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QListView::currentChanged(current, previous);
    if (current.isValid()) {
        emit CurrentItem(current);
    } else {
        QModelIndex currentnull;
        emit CurrentItem(currentnull);
    }
    return;
};

void ScrollEditList::resizeEvent(QResizeEvent *event)
{
    QListView::resizeEvent(event);
    QPainterPath path2;

    int nMinVal = this->width() > this->height() ? this->height() : this->width();

    this->setSpacing(nMinVal / 8);

    path2.addRoundedRect(this->rect(), nMinVal / 5, nMinVal / 5);
    this->setMask(path2.toFillPolygon().toPolygon());
}

//----------------------------------------------------------------------------------
SmplScrollEdit::SmplScrollEdit(QWidget *parent)
    : QLineEdit(parent)
    , pListView_(nullptr)
    , nValueResult_(0)
    , nValueProcess_(0)
    , nStep_(0)
    , bIsLeftPressed_(false)
    , bIsRightPressed_(false)
    , bIsPressed_(false)
    , bIsMoving_(false)
    , bIsReleased_(true)
{
    this->setStyle(&style_);

    this->setReadOnly(true);
    this->setAlignment(Qt::AlignCenter);

    pListView_ = new ScrollEditList(parent);
    if (pListView_) {
        pListView_->hide();
        pListView_->setSelectionMode(QAbstractItemView::SingleSelection);
        connect(pListView_, &ScrollEditList::CurrentItem, this, [&](const QModelIndex &current) {
            if (current.isValid()) {
                QVariant data = current.data();
                this->setText(data.toString());
            } else {
                this->setText("");
            }
        });

        if (pListView_->model()) {
            QModelIndex index = pListView_->model()->index(nValueResult_, 0);
            if (index.isValid()) {
                pListView_->setCurrentIndex(index);
            }
        }
    }
}

ScrollEditList *SmplScrollEdit::GetListView() const { return pListView_; }

void SmplScrollEdit::SetModel(QAbstractItemModel *model)
{
    if (model) {
        pListView_->setModel(model);
        nValueResult_ = 0;
        QModelIndex index = pListView_->model()->index(nValueResult_, 0);
        if (index.isValid()) {
            pListView_->setCurrentIndex(index);
        }
    }
}

int SmplScrollEdit::GetCurrentIndex()
{
    if (!pListView_->model()) {
        return -1;
    }
    QModelIndex index = this->GetListView()->currentIndex();
    if (index.isValid()) {
        return index.row();
    } else {
        return -1;
    }
}

void SmplScrollEdit::resizeEvent(QResizeEvent *event)
{
    QLineEdit::resizeEvent(event);
    pListView_->resize(1.25 * this->width(), this->height());
}

void SmplScrollEdit::mousePressEvent(QMouseEvent *event)
{
    bIsPressed_ = true;
    bIsReleased_ = false;
    pointLast_ = event->pos();
    nStep_ = 0;
    nValueProcess_ = 0;

    if (!bIsMoving_ && pListView_->model() && pListView_->model()->rowCount() != 0) {
        if (pointLast_.x() - width() / 2 <= 0) {
            bIsLeftPressed_ = true;
        } else {
            bIsRightPressed_ = true;
        }
        update();
    }

    return QLineEdit::mousePressEvent(event);
}

void SmplScrollEdit::mouseMoveEvent(QMouseEvent *event)
{
    if (bIsPressed_ && pListView_->model() && pListView_->model()->rowCount() != 0) {

        int nPaddingItem = rect().width() / (pListView_->model()->rowCount());

        if (rect().contains(event->pos()) && (2 * abs(event->pos().x() - pointLast_.x()) >= 1)) {
            bIsMoving_ = true;
            if (bIsPressed_ && bIsMoving_) {
                bIsLeftPressed_ = false;
                bIsRightPressed_ = false;
                if (pListView_->isHidden()) {
                    QPoint pointParent = mapTo((QWidget *)this->parent(), this->rect().topLeft());
                    pListView_->move(pointParent.x() + this->width() / 2 - pListView_->width() / 2,
                                     pointParent.y() - pListView_->height());
                    pListView_->show();
                }

                nValueProcess_ = event->pos().x() - pointLast_.x();
                nStep_ = nValueProcess_ / nPaddingItem;

                int nTmp = nValueResult_ + nStep_;
                if (nTmp >= pListView_->model()->rowCount()) {
                    nTmp = pListView_->model()->rowCount() - 1;
                }
                if (nTmp < 0) {
                    nTmp = 0;
                }

                QModelIndex index = pListView_->model()->index(nTmp, 0);
                if (index.isValid()) {
                    pListView_->scrollTo(index, QAbstractItemView::PositionAtCenter);
                    pListView_->setCurrentIndex(index);

                    pListView_->viewport()->update();
                    pListView_->update();
                }
            }
        }
    }

    return QLineEdit::mouseMoveEvent(event);
}

void SmplScrollEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if (!bIsMoving_ && pListView_->model() && pListView_->model()->rowCount() != 0) {
        if (pointLast_.x() - width() / 2 <= 0) {
            ++nValueResult_;
        } else {
            --nValueResult_;
        }
        update();
        QModelIndex index = pListView_->model()->index(nValueResult_, 0);
        if (index.isValid()) {
            pListView_->setCurrentIndex(index);
            pListView_->viewport()->update();
            pListView_->update();
        }
    }

    bIsPressed_ = false;
    bIsReleased_ = true;

    if (pListView_->model() && pListView_->model()->rowCount() != 0) {
        nValueResult_ += nStep_;
        if (nValueResult_ >= pListView_->model()->rowCount()) {
            nValueResult_ = pListView_->model()->rowCount() - 1;
        }
        if (nValueResult_ < 0) {
            nValueResult_ = 0;
        }

        QModelIndex index = pListView_->model()->index(nValueResult_, 0);
        if (index.isValid()) {
            pListView_->setCurrentIndex(index);

            pListView_->viewport()->update();
            pListView_->update();
        }
    }

    if (!pListView_->isHidden() && bIsMoving_) {
        pListView_->hide();
    }

    bIsMoving_ = false;

    bIsLeftPressed_ = false;
    bIsRightPressed_ = false;

    update();
    return QLineEdit::mouseReleaseEvent(event);
}

void SmplScrollEdit::paintEvent(QPaintEvent *event)
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
    QLinearGradient backgroundBtn = QLinearGradient(0, 0, 0, rect().height());
    backgroundBtn.setColorAt(0, QColor(47, 104, 212));
    backgroundBtn.setColorAt(1, QColor(33, 51, 123));

    painter.save();
    if (bIsLeftPressed_) {
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

    if (bIsRightPressed_) {
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

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
void ScrollViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                    const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    switch (element) {
    case PE_PanelItemViewItem: {
        if (option) {

            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::NoBrush);

            painter->drawRect(option->rect);
            QProxyStyle::drawPrimitive(element, option, painter, widget);
        }

        return;
    }
    default: {
        break;
    }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void ScrollViewStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                  const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    switch (element) {
    case CE_ShapedFrame: {
        QPainterPath pathRoundRect;

        int nWidthRect = option->rect.width();
        int nHeightRect = option->rect.height() * 6 / 7;
        int nRadius = nWidthRect > nHeightRect ? nHeightRect : nWidthRect;
        pathRoundRect.addRoundedRect(0, 0, nWidthRect, nHeightRect, nRadius / 5, nRadius / 5);

        QPainterPath pathTriangle;
        pathTriangle.moveTo(nWidthRect / 2 - option->rect.height() / 7, option->rect.height() * 6 / 7);
        pathTriangle.lineTo(nWidthRect / 2 + option->rect.height() / 7, option->rect.height() * 6 / 7);
        pathTriangle.lineTo(nWidthRect / 2, option->rect.height());
        pathTriangle.closeSubpath();

        QPainterPath pathCombined = pathRoundRect.united(pathTriangle);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(243, 248, 255));
        painter->drawPath(pathCombined);

        return;
    }
    case CE_ItemViewItem: {
        QRect rc = widget->rect();
        rc.adjust(0, 0, -2, -2);
        QPainterPath path2;
        int nRadius2 = rc.width() > rc.height() ? rc.height() : rc.width();
        path2.addRoundedRect(rc, nRadius2 / 4, nRadius2 / 4);

        painter->save();
        painter->translate(0, -option->rect.y() * 3 / 4);

        QPainterPath path1;
        int nRadius = option->rect.width() > option->rect.height() ? option->rect.height() : option->rect.width();
        path1.addRoundedRect(option->rect, nRadius / 4, nRadius / 4);
        QPainterPath path3 = path2.intersected(path1);

        painter->setClipPath(path3);
        drawPrimitive(PE_PanelItemViewItem, option, painter, widget);

        const QStyleOptionViewItem *optionViewItem = qstyleoption_cast<const QStyleOptionViewItem *>(option);
        if (option->state & StateFlag::State_Selected) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(191, 218, 255));
            painter->drawRect(option->rect);
        }
        painter->setPen(Qt::black);

        painter->drawText(option->rect, Qt::AlignCenter, optionViewItem->text);
        painter->restore();
        return;
    }
    default:
        break;
    }
    QProxyStyle::drawControl(element, option, painter, widget);
};

} // namespace ScrollEdit
} // namespace Ui
