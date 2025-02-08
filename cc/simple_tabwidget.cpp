#include "simple_tabwidget.h"
#include "include/bne_app_enum.h"
#include <QApplication>

namespace Ui {
namespace SmplTabWdt {

SimpleTabWidget::SimpleTabWidget(QWidget *parent)
    : QTabWidget{parent}
    , bChange_(true)
    , shadowEffect_(nullptr)
{
    this->setTabBar(&tabBar_);
    this->setTabPosition(QTabWidget::North);
    // 1.
    TabBarStyle *pStyleBar = &StyleInstance<TabBarStyle>::GetIntance();
    if (this->tabBar()) {
        this->tabBar()->setStyle(pStyleBar);
    }

    QFont defaultFont = QApplication::font();
    defaultFont.setPixelSize(AppEnum::BNE_APP_FONTSIZE_BIG);
    defaultFont.setBold(true);
    this->tabBar()->setFont(defaultFont);

    // 2.
    TabProxy *pStyleTab = &StyleInstance<TabProxy>::GetIntance();
    this->setStyle(pStyleTab);

    if (!shadowEffect_) {
        shadowEffect_ = new QGraphicsDropShadowEffect;
        shadowEffect_->setBlurRadius(10);
        shadowEffect_->setColor(QColor(166, 171, 180));
        shadowEffect_->setOffset(-2, 2);

        this->tabBar()->setGraphicsEffect(shadowEffect_);
    }
}

bool SimpleTabWidget::GetChangeFlag() { return bChange_; }

void SimpleTabWidget::SetChangeFlag(bool b) { bChange_ = b; }

void SimpleTabWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath path;
    QRect rct = this->rect();
    rct.adjust(1, 1, -1, -1);
    path.addRoundedRect(rct, this->width() / 40, this->width() / 40);

    painter.setPen(QPen(QColor(178, 205, 243), 2));
    painter.setBrush(Qt::white);
    painter.drawPath(path);
    return QTabWidget::paintEvent(event);
};

//-------------------------------------------------------------------------------------------------------
TabBarStyle::TabBarStyle()
    : imgBarSelected_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_tabbar_over.png")
    , imgBarEnable_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_tabbar_no.png"){

      };

void TabBarStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                              const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (element == CE_TabBarTabLabel) {
        if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
            //-----------------------------------------------------------------------
            QRect tabRect = tab->rect;
            tabRect.adjust(0, 0, -3, 0);
            QPainterPath path;
            path.addRect(tabRect);

            QLinearGradient gradient(tabRect.x(), tabRect.y(), tabRect.x(), tabRect.height());
            gradient.setColorAt(0, QColor(106, 157, 230));
            gradient.setColorAt(0.515, QColor(6, 57, 115));
            gradient.setColorAt(1, QColor(106, 157, 230));

            QPen penGradient(gradient, 1);

            painter->setClipPath(path);

            if ((tab->state & QStyle::State_Selected) || (tab->state & QStyle::State_MouseOver)) {
                painter->save();

                if (!imgBarSelected_.isNull()) {

                    QImage scaledImage = imgBarSelected_.scaled(tabRect.width(), tabRect.height(), Qt::KeepAspectRatio,
                                                                Qt::SmoothTransformation);

                    painter->drawImage(tabRect, scaledImage);
                }

                painter->restore();
                painter->setPen(0xffffff);
            } else {

                painter->save();

                if (!imgBarEnable_.isNull()) {

                    QImage scaledImage = imgBarEnable_.scaled(tabRect.width(), tabRect.height(), Qt::KeepAspectRatio,
                                                              Qt::SmoothTransformation);

                    painter->drawImage(tabRect, scaledImage);
                }

                painter->restore();
                painter->setPen(penGradient);
            }
            painter->drawText(tabRect, Qt::AlignCenter, tab->text);
            return;
        }
    }
    if (element == CE_TabBarTab) {
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}

//---------------------------------------------------------------------------------------------------------
SimpleTabBar::~SimpleTabBar() { }

SimpleTabBar::SimpleTabBar(QWidget *parent)
    : QTabBar(parent)
{
    vecFunc_.clear();
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    this->setBackgroundRole(QPalette::Base);
    this->setPalette(palette);
    this->setDrawBase(false);
}

void SimpleTabBar::AddClickedCheck(const std::function<bool()> &func) { vecFunc_.push_back(func); };

void SimpleTabBar::mousePressEvent(QMouseEvent *e)
{
    if (vecFunc_.size() != 0) {
        for (std::vector<std::function<bool()>>::iterator it = vecFunc_.begin(); it != vecFunc_.end(); ++it) {
            if (!(*it)()) {
                return;
            }
        }
    }

    return QTabBar::mousePressEvent(e);
}

//---------------------------------------------------------------------------------------------------------
TabProxy ::TabProxy(QStyle *style)
    : QProxyStyle(style)
    , colorPane_(QColor(235, 243, 255)){

      };

void TabProxy ::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                              const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (element == PE_FrameTabWidget) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(widget->rect());

        QStyleOptionTabWidgetFrame frameOpt;
        frameOpt.initFrom(widget);
        QRect rctPane = subElementRect(SE_TabWidgetTabPane, &frameOpt, widget);
        QRect rctBar = subElementRect(SE_TabWidgetTabBar, option, widget);
        QRect rctContents = subElementRect(SE_TabWidgetTabContents, option, widget);

        painter->setPen(Qt::NoPen);
        painter->setBrush(option->palette.color(QPalette::Background));

        painter->drawRoundedRect(rctPane, 10, 10);

        painter->drawRoundedRect(rctContents, 10, 10);

        painter->drawRoundedRect(rctBar, 10, 10);
        return;
    } else {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

QRect TabProxy ::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    int nTypePosition = -1;
    const QTabWidget *tabWidget = qobject_cast<const QTabWidget *>(widget);

    QStyleOptionTabWidgetFrame frameOpt;
    frameOpt.initFrom(widget);

    switch (element) {
    case SE_TabWidgetTabBar: {
        QRect rectBar = QProxyStyle::subElementRect(element, option, widget);
        int nPadding = rectBar.height() / 2;
        if (tabWidget->tabPosition() == QTabWidget::North) {

            rectBar.adjust(3, 3, 3, 3);
        } else if (tabWidget->tabPosition() == QTabWidget::South) {
            rectBar.adjust(nPadding, -nPadding, nPadding, -nPadding);
        }

        return rectBar;
    }
    case SE_TabWidgetTabContents: {
        QRect contentsRect = QProxyStyle::subElementRect(element, &frameOpt, widget);
        QRect rectBar = subElementRect(SE_TabWidgetTabBar, option, widget);
        int nPadding = rectBar.height() / 2;
        if (tabWidget->tabPosition() == QTabWidget::North) {

            contentsRect.adjust(3, 3 + rectBar.height() * 1.3, -2, -2);
        } else if (tabWidget->tabPosition() == QTabWidget::South) {
            contentsRect.adjust(nPadding, nPadding, -(nPadding - 1), -(nPadding - 1) - rectBar.height() * 1.5);
        }

        return contentsRect;
    }
    default:
        return QProxyStyle::subElementRect(element, option, widget);
    }
};

SimpleTabWidgetSidebr::SimpleTabWidgetSidebr(QWidget *parent):Ui::SmplTabWdt::SimpleTabWidget(parent)
{

}

} // namespace SmplTabWdt
} // namespace Ui
