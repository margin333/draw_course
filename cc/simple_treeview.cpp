#include "simple_treeview.h"
namespace Ui {
namespace SmpTree {
SimpleTreeView::SimpleTreeView(QWidget *parent)
    : QTreeView(parent)
{
    this->setStyle(&style_);
    QPalette palette = viewport()->palette();
    palette.setColor(QPalette::Base, QColor(219, 234, 255));

    this->viewport()->setPalette(palette);

    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    if (this->verticalScrollBar()) {
        this->verticalScrollBar()->setStyle(&styleBar_);

        this->verticalScrollBar()->setAttribute(Qt::WA_OpaquePaintEvent, false);
    }
    this->setViewportMargins(0, 10, 0, 10);
    setMouseTracking(true);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, [&]([[maybe_unused]] int value) {
        this->update();
        this->viewport()->update();
        this->verticalScrollBar()->update();
    });
}

void SimpleTreeView::paintEvent(QPaintEvent *e) { return QTreeView::paintEvent(e); }

void SimpleTreeView::resizeEvent(QResizeEvent *e)
{
    QTreeView::resizeEvent(e);
    QPainterPath path;
    path.addRoundedRect(this->rect(), 10, 10);
    this->setMask(path.toFillPolygon().toPolygon());

    QPoint relativePos0 = viewport()->mapTo(this, viewport()->pos());
    QRect rect1 = this->rect();
    rect1.adjust(0, 0, relativePos0.x() - 1, relativePos0.y() - 1);
    QPainterPath path1;

    path1.translate(relativePos0.x(), relativePos0.y());
    path1.addRoundedRect(rect1, 10, 10);
    viewport()->setMask(path1.toFillPolygon().toPolygon());

    QPoint relativePos1 = verticalScrollBar()->mapTo(this, verticalScrollBar()->pos());
    QRect rect2 = this->rect();
    rect2.adjust(1, 1, -1, -4 + relativePos1.y());
    QPainterPath path2;
    path2.addRoundedRect(rect2, 10, 10);

    path2.translate(-relativePos1.x(), -relativePos1.y());
    verticalScrollBar()->setMask(path2.toFillPolygon().toPolygon());
    return;
}

//------------------------------------------------------------------------------
CommonTreeStyle::CommonTreeStyle()
    : QProxyStyle()
    , iconAll_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_tree_all.png")
    , iconDatabase_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_tree_database.png")
    , iconFile_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_tree_file.png")
    , iconTxt_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_tree_txt.png")
{
}

QRect CommonTreeStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    QRect rect = QProxyStyle::subElementRect(element, option, widget);
    switch (element) {
    case QStyle::SE_ShapedFrameContents:
        rect.adjust(1, 1, -1, -1);
        break;
    default:
        break;
    }
    return rect;
}

void CommonTreeStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                  const QWidget *widget) const
{
    switch (element) {
    case QStyle::CE_ShapedFrame: {
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        const QTreeView *listView = qobject_cast<const QTreeView *>(option->styleObject);
        if (listView) {
            painter->save();

            QRect tmp = option->rect;

            painter->setPen(QPen(QColor(98, 162, 227), 2));
            painter->setBrush(QColor(98, 162, 227));
            painter->drawRect(widget->rect());

            tmp.adjust(2, 2, -2, -2);
            painter->setPen(QPen(QColor(219, 234, 255), 1));
            painter->setBrush(QColor(219, 234, 255));

            painter->drawRoundedRect(tmp, 10, 10);

            painter->restore();
        }
        return;
    }
    case QStyle::CE_ItemViewItem: {
        const QStyleOptionViewItem *vopt = qstyleoption_cast<const QStyleOptionViewItem *>(option);
        QStyleOptionViewItem tvopt(*vopt);
        tvopt.icon = iconDatabase_;
        if (vopt == nullptr) {
            return;
        }

        painter->save();
        painter->setClipRect(option->rect);
        QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, &tvopt, widget);

        QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, &tvopt, widget);
        proxy()->drawPrimitive(PE_PanelItemViewItem, option, painter, widget);

        QIcon::Mode mode = QIcon::Normal;
        if (vopt->state & QStyle::State_Selected)
            mode = QIcon::Selected;
        QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;

        QModelIndex indexTmp = vopt->index;
        const QAbstractItemModel *modelTmp = indexTmp.model();
        QMap mapTmp = modelTmp->itemData(indexTmp);
        auto it = mapTmp.find(1);
        if (it != mapTmp.end()) {
            QIcon iconTmp;
            switch (it.value().toInt()) {
            case 0: {
                iconTmp = iconAll_;
                break;
            }
            case 1: {
                iconTmp = iconDatabase_;
                break;
            }
            case 2: {
                iconTmp = iconFile_;
                break;
            }
            case 3: {
                iconTmp = iconTxt_;
                break;
            }
            default:
                break;
            }

            if (vopt->state & QStyle::State_Selected) {
                QPixmap pixmap = iconTmp.pixmap(iconTmp.actualSize(iconRect.size()));
                QImage image = pixmap.toImage();

                for (int y = 0; y < image.height(); y++) {
                    for (int x = 0; x < image.width(); x++) {
                        QRgb pixel = image.pixel(x, y);
                        int alpha = qAlpha(pixel);
                        if (alpha != 0) {
                            QRgb color = image.pixel(x, y);
                            image.setPixel(x, y, qRgb(255 - qRed(color), 255 - qGreen(color), 255 - qBlue(color)));
                        }
                    }
                }

                QIcon invertedIcon = QIcon(QPixmap::fromImage(image));

                invertedIcon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
            } else {
                iconTmp.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
            }
        }

        if (!vopt->text.isEmpty()) {
            QPalette::ColorGroup cg = vopt->state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
            if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
                cg = QPalette::Inactive;

            if (vopt->state & QStyle::State_Selected) {
                painter->setPen(vopt->palette.color(cg, QPalette::HighlightedText));
            } else {
                painter->setPen(vopt->palette.color(cg, QPalette::Text));
            }
            if (vopt->state & QStyle::State_Editing) {
                painter->setPen(vopt->palette.color(cg, QPalette::Text));
                painter->drawRect(textRect.adjusted(0, 0, -1, -1));
            } else {
                painter->drawText(textRect, Qt::AlignLeft, vopt->text);
            }
        }

        if (vopt->state & QStyle::State_HasFocus) {
            QStyleOptionFocusRect rcFocus;
            rcFocus.QStyleOption::operator=(*vopt);
            rcFocus.rect = proxy()->subElementRect(SE_ItemViewItemFocusRect, vopt, widget);
            rcFocus.state |= QStyle::State_KeyboardFocusChange;
            rcFocus.state |= QStyle::State_Item;
            QPalette::ColorGroup cg = (vopt->state & QStyle::State_Enabled) ? QPalette::Normal : QPalette::Disabled;
            rcFocus.backgroundColor = vopt->palette.color(
                cg, (vopt->state & QStyle::State_Selected) ? QPalette::Highlight : QPalette::Window);
            proxy()->drawPrimitive(QStyle::PE_FrameFocusRect, &rcFocus, painter, widget);
        }
        painter->restore();
        return;
    }
    default:
        break;
    }
    return QProxyStyle::drawControl(element, option, painter, widget);
}

void CommonTreeStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                    const QWidget *widget) const
{
    painter->setRenderHint(QPainter::Antialiasing);
    switch (element) {
    case QStyle::PE_IndicatorBranch: {
        painter->save();
        int nSizeDecoration = 9;
        int nMidH = option->rect.x() + option->rect.width() / 2;
        int nMidV = option->rect.y() + option->rect.height() / 2;
        int nBefH = nMidH;
        int nBefV = nMidV;
        int nAftH = nMidH;
        int nAftV = nMidV;

        QBrush brush(QColor(159, 174, 219), Qt::Dense4Pattern);
        if (option->state & State_Item) {
            if (option->direction == Qt::RightToLeft)
                painter->fillRect(option->rect.left(), nMidV, nBefH - option->rect.left(), 1, brush);
            else
                painter->fillRect(nAftH, nMidV, option->rect.right() - nAftH + 1, 1, brush);
        }
        if (option->state & State_Sibling)
            painter->fillRect(nMidH, nAftV, 1, option->rect.bottom() - nAftV + 1, brush);
        if (option->state & (State_Open | State_Children | State_Item | State_Sibling))
            painter->fillRect(nMidH, option->rect.y(), 1, nBefV - option->rect.y(), brush);
        if (option->state & State_Children) {
            int nDelta = nSizeDecoration / 2;
            nBefH -= nDelta;
            nBefV -= nDelta;

            QPen oldPen = painter->pen();
            if (option->state & State_Open) {
                painter->setPen(QColor(92, 108, 156));
                painter->setBrush(QColor(92, 108, 156));
            } else {
                painter->setPen(QColor(159, 174, 219));
                painter->setBrush(QColor(159, 174, 219));
            }

            QRect rectOut(nBefH, nBefV, nSizeDecoration - 1, nSizeDecoration - 1);
            rectOut.adjust(-1, -1, 1, 1);
            painter->drawRoundedRect(rectOut, 2, 2);

            oldPen.setColor(Qt::white);
            painter->setPen(oldPen);
            painter->drawLine(nBefH + 1, nBefV + 4, nBefH + 7, nBefV + 4);
            if (!(option->state & State_Open))
                painter->drawLine(nBefH + 4, nBefV + 1, nBefH + 4, nBefV + 7);

            painter->setPen(oldPen);
        }
        painter->restore();
        return;
    }
    case QStyle::PE_PanelItemViewItem: {
        if (const QStyleOptionViewItem *vopt = qstyleoption_cast<const QStyleOptionViewItem *>(option)) {
            QPalette::ColorGroup cg = (widget ? widget->isEnabled() : (vopt->state & QStyle::State_Enabled))
                                          ? QPalette::Normal
                                          : QPalette::Disabled;
            if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
                cg = QPalette::Inactive;

            if (vopt->showDecorationSelected && (vopt->state & QStyle::State_Selected)) {
                painter->fillRect(vopt->rect, vopt->palette.brush(cg, QPalette::Highlight));
            } else {
                if (vopt->backgroundBrush.style() != Qt::NoBrush) {
                    QPointF oldBO = painter->brushOrigin();
                    painter->setBrushOrigin(vopt->rect.topLeft());
                    painter->fillRect(vopt->rect, vopt->backgroundBrush);
                    painter->setBrushOrigin(oldBO);
                }

                if (vopt->state & QStyle::State_Selected) {
                    QRect textRect = subElementRect(QStyle::SE_ItemViewItemText, option, widget);

                    QBrush brush(QColor(48, 140, 198));
                    painter->fillRect(textRect, brush);
                } else if (vopt->state & QStyle::State_MouseOver) {
                    QRect textRect = subElementRect(QStyle::SE_ItemViewItemText, option, widget);

                    QBrush brush(QColor(193, 208, 255));
                    painter->fillRect(textRect, brush);
                }
            }
        }
        return;
    }
    default:
        break;
    }
    return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

//-ComboScrollbarStyle-------------------------------------------------------------------------
ComboScrollbarStyle::ComboScrollbarStyle(){};

QRect ComboScrollbarStyle::subControlRect(ComplexControl element, const QStyleOptionComplex *option,
                                          SubControl whichSubControl, const QWidget *widget) const
{
    return QProxyStyle::subControlRect(element, option, whichSubControl, widget);
};

void ComboScrollbarStyle::drawComplexControl(ComplexControl element, const QStyleOptionComplex *option,
                                             QPainter *painter, const QWidget *widget) const

{
    if (element == CC_ScrollBar && option) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QRect rectOption = option->rect;
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(219, 234, 255));

        painter->drawRect(rectOption);

        painter->setPen(QPen(QColor(98, 162, 227), 2));
        painter->drawLine(0, 0, 0, option->rect.height());

        QRect rcSlider = subControlRect(element, option, SC_ScrollBarSlider, widget);
        rcSlider.adjust(rcSlider.width() / 4, 0, -rcSlider.width() / 4, 0);
        if (option->state & State_MouseOver) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(122, 142, 207));
            painter->drawRoundedRect(rcSlider, rcSlider.width() / 2, rcSlider.width() / 2);

            QRect rcSubLine = subControlRect(element, option, SC_ScrollBarSubLine, widget);
            painter->save();
            painter->translate(rcSubLine.topLeft());
            QPainterPath path1;
            path1.moveTo(rcSubLine.width() / 2, rcSubLine.height() / 5);
            path1.lineTo(rcSubLine.width() / 5, rcSubLine.height() * 4 / 5);
            path1.lineTo(rcSubLine.width() * 4 / 5, rcSubLine.height() * 4 / 5);
            path1.closeSubpath();
            painter->drawPath(path1);
            painter->restore();

            QRect rcAddLine = subControlRect(element, option, SC_ScrollBarAddLine, widget);
            painter->save();
            painter->translate(rcAddLine.topLeft());
            QPainterPath path2;
            path2.moveTo(rcAddLine.width() / 2, rcAddLine.height() * 4 / 5);
            path2.lineTo(rcAddLine.width() / 5, rcAddLine.height() / 5);
            path2.lineTo(rcAddLine.width() * 4 / 5, rcAddLine.height() / 5);
            path2.closeSubpath();
            painter->drawPath(path2);
            painter->restore();
        } else {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(193, 208, 255));
            painter->drawRoundedRect(rcSlider, rcSlider.width() / 2, rcSlider.width() / 2);
        }

        painter->restore();
        return;
    } else {
        return QProxyStyle::drawComplexControl(element, option, painter, widget);
    }
}
} // namespace SmpTree
} // namespace Ui
