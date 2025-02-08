#include "bnepropertygrid.h"

void PropertyGridStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                    const QWidget *widget) const
{
    if (QStyle::CE_ShapedFrame == element) {
        return;
    } else {
        return QProxyStyle::drawControl(element, option, painter, widget);
    }
};
void PropertyGridStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                      const QWidget *widget) const
{
    if (QStyle::PE_Frame == element) {
        return;
    } else {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

QRect PropertyGridStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
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
//--------------------------------------------------------------------
BNEPropertyGrid::BNEPropertyGrid(QWidget *parent)
    : QScrollArea(parent)
    , pBrowser_(nullptr)
{
    this->setStyle(&style_);
    pBrowser_ = new QtGroupBoxPropertyBrowser();

    QPalette pal0 = this->palette();
    pal0.setColor(QPalette::Background, Qt::transparent);
    this->setPalette(pal0);

    QPalette pal = pBrowser_->palette();
    pal.setColor(QPalette::Background, Qt::transparent);
    pBrowser_->setPalette(pal);
    this->setWidget(pBrowser_);

    if (verticalScrollBar()) {
        // 3.
        Ui::BNECombo::ComboScrollbarStyle *pStyleScroll = new Ui::BNECombo::ComboScrollbarStyle();
        verticalScrollBar()->setStyle(pStyleScroll);
        verticalScrollBar()->setAttribute(Qt::WA_OpaquePaintEvent, false);
        connect(verticalScrollBar(), &QScrollBar::valueChanged, this, [&]([[maybe_unused]] int value) {
            this->update();
            this->viewport()->update();
            pBrowser_->update();
            this->verticalScrollBar()->update();
        });
    }
};

BNEPropertyGrid::~BNEPropertyGrid() { }

void BNEPropertyGrid::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setBrush(Qt::red);
    painter.drawRect(rect());
    QScrollArea::paintEvent(e);

    return;
};

void BNEPropertyGrid::resizeEvent(QResizeEvent *e)
{
    QScrollArea::resizeEvent(e);
    AutoSetMask();
    return;
};

void BNEPropertyGrid::AutoSetMask()
{
    this->viewport()->clearMask();

    QRect rect1(viewport()->rect());
    QRect rect2(verticalScrollBar()->rect());
    int nRadius = 15;
    bool bVisible = this->verticalScrollBar()->isVisible();

    QPainterPath roundedRectPath;
    if (!bVisible) {

        roundedRectPath.addRoundedRect(0, 0, viewport()->width(), viewport()->height(), nRadius, nRadius);

    } else {
        roundedRectPath.moveTo(rect1.topRight());
        roundedRectPath.lineTo(rect1.topLeft().x() + nRadius, rect1.topLeft().y());
        roundedRectPath.arcTo(QRectF(QPoint(rect1.topLeft().x(), rect1.topLeft().y()), QSize(nRadius * 2, nRadius * 2)),
                              90, 90);
        roundedRectPath.lineTo(rect1.bottomLeft().x(), rect1.bottomLeft().y() - nRadius);
        roundedRectPath.arcTo(QRectF(QPoint(rect1.bottomLeft().x(), rect1.bottomLeft().y() - nRadius * 2),
                                     QSize(nRadius * 2, nRadius * 2)),
                              180, 90);
        roundedRectPath.lineTo(rect1.bottomRight());
        roundedRectPath.closeSubpath();

        QPainterPath pathRoundBar;
        pathRoundBar.moveTo(rect2.topLeft());
        pathRoundBar.lineTo(rect2.bottomLeft());
        pathRoundBar.lineTo(rect2.bottomRight().x() - nRadius, rect2.bottomRight().y());
        pathRoundBar.arcTo(QRectF(QPoint(rect2.bottomRight().x() - nRadius * 2, rect2.bottomRight().y() - nRadius * 2),
                                  QSize(nRadius * 2, nRadius * 2)),
                           270, 90);
        pathRoundBar.lineTo(rect2.topRight().x(), rect2.topRight().y() + nRadius);
        pathRoundBar.arcTo(
            QRectF(QPoint(rect2.topRight().x() - nRadius * 2, rect2.topRight().y()), QSize(nRadius * 2, nRadius * 2)),
            0, 90);

        pathRoundBar.closeSubpath();
        this->verticalScrollBar()->setMask(pathRoundBar.toFillPolygon().toPolygon());
    }
    this->viewport()->setMask(roundedRectPath.toFillPolygon().toPolygon());

    return;
}

bool BNEPropertyGrid::viewportEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Resize: {
        QScrollArea::viewportEvent(event);
        AutoSetMask();
        return false;
    }
    case QEvent::Scroll:
        this->viewport()->update();
        return true;

    default:
        return QScrollArea::viewportEvent(event);
    }
};

void BNEPropertyGrid::wheelEvent(QWheelEvent *event)
{
    QScrollArea::wheelEvent(event);
    this->update();
    this->viewport()->update();
    pBrowser_->update();
    this->verticalScrollBar()->update();
    return;
}
