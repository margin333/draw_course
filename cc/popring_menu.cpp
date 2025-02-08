#include "popring_menu.h"
#include <QPainter>
#include <QPropertyAnimation>

#define ShowMenuDuration 100
#define ShowMenuStartValue 500
#define ShowMenuEndValue 1000

#define SlowlyFadeDuration 3000
#define SlowlyStartValue 1
#define SlowLyEndValue 0.3
PopRingItem::PopRingItem(QWidget *parent)
    : QPushButton(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setMouseTracking(true);
    //  setScaledContents(true);
    //  setScaledContents(true); // 让图标自动适应按钮大小
}

PopRingItem::~PopRingItem() { }
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingItem::SetRadius(int radius)
{
    m_iRadius = radius;

    int width = m_iRadius / qSqrt(2);
    resize(width, width);
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
int PopRingItem::GetRadius() const { return m_iRadius; }
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingItem::BindAction(QAction *action)
{
    if (nullptr == m_actAction) {
        m_actAction = new QAction(this);
    }
    //  setPixmap(QPixmap(":/MainMaskAlign/build/bin/image/bkg/status_on.png")
    //                .scaled(size()));
    m_actAction->setIcon(action->icon());
    // connect(m_actAction, &QAction::triggered, action, &QAction::trigger);
    connect(this, &PopRingItem::clicked, action, &QAction::trigger);
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingItem::enterEvent(QEvent *event)
{
    QPushButton::enterEvent(event);

    emit MouseEvent(true);
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingItem::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    emit MouseEvent(false);
}

// void PopRingItem::mouseMoveEvent(QMouseEvent *event) {
//   QPushButton::mouseMoveEvent(event);
// }
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::gray);

    int radius = m_iRadius / qSqrt(2) / 2;
    painter.drawRoundedRect(rect(), radius, radius);

    QPushButton::paintEvent(event);
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
PopRingMenu::PopRingMenu(QWidget *parent)
    : PopRingItem(parent)
{
    SetRadius(iMenuSize_);
    //  setPixmap(QPixmap(":/MainMaskAlign/build/bin/image/bkg/status_on.png"));
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
PopRingMenu::~PopRingMenu() { }
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingMenu::SetActions(const QVector<QAction *> &acts)
{
    std::for_each(acts.begin(), acts.end(), [this](QAction *act) {
        PopRingItem *item = new PopRingItem(this);
        item->SetRadius(iItemSize_);
        item->BindAction(act);
        connect(item, &PopRingItem::MouseEvent, this, &PopRingMenu::OnMouseEvent);
        items_.push_back(item);
    });
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingMenu::SetIcons(const QVector<QString> &icons)
{
    for (int i = 0; i < items_.size() && i < icons.size(); ++i) {
        PopRingItem *item = items_.at(i);
        const QString &icon = icons.at(i);
        item->setIcon(QPixmap(icon));
    };
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingMenu::SetAnimationEnabled(bool enabled)
{
    if (enabled) {
        if (nullptr == pItemAnimation_) {
            pItemAnimation_ = new QVariantAnimation(this);

            pItemAnimation_->setEasingCurve(QEasingCurve::InCubic);
            pItemAnimation_->setStartValue(ShowMenuStartValue);
            pItemAnimation_->setEndValue(ShowMenuEndValue);
            pItemAnimation_->setDuration(ShowMenuDuration);

            connect(pItemAnimation_, &QVariantAnimation::valueChanged, this,
                    [this](const QVariant &v) { UpdateActions(v.toInt()); });
        }
    } else {

        if (pItemAnimation_) {
            delete pItemAnimation_;
            pItemAnimation_ = nullptr;
        }
    }
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
bool PopRingMenu::IsAnimationEnabled() const { return pItemAnimation_ != nullptr; }
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingMenu::SetSlowlyFade(bool enabled)
{
    if (enabled) {
        if (nullptr == pOpacityAnimation_) {
            pOpacityAnimation_ = new QPropertyAnimation(this, "opacity");
            pOpacityAnimation_->setEasingCurve(QEasingCurve::OutCubic);
            pOpacityAnimation_->setStartValue(SlowlyStartValue);
            pOpacityAnimation_->setEndValue(SlowLyEndValue);
            pOpacityAnimation_->setDuration(SlowlyFadeDuration);
        }
    } else {

        if (pOpacityAnimation_) {
            delete pOpacityAnimation_;
            pOpacityAnimation_ = nullptr;
        }
    }
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
bool PopRingMenu::IsSlowlyFade() const { return pOpacityAnimation_ != nullptr; }
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingMenu::SetDistanced(int distance) { iDistance_ = distance; }
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
int PopRingMenu::GetDistanced() const { return iDistance_; }
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingMenu::SetStartAngle(int angle) { iStartAngle_ = angle; }
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
int PopRingMenu::GetStartAngle() const { return iStartAngle_; }

void PopRingMenu::SetStepAngle(int angle) { iStepAngle_ = angle; }

int PopRingMenu::GetStepAngle() const { return iStepAngle_; }

void PopRingMenu::SetNormalMenuSize(int size)
{
    if (iMenuSize_ == size) {
        return;
    }

    iMenuSize_ = size;
    SetRadius(size);
}

int PopRingMenu::GetNormalMenuSize() const { return iMenuSize_; }

void PopRingMenu::SetNormalItemSize(int size) { iItemSize_ = size; }

int PopRingMenu::GetNormalItemSize() const { return iItemSize_; }

void PopRingMenu::enterEvent(QEvent *event)
{
    PopRingItem::enterEvent(event);
    ExpandMenu();
}

void PopRingMenu::leaveEvent(QEvent *event)
{

    //  bool under = false;
    //  QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    //  QPoint mousePos = mouseEvent->pos();
    //  qDebug() << "----P:" << mousePos.x() << "/" << mousePos.y();

    //  if (event->type() == QEvent::MouseMove) {
    //    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    //    foreach (auto item, items_) {
    //      QRect r = item->rect();
    //      QPoint p = mouseEvent->pos();
    //      qDebug() << "----P:" << p.x() << "/" << p.y();
    //      if (r.contains(p)) {
    //        under = true;
    //      }
    //    }
    //  }

    //  // 如果鼠标在items_上，不收起菜单，否则收起菜单

    //  if (!under) {
    //    TryCollapseMenu();
    //  }

    TryCollapseMenu();
    PopRingItem::leaveEvent(event);
}

void PopRingMenu::mouseMoveEvent(QMouseEvent *event)
{

    if (mousePress_) {
        move(event->globalPos() - mousePoint_);
        if (closeMenu_) {
            if (pItemAnimation_) {
                pItemAnimation_->setDirection(QVariantAnimation::Backward);
                pItemAnimation_->start();
            } else {
                // UpdateActions(ShowMenuStartValue);
            }

            KillHideTimer();
            closeMenu_ = false;
            // SlowlyFade();
        }
    }

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    QPoint mousePos = mouseEvent->pos();
    qDebug() << "----P:" << mousePos.x() << "/" << mousePos.y();

    //  if (event->type() == QEvent::MouseMove) {
    //    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    //    foreach (auto item, items_) {
    //      QRect r = item->rect();
    //      QPoint p = mouseEvent->pos();
    //      qDebug() << "----P:" << p.x() << "/" << p.y();
    //      if (r.contains(p)) {
    //        under = true;
    //      }
    //    }
    //  }

    // 如果鼠标在items_上，不收起菜单，否则收起菜单

    TryCollapseMenu();
    PopRingItem::mouseMoveEvent(event);
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
// void PopRingMenu::mousePressEvent(QMouseEvent *event) {

//  // 如果是鼠标左键按下
//  if (event->button() == Qt::LeftButton) {
//    // ···
//  }
//  // 如果是鼠标右键按下
//  else if (event->button() == Qt::RightButton) {
//    // ···
//  }
//}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingMenu::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPushButton::mouseDoubleClickEvent(event);

    emit DoubleClicked();
}
/**************************************************************************
 * 名称:
 * 功能:
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   zhouweize
 * 创建时间:
 ***************************************************************************/
void PopRingMenu::timerEvent(QTimerEvent *event)
{
    if (m_iTimerID == event->timerId()) {
        CollapseMenu();
        KillHideTimer();
    }
    PopRingItem::timerEvent(event);
}

bool PopRingMenu::event(QEvent *event) { return PopRingItem::event(event); }

void PopRingMenu::OnMouseEvent(bool enter)
{
    // 正在收起时 不支持通过展开的菜单项 进行再次展开或者收起
    //  if (pItemAnimation_ &&
    //      pItemAnimation_->direction() == QVariantAnimation::Backward &&
    //      pItemAnimation_->state() == QVariantAnimation::Running) {
    //    return;
    //  }

    if (enter) {
        ExpandMenu();
    } else {
        TryCollapseMenu();
    }
}

void PopRingMenu::UpdateActions(int msecond)
{
    int curDistance = msecond * iDistance_ / ShowMenuEndValue;
    for (int i = 0; i < items_.size(); ++i) {
        PopRingItem *item = items_.at(i);

        double radians = qDegreesToRadians(iStepAngle_ * i * 1.0 + iStartAngle_);
        int offx = curDistance * qCos(radians);
        int offy = curDistance * qSin(radians);
        item->move(pos() + QPoint(offx, offy));

        int curSize = msecond * iItemSize_ / ShowMenuEndValue;
        item->SetRadius(curSize);

        item->setVisible(ShowMenuStartValue != msecond);
    };

    // 将窗口置于最前面
    //  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void PopRingMenu::ExpandMenu()
{
    if (pItemAnimation_) {
        if (pItemAnimation_->state() != QAbstractAnimation::Running &&
            pItemAnimation_->currentValue().toInt() != ShowMenuEndValue) {
            pItemAnimation_->setDirection(QVariantAnimation::Forward);
            pItemAnimation_->start();
        }
    } else {
        UpdateActions(ShowMenuEndValue);
    }

    KillHideTimer();
    QuicklyLighter();
}

void PopRingMenu::CollapseMenu()
{
    if (false == IsUnderMouse()) {
        if (pItemAnimation_) {
            pItemAnimation_->setDirection(QVariantAnimation::Backward);
            pItemAnimation_->start();
        } else {
            UpdateActions(ShowMenuStartValue);
        }

        KillHideTimer();
        SlowlyFade();
    }
}

void PopRingMenu::SlowlyFade()
{
    if (pOpacityAnimation_) {
        pOpacityAnimation_->setDirection(QVariantAnimation::Forward);
        pOpacityAnimation_->start();
    }
}

void PopRingMenu::QuicklyLighter()
{
    if (pOpacityAnimation_) {
        if (pItemAnimation_->state() == QAbstractAnimation::Running) {
            pOpacityAnimation_->stop();
        }

        pItemAnimation_->setCurrentTime(0);
    }
}

bool PopRingMenu::IsUnderMouse() const
{
    bool under = underMouse();
    //  if (false == under) {
    //    std::find_if(items_.begin(), items_.end(), [&under](PopRingItem *item)
    //    {
    //      if (item->underMouse()) {
    //        under = true;
    //        qDebug() << "==================" << endl;
    //        return true;
    //      }
    //      qDebug() << "++++++++++++++++++++" << endl;
    //      return false;
    //    });
    //  }

    return under;
}

void PopRingMenu::TryCollapseMenu()
{
    KillHideTimer(); // 杀掉上一个定时器 重新开始计时
    m_iTimerID = startTimer(2000);
}

void PopRingMenu::KillHideTimer()
{
    if (-1 != m_iTimerID) {
        killTimer(m_iTimerID);
        m_iTimerID = -1;
    }
}
