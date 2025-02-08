#include "center_tabwidget.h"
#include <QApplication>

namespace Ui {
namespace TabBar {
CenterTabBar::CenterTabBar(QWidget *parent)
    : Ui::SmplTabWdt::SimpleTabBar(parent)
    , nWidth_(0)
    , nHeight_(0)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::transparent);
    this->setBackgroundRole(QPalette::Background);
    this->setPalette(palette);
    this->setDrawBase(false);
}

CenterTabBar::~CenterTabBar() { }

void CenterTabBar::SetTabHeight(int nHeight) { nHeight_ = nHeight; }

void CenterTabBar::SetTabWidth(int nWidth) { nWidth_ = nWidth; }

QSize CenterTabBar::tabSizeHint(int index) const
{
    QSize size = QTabBar::tabSizeHint(index);
    if (nHeight_ > 0) {
        size.setHeight(nHeight_);
    }

    if (nWidth_ > 0) {
        size.setWidth(nWidth_);
    }

    return size;
}

//------------------------------------------------------------------
CenterTabWidget::CenterTabWidget(QWidget *parent)
    : QTabWidget{parent}
    , shadowEffect_(nullptr)
{
    this->setTabBar(&tabBar_);
    this->setTabPosition(QTabWidget::South);
    // 1.
    if (this->tabBar()) {
        CenterTabBarStyle *pStyleBar = &StyleInstance<CenterTabBarStyle>::GetIntance();
        this->tabBar()->setStyle(pStyleBar);
    }

    this->tabBar()->setDocumentMode(true);
    this->tabBar()->setExpanding(true);

    QFont defaultFont = QApplication::font();
    defaultFont.setPixelSize(18);
    defaultFont.setBold(true);
    this->tabBar()->setFont(defaultFont);

    // 2.
    CenterTabProxy *pStyleTab = &StyleInstance<CenterTabProxy>::GetIntance();
    this->setStyle(pStyleTab);

    if (!shadowEffect_) {
        shadowEffect_ = new QGraphicsDropShadowEffect;
        shadowEffect_->setBlurRadius(10);

        shadowEffect_->setColor(QColor(0, 0, 0, 0.4 * 255));
        shadowEffect_->setOffset(-2, 2);

        this->tabBar()->setGraphicsEffect(shadowEffect_);
    }

    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::transparent);
    this->setBackgroundRole(QPalette::Background);
    this->setPalette(palette);
    tabBar_.SetTabHeight(50);
}
//-------------------------------------------------------------------------------------------------------
CenterTabBarStyle::CenterTabBarStyle()
    : imgBarBeginPress_(":/MainMaskAlign/build/bin/image/bkg/tab_Begin_btn_press.png")
    , imgBarBeginEnable_(":/MainMaskAlign/build/bin/image/bkg/tab_Begin_btn.png")
    , imgBarMidPress_(":/MainMaskAlign/build/bin/image/bkg/tab_Mid_btn_press.png")
    , imgBarMidEnable_(":/MainMaskAlign/build/bin/image/bkg/tab_Mid_btn.png")
    , imgBarEndPress_(":/MainMaskAlign/build/bin/image/bkg/tab_End_btn_press.png")
    , imgBarEndEnable_(":/MainMaskAlign/build/bin/image/bkg/tab_End_btn.png"){

      };

void CenterTabBarStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                    const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (element == CE_TabBarTabLabel) {
        if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
            QStyleOptionTab tabTmp(*tab);
            //-----------------------------------------------------------------------
            QRect tabRect = tab->rect;
            tabRect.adjust(0, 0, 0, 0);

            QPixmap pixIcon = tab->icon.pixmap(tab->rect.size());

            QImage imgIcon = pixIcon.toImage();
            imgIcon = imgIcon.scaled(tabTmp.iconSize, Qt::IgnoreAspectRatio);

            bool bMouseOver = false;
            if ((tab->state & QStyle::State_Selected) || (tab->state & QStyle::State_MouseOver)) {
                bMouseOver = true;
                for (int y = 0; y < imgIcon.height(); y++) {
                    for (int x = 0; x < imgIcon.width(); x++) {
                        QRgb pixel = imgIcon.pixel(x, y);
                        int alpha = qAlpha(pixel);
                        if (alpha != 0) {
                            int red = 255;
                            int green = 255;
                            int blue = 255;
                            QRgb invertedPixel = qRgba(red, green, blue, 255);
                            imgIcon.setPixel(x, y, invertedPixel);
                        }
                    }
                }
                painter->setPen(0xffffff);
            }

            //------------------------------
            QPainterPath path;
            path.addRect(tabRect);

            QLinearGradient gradient(tabRect.x(), tabRect.y(), tabRect.x(), tabRect.height());
            gradient.setColorAt(0, QColor(106, 157, 230));
            gradient.setColorAt(0.515, QColor(6, 57, 115));
            gradient.setColorAt(1, QColor(106, 157, 230));

            QPen penGradient(gradient, 1);
            painter->setPen(penGradient);

            painter->setClipPath(path);

            switch (tabTmp.position) {
            case QStyleOptionTab::Beginning:
                if (bMouseOver) {
                    painter->save();
                    if (!imgBarBeginPress_.isNull()) {
                        painter->drawImage(tabRect, imgBarBeginPress_);
                    }
                    painter->restore();
                } else {
                    painter->save();
                    if (!imgBarBeginEnable_.isNull()) {
                        painter->drawImage(tabRect, imgBarBeginEnable_);
                    }
                    painter->restore();
                }

                break;
            case QStyleOptionTab::End:
                if (bMouseOver) {
                    painter->save();
                    if (!imgBarEndPress_.isNull()) {
                        painter->drawImage(tabRect, imgBarEndPress_);
                    }
                    painter->restore();
                } else {
                    painter->save();
                    if (!imgBarEndEnable_.isNull()) {
                        painter->drawImage(tabRect, imgBarEndEnable_);
                    }
                    painter->restore();
                }
                break;
            default:
                if (bMouseOver) {
                    painter->save();
                    if (!imgBarMidPress_.isNull()) {
                        painter->drawImage(tabRect, imgBarMidPress_);
                    }
                    painter->restore();

                } else {
                    painter->save();
                    if (!imgBarMidEnable_.isNull()) {
                        painter->drawImage(tabRect, imgBarMidEnable_);
                    }
                    painter->restore();
                }
                break;
            }

            if (bMouseOver) {
                painter->setPen(0xffffff);
            } else {
                painter->setPen(penGradient);
            }

            QRect rectTmp(option->rect);
            rectTmp.setSize(imgIcon.size());
            rectTmp.setX(option->rect.center().x() - imgIcon.size().width() / 2);
            rectTmp.setY(option->rect.height() / 2 - imgIcon.size().height() / 2);

            QRect recttext = tabTmp.fontMetrics.boundingRect(tabTmp.text);

            int nLeft = option->rect.center().x() - (imgIcon.size().width() + recttext.width() + 5) / 2;
            QRect rctImg(nLeft, option->rect.height() / 2 - imgIcon.size().height() / 2, imgIcon.size().width(),
                         imgIcon.size().height());

            painter->drawImage(rctImg, imgIcon);

            QRect rctTxt(nLeft + imgIcon.size().width() + 5, option->rect.height() / 2 - recttext.size().height() / 2,
                         recttext.size().width(), recttext.size().height());
            painter->drawText(rctTxt, Qt::AlignCenter, tab->text);

            return;
        }
    } else if (element == CE_TabBarTab) {
        drawControl(CE_TabBarTabLabel, option, painter, widget);
        return;
    } else if (element == CE_TabBarTabShape) {
        return;
    } else {
        return QProxyStyle::drawControl(element, option, painter, widget);
    }
}

//---------------------------------------------------------------------------------------------------------
CenterTabProxy ::CenterTabProxy(){};

QRect CenterTabProxy ::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    int nTypePosition = -1;
    const QTabWidget *tabWidget = qobject_cast<const QTabWidget *>(widget);

    QStyleOptionTabWidgetFrame frameOpt;
    frameOpt.initFrom(widget);

    switch (element) {
    case SE_TabWidgetTabBar: {
        QRect rectBar = QProxyStyle::subElementRect(element, option, widget);

        if (tabWidget->tabPosition() == QTabWidget::North) {

            rectBar.adjust(3, 3, 3, 3);
        } else if (tabWidget->tabPosition() == QTabWidget::South) {
            rectBar.adjust(0.2 * widget->width(), -10, -0.2 * widget->width(), -10);
        }

        return rectBar;
    }
    case SE_TabWidgetTabPane: {
        QRect rectPane = QProxyStyle::subElementRect(element, &frameOpt, widget);
        return rectPane;
    }
    case SE_TabWidgetTabContents: {

        QRect contentsRect = QProxyStyle::subElementRect(element, &frameOpt, widget);
        QRect rectBar = subElementRect(SE_TabWidgetTabBar, option, widget);

        if (tabWidget->tabPosition() == QTabWidget::North) {

            contentsRect.adjust(3, 3 + rectBar.height() * 1.5, -2, -2);
        } else if (tabWidget->tabPosition() == QTabWidget::South) {
            contentsRect.adjust(10, 5, -10, -widget->height() * 0.1);
        }

        return contentsRect;
    }
    default:
        return QProxyStyle::subElementRect(element, option, widget);
    }
};

void CenterTabProxy ::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                    const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (element == PE_FrameTabWidget) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawRect(widget->rect());
        return;
    } else {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};
} // namespace TabBar
} // namespace Ui
