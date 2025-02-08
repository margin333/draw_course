#include "warningwidget.h"
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QtCore/qmath.h>
/**************************************************************************
 * 名称: WarningWidget
 * 功能: 构造函数
 * 输入参数: QWidget *parent, 父部件
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-15
 ***************************************************************************/
WarningWidget::WarningWidget(QWidget *parent)
    : QWidget(parent),
      text_("Moving in progress, please wait for the movement to complete...") {
  this->setWindowTitle("Warning");
  this->setAutoFillBackground(true); // 设置自动填充背景
  // 启动定时器，每隔一定时间刷新窗口
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(50); // 每50毫秒更新一次
  Init();
}
/**************************************************************************
 * 名称: SetWindowWH
 * 功能: 设置宽高
 * 输入参数: h,高；w,宽
 * 输出参数: NULL
 * 返回值:   void
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-15
 ***************************************************************************/
void WarningWidget::SetWindowWH(const int h, const int w) {
  height_ = h;
  width_ = w;
}
/**************************************************************************
 * 名称: Init
 * 功能:  初始化
 * 输入参数: null
 * 输出参数: NULL
 * 返回值:   void
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-15
 ***************************************************************************/
void WarningWidget::Init() {
  // QLabel *label = new QLabel(this);
  progress_ = new CircularProgress(this);
  progress_->blockSignals(true);
  progress_->setGeometry(width_ / 4, height_ / 21, height_ / 2, height_ / 2);
  setFixedSize(width_, height_);

  // 创建按钮并设置在窗口的右下角
  quitButton = new Ui::OctagonButton::OctagonButton("QUIT", this);
  quitButton->setGeometry(width_ / 1.4, height_ * 7 / 8, 50, 40);

  // 点击按钮退出当前页面
  connect(quitButton, &QPushButton::clicked, this, &WarningWidget::hide);
  // 点击按钮发出退出信号
  connect(quitButton, &QPushButton::clicked, this, &WarningWidget::sig_quit);
}
/**************************************************************************
 * 名称: IsButton
 * 功能: 设置按钮是否有效
 * 输入参数: isOk,是否有效
 * 输出参数: NULL
 * 返回值:   void
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-15
 ***************************************************************************/
void WarningWidget::IsButton(bool isOk) {
  if (isOk) {
    // 设置按钮为有效
    quitButton->setEnabled(true);
  } else {
    // 设置按钮为无效
    quitButton->setEnabled(false);
  }
}
/**************************************************************************
 * 名称: paintEvent
 * 功能: 重绘事件
 * 输入参数: QPaintEvent *event, 重绘事件
 * 输出参数: NULL
 * 返回值:   void
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-15
 ***************************************************************************/
void WarningWidget::paintEvent(QPaintEvent *) {

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  // 创建一个从左到右的线性渐变
  QLinearGradient gradient(0, 0, width(), 0);
  gradient.setColorAt(0.0, Qt::blue);
  gradient.setColorAt(0.16, Qt::cyan);
  gradient.setColorAt(0.32, Qt::green);
  gradient.setColorAt(0.48, Qt::yellow);
  gradient.setColorAt(0.64, QColor(255, 165, 0)); // orange
  gradient.setColorAt(0.80, Qt::red);
  gradient.setColorAt(1.0, Qt::magenta);

  // 设置画笔为渐变颜色
  painter.setPen(QPen(gradient, 2));
  painter.drawRect(rect());
  // 设置字体
  QFont font("Arial", width() / 20, QFont::Bold);

  //    // 设置字体颜色
  //    QColor fontColor(Qt::red);
  painter.setFont(font);
  //    painter.setPen(fontColor);

  // 设置渐变色
  QLinearGradient gradientText(0, 0, width(), 0);
  gradientText.setColorAt(0, Qt::red);
  gradientText.setColorAt(0.5, Qt::blue);
  gradientText.setColorAt(1, Qt::green);

  // 创建渐变画刷
  QPen pen(gradientText, 1);

  // 绘制文字
  painter.setPen(pen);

  // 计算文本跳动的偏移量
  int yOffset =
      static_cast<int>(qSin(QDateTime::currentMSecsSinceEpoch() / 200.0) * 10);

  // 设置文本矩形的位置和大小，加上偏移量
  QRect textRect(0, height() / 1.8 + yOffset, width(), height() / 4);

  // 绘制文本
  painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text_);
  // update();
}
