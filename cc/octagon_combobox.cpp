#include "octagon_combobox.h"
#include <QAbstractItemView>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>
#include <QScrollBar>

namespace Ui {
namespace OctagonComboBox {
void OctagonComboBoxListView::paintEvent(QPaintEvent *event)
{
    QListView::paintEvent(event);

    QPainter painter(viewport());
    painter.setRenderHints(QPainter::HighQualityAntialiasing);

    QBrush brushLTPress(QColor(166, 166, 166));
    QBrush brushRBRelease(QColor(255, 255, 255, 255));

    QPen pen1(brushRBRelease, 3);
    painter.setPen(pen1);
    painter.drawLine(0, viewport()->height() - 2, viewport()->width(), viewport()->height() - 2);

    QPen pen2(brushRBRelease, 3);
    painter.setPen(pen2);
    painter.drawLine(viewport()->width() - 2, 0, viewport()->width() - 2, viewport()->height());

    QPen pen3(brushLTPress, 5);
    painter.setPen(pen3);
    painter.drawLine(0, 0, 0, viewport()->height());
}

OctagonComboBox::OctagonComboBox(QWidget *parent)
    : QComboBox(parent)
{
    this->setStyle(&styleCustom_);
    setStyleSheet(QString("QComboBox QAbstractItemView               "
                          "{                                         "
                          "background:rgb(207,207,207);                          "
                          "}                                         "));

    setView(&viewCustom_);
}

OctagonComboBox::~OctagonComboBox() { }

void OctagonComboBox::paintEvent(QPaintEvent *event)
{
    QComboBox::paintEvent(event);

    QPainterPath path;
    path.moveTo(QPoint(nPadding_, 0));
    path.lineTo(QPoint(nWidth_ - nPadding_, 0));
    path.lineTo(QPoint(nWidth_, nPadding_));
    path.lineTo(QPoint(nWidth_, nHeight_ - nPadding_));
    path.lineTo(QPoint(nWidth_ - nPadding_, nHeight_));
    path.lineTo(QPoint(nPadding_, nHeight_));
    path.lineTo(QPoint(0, nHeight_ - nPadding_));
    path.lineTo(QPoint(0, nPadding_));
    path.closeSubpath();
    QRegion region1(path.toFillPolygon().toPolygon());
    this->setMask(region1);

    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setPen(QPen(QColor(130, 130, 130), 2));
    painter.drawPolygon(path.toFillPolygon().toPolygon());
    painter.drawLine(nPadding_, nHeight_, 0, nHeight_ - nPadding_);
    painter.drawLine(nWidth_ - nPadding_, nHeight_, nPadding_, nHeight_);
}

void OctagonComboBox::showPopup()
{
    QComboBox::showPopup();
    // QComboBox::showPopup();
    QWidget *popupWidget = findChild<QWidget *>();

    QPolygonF octagon;
    int nTmpWidth = popupWidget->width();
    int nTmpHeight = popupWidget->height();

    QPainterPath path;
    path.moveTo(QPoint(0, 0));
    path.lineTo(QPoint(nTmpWidth, 0));
    path.lineTo(QPoint(nTmpWidth, nTmpHeight - nPadding_));
    path.lineTo(QPoint(nTmpWidth - nPadding_, nTmpHeight));
    path.lineTo(QPoint(nPadding_, nTmpHeight));
    path.lineTo(QPoint(0, nTmpHeight - nPadding_));
    path.lineTo(QPoint(0, 0));
    path.closeSubpath();

    QPolygonF tmp1 = path.toFillPolygon();

    QPainterPath path1;
    path1.moveTo(QPoint(0, 0));
    path1.lineTo(QPoint(nTmpWidth, 0));
    path1.lineTo(QPoint(nTmpWidth - nPadding_ - 1, nPadding_ + 1));
    path1.lineTo(QPoint(nPadding_ + 1, nPadding_ + 1));
    path1.lineTo(QPoint(0, 0));
    path1.closeSubpath();
    QPolygonF tmp2 = path1.toFillPolygon();

    QRegion region1(tmp1.subtracted(tmp2).toPolygon());

    QPoint gp = this->mapToGlobal(this->rect().bottomLeft());
    gp.setY(gp.y() - nPadding_);
    popupWidget->move(gp);
    popupWidget->setMask(region1);
}

void OctagonComboBox::resizeEvent(QResizeEvent *event)
{
    QComboBox::resizeEvent(event);

    nWidth_ = this->width();
    nHeight_ = this->height();
    nPadding_ = this->height() / 4;

    nTmpX1_ = nWidth_ - nPadding_;
    nWidthCtrl_ = nWidth_ - 2 * nPadding_;
    nLengthCtrl_ = nHeight_ - 2 * nPadding_;
    nHypotenuse_ = nPadding_ * fGEN_2_;
}

OctagonComboBoxStyle::OctagonComboBoxStyle() { }
void OctagonComboBoxStyle::drawComplexControl(ComplexControl which, const QStyleOptionComplex *option,
                                              QPainter *painter, const QWidget *widget) const
{
    if (which == CC_ComboBox) {
        if (const QStyleOptionComboBox *cbOption = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
            painter->save();
            QRect rect = subControlRect(CC_ComboBox, option, SC_ComboBoxFrame).adjusted(0, 0, 0, 0);
            painter->setPen(Qt::transparent);
            painter->setBrush(QColor(207, 207, 207));
            painter->drawRect(rect);

            const double fGEN_2 = 1.4142135;
            int nWidthLTBorder = 4;
            int nWidthRBBorder = 3;

            int nWidth = rect.width();
            int nHeight = rect.height();
            int nPadding = rect.height() / 4;

            int nHypotenuse = nPadding * fGEN_2;

            QBrush brushLTPress(QColor(166, 166, 166));
            QBrush brushRBRelease(QColor(255, 255, 255, 255));

            // 1
            QPolygonF octagonPress1;
            octagonPress1 << QPoint(0, 0);
            octagonPress1 << QPoint(nWidth, 0);
            octagonPress1 << QPoint(nWidth, nWidthLTBorder);
            octagonPress1 << QPoint(0, nWidthLTBorder);
            painter->setPen(Qt::NoPen);
            painter->setBrush(brushLTPress);
            painter->drawPolygon(octagonPress1);

            // 2
            painter->save();
            painter->translate(nWidth, 0);

            // 3
            painter->rotate(90);
            QPolygonF octagonPress3;
            octagonPress3 << QPoint(nWidthLTBorder, 0);
            octagonPress3 << QPoint(nHeight, 0);
            octagonPress3 << QPoint(nHeight, nWidthRBBorder);
            octagonPress3 << QPoint(nWidthLTBorder, nWidthRBBorder);

            // 4
            painter->translate(nHeight, 0);
            painter->rotate(90);

            // 5
            QPolygonF octagonPress5;
            octagonPress5 << QPoint(0, 0);
            octagonPress5 << QPoint(nWidth, 0);
            octagonPress5 << QPoint(nWidth, nWidthRBBorder);
            octagonPress5 << QPoint(0, nWidthRBBorder);
            painter->setPen(Qt::NoPen);
            painter->setBrush(brushRBRelease);
            painter->drawPolygon(octagonPress5);

            // 6
            painter->translate(nWidth, 0);
            painter->rotate(90);
            QPolygonF octagonPress7;
            octagonPress7 << QPoint(0, 0);
            octagonPress7 << QPoint(nHeight, 0);
            octagonPress7 << QPoint(nHeight, nWidthLTBorder);
            octagonPress7 << QPoint(0, nWidthLTBorder);
            painter->setBrush(brushLTPress);
            painter->drawPolygon(octagonPress7);

            // 8
            painter->translate(nHeight - nPadding, 0);
            painter->rotate(45);
            painter->fillRect(0, 0, nHypotenuse, nWidthLTBorder, brushLTPress);

            painter->restore();
            painter->restore();

            rect = subControlRect(CC_ComboBox, option, SC_ComboBoxArrow).adjusted(0, 0, 0, 0);

            painter->save();
            painter->setPen(Qt::transparent);
            painter->translate(option->rect.x(), option->rect.y());
            painter->setBrush(QColor(207, 207, 207));
            painter->drawRect(rect);
            painter->fillRect(0, 0, widget->width(), nWidthLTBorder, brushLTPress);
            painter->fillRect(0, option->rect.height() - nWidthRBBorder, widget->width(), nWidthRBBorder,
                              brushRBRelease);
            painter->restore();

            QStyleOption arrowOpt(*cbOption);
            arrowOpt.rect = rect;
            drawPrimitive(PE_IndicatorArrowDown, &arrowOpt, painter);
        }

    } else {
        return QProxyStyle::drawComplexControl(which, option, painter, widget);
    }
}

void OctagonComboBoxStyle::drawPrimitive(PrimitiveElement which, const QStyleOption *option, QPainter *painter,
                                         const QWidget *widget) const
{
    switch (which) {
    case PE_IndicatorArrowDown: {

        painter->setRenderHints(QPainter::HighQualityAntialiasing);
        int nWidthBorder_ = 2;
        int nWidthLTBorder_ = 4;
        int nWidthRBBorder_ = 3;
        int nWidth = option->rect.width();
        int nHeight = option->rect.height();
        int nPadding = option->rect.height() / 4;

        int nTmpX1 = nWidth - nPadding;
        int nWidthCtrl = nWidth - 2 * nPadding;

        int nLengthCtrl = nHeight - 2 * nPadding;

        int nHypotenuse = nPadding * 1.4142135;

        painter->save();
        painter->translate(option->rect.x(), option->rect.y());
        QPolygonF octagon;
        octagon << QPoint(nPadding, 0);                // 上左
        octagon << QPoint(nWidth - nPadding, 0);       // 上右
        octagon << QPoint(nWidth, nPadding);           // 右上
        octagon << QPoint(nWidth, nHeight - nPadding); // 右下
        octagon << QPoint(nWidth - nPadding, nHeight); // 下右
        octagon << QPoint(nPadding + 1, nHeight);      // 下左
        octagon << QPoint(0, nHeight - nPadding - 1);  // 左下
        octagon << QPoint(0, nPadding);                // 左上
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(207, 207, 207, 255));
        painter->drawPolygon(octagon);

        if (option->state & State_MouseOver) {
            //  1
            QBrush brushLTPress(QColor(166, 166, 166));
            QBrush brushRBRelease(QColor(245, 245, 245));

            painter->fillRect(0, 0, nWidth, nWidthLTBorder_, brushLTPress);

            // 2
            painter->translate(nTmpX1, 0);

            painter->rotate(45);

            // 3
            painter->translate(nHypotenuse, 0);
            painter->rotate(45);
            painter->fillRect(0, 0, nLengthCtrl, nWidthRBBorder_, brushRBRelease);

            // 4
            painter->translate(nLengthCtrl, 0);
            painter->rotate(45);
            painter->fillRect(0, 0, nHypotenuse, nWidthRBBorder_, brushRBRelease);

            // 5
            painter->translate(nHypotenuse, 0);
            painter->rotate(45);
            QPolygonF octagonPress5;
            octagonPress5 << QPoint(0, 0);                                          // 左上
            octagonPress5 << QPoint(nWidthCtrl, 0);                                 // 右上
            octagonPress5 << QPoint(nWidthCtrl + nWidthRBBorder_, nWidthRBBorder_); // 右下
            octagonPress5 << QPoint(0, nWidthRBBorder_);
            painter->setPen(Qt::NoPen);
            painter->setBrush(brushRBRelease);
            painter->drawPolygon(octagonPress5);

            // 6
            painter->translate(nWidthCtrl, 0);
            painter->rotate(45);

            // 7
            painter->translate(nHypotenuse, 0);
            painter->rotate(45);
            QPolygonF octagonPress7;
            octagonPress7 << QPoint(0, 0);                         // 左上
            octagonPress7 << QPoint(nLengthCtrl, 0);               // 右上
            octagonPress7 << QPoint(nLengthCtrl, nWidthLTBorder_); // 右下
            octagonPress7 << QPoint(-nWidthLTBorder_, nWidthLTBorder_);
            painter->setBrush(brushLTPress);
            painter->drawPolygon(octagonPress7);

            // 8
            painter->translate(nLengthCtrl, 0);
            painter->rotate(45);
            QLinearGradient background7 = QLinearGradient(0, 0, 0, nWidthLTBorder_);
            background7.setColorAt(0, QColor(130, 130, 130).darker(90));
            background7.setColorAt(0.7, QColor(130, 130, 130).darker(50));
            background7.setColorAt(1, QColor(130, 130, 130).lighter(300));
            painter->fillRect(0, 0, nHypotenuse, nWidthLTBorder_, brushLTPress);

        } else {
            // 1
            QLinearGradient background = QLinearGradient(0, 0, 0, nWidthLTBorder_); // 横线  上灰下白
            background.setColorAt(0, QColor(130, 130, 130).darker(90));
            background.setColorAt(0.7, QColor(130, 130, 130).darker(50));
            background.setColorAt(1, QColor(130, 130, 130).lighter(300));
            painter->fillRect(nPadding, 0, nWidth - 2 * nPadding, nWidthLTBorder_, background);

            // 2
            painter->translate(nTmpX1, 0);
            painter->rotate(45);
            QLinearGradient background1 = QLinearGradient(0, 0, 0, nWidthBorder_);
            background1.setColorAt(0, QColor(130, 130, 130).darker(100));
            background1.setColorAt(0.15, QColor(130, 130, 130).darker(90));
            background1.setColorAt(0., QColor(207, 207, 207));
            painter->fillRect(0, 0, nHypotenuse, nWidthBorder_, background1);

            // 3
            painter->translate(nHypotenuse, 0);
            painter->rotate(45);
            QLinearGradient background2 = QLinearGradient(0, 0, 0, nWidthRBBorder_);
            background2.setColorAt(0, QColor(130, 130, 130).darker(120));
            background2.setColorAt(0.2, QColor(130, 130, 130).darker(110));
            background2.setColorAt(0.6, QColor(130, 130, 130).darker(70));
            background2.setColorAt(0.7, QColor(207, 207, 207));
            painter->fillRect(0, 0, nLengthCtrl, nWidthRBBorder_, background2);

            // 4
            painter->translate(nLengthCtrl, 0);
            painter->rotate(45);
            QLinearGradient background3 = QLinearGradient(0, 0, 0, nWidthRBBorder_);
            background3.setColorAt(0, QColor(130, 130, 130).darker(120));
            background3.setColorAt(0.3, QColor(130, 130, 130).darker(100));
            background3.setColorAt(0.5, QColor(130, 130, 130).darker(80));
            background3.setColorAt(0.9, QColor(207, 207, 207));
            painter->fillRect(0, 0, nHypotenuse, nWidthRBBorder_, background3);

            // 5
            painter->translate(nHypotenuse, 0);
            painter->rotate(45);
            QLinearGradient background4 = QLinearGradient(0, 0, 0, nWidthRBBorder_);
            background4.setColorAt(0, QColor(130, 130, 130).darker(120));
            background4.setColorAt(0.3, QColor(130, 130, 130).darker(90));
            background4.setColorAt(0.5, QColor(130, 130, 130).darker(70));
            background3.setColorAt(0.9, QColor(207, 207, 207));
            painter->fillRect(0, 0, nWidthCtrl, nWidthRBBorder_, background4);

            // 6
            painter->translate(nWidthCtrl, 0);
            painter->rotate(45);
            QLinearGradient background5 = QLinearGradient(0, 0, 0, nWidthBorder_);
            background5.setColorAt(0, QColor(130, 130, 130).darker(100));
            background5.setColorAt(0.15, QColor(130, 130, 130).darker(90));
            background5.setColorAt(0.9, QColor(207, 207, 207));
            painter->fillRect(0, 0, nHypotenuse, nWidthBorder_, background5);

            // 7
            painter->translate(nHypotenuse, 0);
            painter->rotate(45);
            QLinearGradient background6 = QLinearGradient(0, 0, 0, nWidthLTBorder_);
            background6.setColorAt(0, QColor(130, 130, 130).darker(90));
            background6.setColorAt(0.7, QColor(130, 130, 130).darker(50));
            background6.setColorAt(1, QColor(130, 130, 130).lighter(300));
            painter->fillRect(0, 0, nLengthCtrl, nWidthLTBorder_, background6);

            // 8
            painter->translate(nLengthCtrl, 0);
            painter->rotate(45);
            QLinearGradient background7 = QLinearGradient(0, 0, 0, nWidthLTBorder_);
            background7.setColorAt(0, QColor(130, 130, 130).darker(90));
            background7.setColorAt(0.7, QColor(130, 130, 130).darker(50));
            background7.setColorAt(1, QColor(130, 130, 130).lighter(300));
            painter->fillRect(0, 0, nHypotenuse, nWidthLTBorder_, background7);
        }
        painter->restore();

        painter->save();
        painter->translate(option->rect.x(), option->rect.y());
        painter->setPen(QPen(QColor(130, 130, 130), 1));
        painter->drawPolygon(octagon);
        painter->restore();

        painter->save();
        painter->translate(option->rect.x(), option->rect.y());
        QPainterPath drawtriangle;

        if (option->state & State_MouseOver) {
            drawtriangle.moveTo(option->rect.width() / 2, option->rect.height() * 1 / 3);
            drawtriangle.lineTo(option->rect.width() * 2 / 3, option->rect.height() * 2 / 3);
            drawtriangle.lineTo(option->rect.width() / 3, option->rect.height() * 2 / 3);

            drawtriangle.lineTo(option->rect.width() / 2, option->rect.height() * 1 / 3);

            drawtriangle.closeSubpath();
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::red);
        } else {
            drawtriangle.moveTo(option->rect.width() / 3, option->rect.height() / 3);
            drawtriangle.lineTo(option->rect.width() * 2 / 3, option->rect.height() / 3);
            drawtriangle.lineTo(option->rect.width() / 2, option->rect.height() * 2 / 3);
            drawtriangle.lineTo(option->rect.width() / 3, option->rect.height() / 3);
            drawtriangle.closeSubpath();
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::black);
        }

        painter->drawPath(drawtriangle);
        painter->restore();
    } break;
    default:
        QProxyStyle::drawPrimitive(which, option, painter, widget);
    }
}

QRect OctagonComboBoxStyle::subControlRect(ComplexControl whichControl, const QStyleOptionComplex *option,
                                           SubControl whichSubControl, const QWidget *widget) const
{
    if (whichControl == CC_ComboBox) {
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

void OctagonComboBoxStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                       const QWidget *widget) const
{
    if (element == CE_ComboBoxLabel) {
        const QStyleOptionComboBox *comboBoxOption = qstyleoption_cast<const QStyleOptionComboBox *>(option);
        if (comboBoxOption) {
            QRect textRect = comboBoxOption->rect;
            textRect.adjust(0, 0, -textRect.height(), 0);
            painter->setFont(QFont("Arial", 12));
            if (comboBoxOption->state & State_Enabled) {
                painter->setPen(comboBoxOption->palette.foreground().color());

                painter->drawText(textRect, Qt::AlignCenter, comboBoxOption->currentText);
            } else {
                painter->setPen(comboBoxOption->palette.foreground().color());
                painter->drawText(textRect, Qt::AlignCenter, comboBoxOption->currentText);
            }
            return;
        }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}
} // namespace OctagonComboBox
} // namespace Ui
