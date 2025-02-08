#include "composite_tablewidget.h"
namespace Ui {
namespace BNECombo {
SubComboBox::SubComboBox(QWidget *parent)
    : QComboBox(parent)
    , bShowView_(false)
{
    this->setWindowFlags(windowFlags());
    this->setStyle(&style_);
    this->setStyleSheet("QComboBox{combobox-popup:0;}");
    this->setMaxVisibleItems(10);
    this->setEditable(false);
    view()->setStyle(&styleList_);
    view()->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    view()->setItemDelegateForColumn(0, new ListItemDelegate());
    if (view()->verticalScrollBar()) {

        view()->verticalScrollBar()->setStyle(&styleScroll_);

        view()->verticalScrollBar()->setAttribute(Qt::WA_OpaquePaintEvent, false); // WA_NoBackground
    }
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(219, 234, 255));
    view()->viewport()->setBackgroundRole(QPalette::Background);
    view()->viewport()->setPalette(palette);

    this->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->SetAlignments(Qt::AlignVCenter | Qt::AlignLeft);

    connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&]() { clearFocus(); });
    connect(this, &SubComboBox::popUp, this, [&]() { this->showPopup(); });
}

void SubComboBox::SetAlignments(Qt::Alignment align) { nFlagAlign_ = align; }

Qt::Alignment SubComboBox::GetAlignment() const { return nFlagAlign_; }

bool SubComboBox::isViewShowing() const { return bShowView_; };

void SubComboBox::paintEvent(QPaintEvent *event)
{
    QComboBox::paintEvent(event);

    QPainterPath path;
    path.addRoundedRect(rect().adjusted(1, 1, -1, -1), height() / 3, height() / 3);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(98, 162, 227), 2));
    painter.drawRect(rect());
};

void SubComboBox::showPopup()
{
    QComboBox::showPopup();
    QWidget *widget = findChild<QWidget *>();

    QPainterPath path;
    path.addRoundedRect(widget->rect().adjusted(1, 1, -1, -1), 10, 10);

    QPoint gp = this->mapToGlobal(this->rect().bottomLeft());
    gp.setY(gp.y());
    widget->move(gp);
    widget->setMask(path.toFillPolygon().toPolygon());
    widget->setAutoFillBackground(true);
    bShowView_ = true;
}

void SubComboBox::hidePopup()
{
    QComboBox::hidePopup();
    bShowView_ = false;
    return;
}

QString SubComboBox::currentText() const { return QComboBox::currentText(); }

//----------------------------------------------------------------------------------------------------------
ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

ComboBoxDelegate::ComboBoxDelegate(const QList<std::pair<QString, int>> &list, QObject *parent)
    : QItemDelegate(parent)
    , listComboBox_(list)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    SubComboBox *editor = new SubComboBox(parent);
    if (!listComboBox_.isEmpty()) {
        editor->AddPairList<int>(listComboBox_);
    }

    int nValue = index.model()->data(index, Qt::UserRole).toInt();
    editor->SetCurrentIndexByUserData(nValue);

    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    return QItemDelegate::setEditorData(editor, index);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QItemDelegate::setModelData(editor, model, index);
    SubComboBox *pTmp = dynamic_cast<SubComboBox *>(editor);
    if (pTmp) {
        model->setData(index, pTmp->GetCurrentIndexUserData(), Qt::UserRole);
        model->setData(index, pTmp->currentText(), Qt::DisplayRole);
    }
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    QItemDelegate::updateEditorGeometry(editor, option, index);

    SubComboBox *pTmp = dynamic_cast<SubComboBox *>(editor);
    if (pTmp) {
        QMouseEvent *pressEvent = new QMouseEvent(QEvent::MouseButtonPress, pTmp->rect().center(), Qt::LeftButton,
                                                  Qt::LeftButton, Qt::NoModifier);

        QMouseEvent *releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease, pTmp->rect().center(), Qt::LeftButton,
                                                    Qt::LeftButton, Qt::NoModifier);

        QCoreApplication::postEvent(pTmp, pressEvent);
        QCoreApplication::postEvent(pTmp, releaseEvent);
    }
};

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setPen(QPen(QColor(174, 184, 220), 1));
    painter->setBrush(QColor(219, 234, 255));
    QRect rect = option.rect;
    rect.adjust(0, 0, 1, 1);
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    painter->drawLine(rect.topRight(), rect.bottomRight());
    QItemDelegate::paint(painter, option, index);

    return;
};

bool ComboBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                                   const QModelIndex &index)
{
    return QItemDelegate::editorEvent(event, model, option, index);
};

//------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
SubComboBoxStyle::SubComboBoxStyle() { }

void SubComboBoxStyle::drawComplexControl(ComplexControl which, const QStyleOptionComplex *option, QPainter *painter,
                                          const QWidget *widget) const
{
    if (which == CC_ComboBox) {
        const QStyleOptionComboBox *cbOption = qstyleoption_cast<const QStyleOptionComboBox *>(option);
        if (cbOption) {
            painter->setRenderHints(QPainter::Antialiasing);
            painter->save();
            QRect rect = subControlRect(CC_ComboBox, option, SC_ComboBoxFrame).adjusted(0, 0, 0, 0);
            painter->setPen(Qt::transparent);
            painter->setBrush(QColor(219, 234, 255));
            painter->drawRect(rect);

            painter->restore();
            rect = subControlRect(CC_ComboBox, option, SC_ComboBoxArrow).adjusted(0, 0, 0, 0);

            QStyleOption arrowOpt(*cbOption);
            arrowOpt.rect = rect;
            drawPrimitive(PE_IndicatorArrowDown, &arrowOpt, painter, widget);
        }

    } else {
        return QProxyStyle::drawComplexControl(which, option, painter, widget);
    }
}

void SubComboBoxStyle::drawPrimitive(PrimitiveElement which, const QStyleOption *option, QPainter *painter,
                                     const QWidget *widget) const
{
    switch (which) {
    case PE_IndicatorArrowDown: {
        painter->setRenderHints(QPainter::Antialiasing);

        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(219, 234, 255));
        painter->drawRect(option->rect);
        painter->restore();

        const SubComboBox *pTmp = qobject_cast<const SubComboBox *>(widget);
        if (pTmp == nullptr) {
            return;
        } else {
            painter->save();

            painter->translate(option->rect.x(), option->rect.y());
            QPainterPath drawtriangle;
            if (pTmp->isViewShowing()) {
                drawtriangle.moveTo(option->rect.width() / 3, option->rect.height() * 2 / 3);
                drawtriangle.lineTo(option->rect.width() / 2, option->rect.height() * 1 / 3);
                drawtriangle.lineTo(option->rect.width() * 2 / 3, option->rect.height() * 2 / 3);
            } else {
                drawtriangle.moveTo(option->rect.width() / 3, option->rect.height() / 3);
                drawtriangle.lineTo(option->rect.width() / 2, option->rect.height() * 2 / 3);
                drawtriangle.lineTo(option->rect.width() * 2 / 3, option->rect.height() / 3);
            }

            painter->setPen(QPen(QColor(157, 167, 222), 2));
            painter->setBrush(Qt::NoBrush);

            painter->drawPath(drawtriangle);

            //----------------------------------------------------------------
            painter->restore();
        }

        break;
    }
    default:
        QProxyStyle::drawPrimitive(which, option, painter, widget);
    }
}

QRect SubComboBoxStyle::subControlRect(ComplexControl whichControl, const QStyleOptionComplex *option,
                                       SubControl whichSubControl, const QWidget *widget) const
{
    if (whichControl == CC_ComboBox && option) {
        switch (whichSubControl) {
        case SC_ComboBoxFrame:
            return QRect(0, 0, option->rect.width() - option->rect.height(), option->rect.height());
        case SC_ComboBoxArrow:
            return QRect(option->rect.width() - option->rect.height(), 0, option->rect.height(), option->rect.height());
        default:
            return QProxyStyle::subControlRect(whichControl, option, whichSubControl, widget);
        }
    } else {
        return QProxyStyle::subControlRect(whichControl, option, whichSubControl, widget);
    }
}

void SubComboBoxStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                   const QWidget *widget) const
{
    if (element == CE_ComboBoxLabel) {
        const SubComboBox *pCombo = static_cast<const SubComboBox *>(widget);
        const QStyleOptionComboBox *comboBoxOption = qstyleoption_cast<const QStyleOptionComboBox *>(option);
        if (comboBoxOption && pCombo) {
            QRect textRect = comboBoxOption->rect;
            textRect.adjust(3, 0, -1 * option->rect.height(), 0);
            painter->setPen(Qt::black);
            painter->setBrush(Qt::NoBrush);

            painter->drawText(widget->rect(), pCombo->GetAlignment(), comboBoxOption->currentText);
            return;
        } else {
            return QProxyStyle::drawControl(element, option, painter, widget);
        }
    }

    return QProxyStyle::drawControl(element, option, painter, widget);
}
} // namespace BNECombo
} // namespace Ui
