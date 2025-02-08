#include "waitprogresswidget.h"

#include <QPainter>
#include <QStackedWidget>
#include <QLabel>
namespace Ui {
namespace Custom {
/**************************************************************************
 * 名称: CircularProgress
 * 功能: CircularProgress类是一个自定义的圆形进度条，继承自QWidget。
 * 输入参数: QWidget *parent, 父部件
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-09
 ***************************************************************************/
WaitProgressWidget::WaitProgressWidget(QWidget *parent)
    : QWidget(parent)
    , parentWidget_(parent)
{
    this->setAutoFillBackground(true); // 设置自动填充背景
    QColor color(165, 165, 165);       // 设置颜色
    color.setAlphaF(0.6);              // 设置透明度
    this->setBgColor(color);           // 设置背景颜色
    parent->installEventFilter(this);  // 安装事件过滤器
}

WaitProgressWidget::~WaitProgressWidget() { }
/**************************************************************************
 * 名称: setBgColor
 * 功能: 设置WaitProgressWidget的背景颜色。
 * 输入参数: QColor color, 颜色
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-09
 ***************************************************************************/
void WaitProgressWidget::setBgColor(QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    this->setPalette(palette);
}
/**************************************************************************
 * 名称: Init
 * 功能: 初始化函数
 * 输入参数: NULL
 * 输出参数: NULL
 * 返回值:   BNEStatus
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-15
 ***************************************************************************/
BNEStatus WaitProgressWidget::Init()
{
    warningWidget_ = new WarningWidget(this);
    // warningWidget_->setStyleSheet("background-color:red;");
    //  点击警告窗口上的按钮退出当前页面
    connect(warningWidget_, &WarningWidget::sig_quit, this, [this]() { stop(false); });
    this->setFixedSize(parentWidget_->size());
    // 将警告窗口设置在父窗口的中间
    warningWidget_->SetWindowWH(400, 400);
    warningWidget_->Init();
    // 让warningWidget_显示在屏幕中间
    warningWidget_->move((this->width() - warningWidget_->width()) / 2,
                         (this->height() - warningWidget_->height()) / 2);
    warningWidget_->setWindowFlags(Qt::FramelessWindowHint);
    return BNE_APP_SUCCESS;
}
/**************************************************************************
 * 名称: eventFilter
 * 功能:在父部件调整大小时，自动调整并显示 WaitProgressWidget，使其与父部件的大小保持一致，并保持在最前面。
 * 输入参数:
 * 输出参数: NULL
 * 返回值:
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-09
 ***************************************************************************/
bool WaitProgressWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == parentWidget_ && ev->type() == QEvent::Resize) {
        if (!this->isHidden()) {
            this->setFixedSize(parentWidget_->size());
            this->raise();
        }
    }
    return QWidget::eventFilter(obj, ev);
}
/**************************************************************************
 * 名称: showEvent
 * 功能: WaitProgressWidget将显示在其同级窗口部件的顶部。
 * 输入参数: QShowEvent *ev, 事件
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-09
 ***************************************************************************/
void WaitProgressWidget::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev);
    Q_UNUSED(ev);

    warningWidget_->hide();
    this->setFixedSize(parentWidget_->size());
    this->raise();
    warningWidget_->show();
}
/**************************************************************************
 * 名称: start
 * 功能: 开始显示WaitProgressWidget，设置定时器，定时器超时后隐藏WaitProgressWidget。
 * 输入参数: int msecs, 定时器超时时间
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-09
 ***************************************************************************/
void WaitProgressWidget::start() { this->show(); }
/**************************************************************************
 * 名称: stop
 * 功能: 停止显示WaitProgressWidget，隐藏WaitProgressWidget。
 * 输入参数: bool isOk, 是否正常结束
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-09
 ***************************************************************************/
void WaitProgressWidget::stop(bool isOk)
{
    this->hide();
    emit sig_finished(isOk);
}
/**************************************************************************
 * 名称: show
 * 功能: 显示WaitProgressWidget。
 * 输入参数: NULL
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-09
 ***************************************************************************/
void WaitProgressWidget::show() { QWidget::show(); }
/**************************************************************************
 * 名称: hide
 * 功能: 隐藏WaitProgressWidget。
 * 输入参数: NULL
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-09
 ***************************************************************************/
void WaitProgressWidget::hide() { QWidget::hide(); }

} // namespace Custom
} // namespace Ui
