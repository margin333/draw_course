#include "BNEWasher.h"
#include "ui_BNEWasher.h"

namespace Ui {
namespace Washer {

qreal cos30 = qCos(qDegreesToRadians(30.0));
qreal sin30 = qSin(qDegreesToRadians(30.0));

/**************************************************************************
 * 名称:  BNEWasher
 * 功能:  BNEWasher类构造函数
 * 输入参数:  data：电机校正量结构体，type：电机类型，parent：父对象
 * 输出参数:  NULL
 * 返回值:   NULL
 * 创建者:   yexin
 * 创建时间: 2024-08-01
 ***************************************************************************/
BNEWasher::BNEWasher(WasherType type, QWidget *parent)
    : QWidget(parent), ui(new Ui::BNEWasher), nType(type) {
  ui->setupUi(this);
  initialize();
}

/**************************************************************************
 * 名称:  ～BNEWasher
 * 功能:  BNEWasher类析构函数
 * 输入参数:  NULL
 * 输出参数:  NULL
 * 返回值:   NULL
 * 创建者:   yexin
 * 创建时间: 2024-08-01
 ***************************************************************************/
BNEWasher::~BNEWasher() { delete ui; }

/**************************************************************************
 * 名称:  eventFilter
 * 功能:  重载事件过滤器，处理捕获到的绘画事件
 * 输入参数: watched：事件产生者，event：事件
 * 输出参数:  NULL
 * 返回值:   返回父对象的事件处理，不阻碍其他事件
 * 创建者:   yexin
 * 创建时间: 2024-08-01
 ***************************************************************************/
bool BNEWasher::eventFilter(QObject *watched, QEvent *event) {
  if (watched == ui->widget && event->type() == QEvent::Paint) {
    showPaint(this->nType); // 响应函数
  }
  return QWidget::eventFilter(watched, event);
}

/**************************************************************************
 * 名称:  showPaint
 * 功能:  根据电机类型绘制电机图形
 * 输入参数:type:电机类型
 * 输出参数:  NULL
 * 返回值:   NULL
 * 创建者:   yexin
 * 创建时间: 2024-08-01
 ***************************************************************************/
void BNEWasher::showPaint(WasherType type) {

  // 设置笔刷
  QPainter painter(ui->widget);
  painter.setPen(Qt::blue);
  painter.setBrush(Qt::NoBrush);
  painter.setRenderHint(QPainter::Antialiasing);
  // 设置绘图区域和大小
  int width = ui->widget->width();
  int height = ui->widget->height();
  int fontSize = 20;
  qreal minLength = width < height ? width : height;
  minLength /= 2;
  qreal centerX = width - minLength;
  qreal centerY = minLength;
  // qDebug()<<"center X:"<<centerX<<"  Y:"<<centerY;
  QPointF centerPoint = QPointF(centerX, centerY);

  switch (type) {
  case WasherType::Circle: {
    PaintCirle(&painter, centerPoint, minLength - fontSize, fontSize);
    break;
  }
  case WasherType::Rect: {
    PaintRect(&painter, centerX - minLength + fontSize, fontSize,
              2 * (minLength - fontSize), fontSize);
    break;
  }
  default: {
    return;
  }
  }
}

/**************************************************************************
 * 名称:  PaintCirle
 * 功能:  绘制圆形电机
 * 输入参数:painter：画笔，center：圆点，radius：半径，fontSize：字体大小
 * 输出参数:  NULL
 * 返回值:   NULL
 * 创建者:   yexin
 * 创建时间: 2024-08-01
 ***************************************************************************/
void BNEWasher::PaintCirle(QPainter *painter, QPointF center, qreal radius,
                           qreal fontSize) {
  int minRadius = 4;
  qreal x = center.x();
  qreal y = center.y();
  painter->save();

  painter->drawText(
      QRectF(x - fontSize / 4, y - radius - fontSize, fontSize, fontSize), "1");
  painter->drawText(
      QRectF(x + radius * cos30, y + radius * sin30, fontSize, fontSize), "3");
  painter->drawText(QRectF(x - radius * cos30 - fontSize / 2,
                           y + radius * sin30, fontSize, fontSize),
                    "2");

  painter->drawEllipse(center, radius, radius);
  painter->setBrush(Qt::blue);
  painter->drawEllipse(QPointF(x, y - radius), minRadius, minRadius);
  painter->drawEllipse(QPointF(x + radius * cos30, y + radius * sin30),
                       minRadius, minRadius);
  painter->drawEllipse(QPointF(x - radius * cos30, y + radius * sin30),
                       minRadius, minRadius);
  painter->restore();
}

/**************************************************************************
 * 名称:  PaintRect
 * 功能:  绘制矩形电机
 * 输入参数:painter：画笔，pointX：矩形左上角x坐标，pointY：矩形左上角y坐标，length：矩形边长，fontSize：字体大小
 * 输出参数:  NULL
 * 返回值:   NULL
 * 创建者:   yexin
 * 创建时间: 2024-08-01
 ***************************************************************************/
void BNEWasher::PaintRect(QPainter *painter, qreal pointX, qreal pointY,
                          qreal length, qreal fontSize) {

  painter->save();
  int radius = 4;

  painter->drawText(
      QRectF(pointX - fontSize / 2, pointY - fontSize, fontSize, fontSize),
      "1");
  painter->drawText(
      QRectF(pointX + length, pointY - fontSize, fontSize, fontSize), "2");
  painter->drawText(
      QRectF(pointX - fontSize / 2, pointY + length, fontSize, fontSize), "3");
  painter->drawText(
      QRectF(pointX + length, pointY + length, fontSize, fontSize), "4");

  painter->drawRect(pointX, pointY, length, length);
  painter->setBrush(Qt::blue);
  painter->drawEllipse(QPoint(pointX, pointY), radius, radius);
  painter->drawEllipse(QPoint(pointX, pointY + length), radius, radius);
  painter->drawEllipse(QPoint(pointX + length, pointY), radius, radius);
  painter->drawEllipse(QPoint(pointX + length, pointY + length), radius,
                       radius);
  painter->restore();
}

/**************************************************************************
 * 名称:  onClickedSendButton
 * 功能:  点击send按钮后触发数据发送
 * 输入参数：NULL
 * 输出参数:  NULL
 * 返回值:   NULL
 * 创建者:   yexin
 * 创建时间: 2024-08-01
 ***************************************************************************/
void BNEWasher::onClickedSendButton() {
  // 如果有需求，可以加正则进行判断，此处也没做转换失败的处理
  std::vector<double> tmpData;
  tmpData.push_back(ui->LineEdit->text().toDouble());
  tmpData.push_back(ui->LineEdit_2->text().toDouble());
  tmpData.push_back(ui->LineEdit_3->text().toDouble());
  tmpData.push_back(ui->LineEdit_4->text().toDouble());

  // 信号发送，通常此信号绑定到外部类，从而通知外部类进行数据更新
  emit sendDataSig(tmpData);
}

/**************************************************************************
 * 名称:  initialize
 * 功能:  初始化函数
 * 输入参数：NULL
 * 输出参数:  NULL
 * 返回值:   NULL
 * 创建者:   yexin
 * 创建时间: 2024-08-01
 ***************************************************************************/
void BNEWasher::initialize() {

  this->setMinimumSize(220, 162);
  //  安装事件过滤器
  ui->widget->installEventFilter(this);
  // 隐藏第四项
  if (nType == WasherType::Circle) {
    this->setMinimumSize(220, 140);
    ui->Label_4->setVisible(false);
    ui->LineEdit_4->setVisible(false);
    ui->label_4->setVisible(false);
  }
  connect(ui->pushButton, &QPushButton::clicked, this,
          &BNEWasher::onClickedSendButton);
}

/**************************************************************************
 * 名称:  setData
 * 功能:  设置电机校正量并显示
 * 输入参数：data：电机校正量 数组
 * 输出参数:  NULL
 * 返回值:   NULL
 * 创建者:   yexin
 * 创建时间: 2024-08-01
 ***************************************************************************/
void BNEWasher::setData(const std::vector<double> &data)  {

  if (data.size() < 3 || (data.size() == 3 && nType == WasherType::Rect)) {
    // 在此进行错误处理
    return;
  }
  ui->LineEdit->setText(QString::number(data.at(0), 'g', 6));
  ui->LineEdit_2->setText(QString::number(data.at(1), 'g', 6));
  ui->LineEdit_3->setText(QString::number(data.at(2), 'g', 6));
  if (nType == WasherType::Rect) {
    ui->LineEdit_4->setText(QString::number(data.at(3), 'g', 6));
  }
}

/**************************************************************************
 * 名称:  getData
 * 功能:  获取界面上的电机校正量显示值
 * 输入参数：NULL
 * 输出参数:  NULL
 * 返回值:   电机校正值 数组
 * 创建者:   yexin
 * 创建时间: 2024-08-01
 ***************************************************************************/
std::vector<double> BNEWasher::getData() {
  std::vector<double> tmpData;
  tmpData.push_back(ui->LineEdit->text().toDouble());
  tmpData.push_back(ui->LineEdit_2->text().toDouble());
  tmpData.push_back(ui->LineEdit_3->text().toDouble());
  tmpData.push_back(ui->LineEdit_4->text().toDouble());

  return tmpData;
}

/**************************************************************************
 * 名称:  setButtonVisible
 * 功能:  设置发送数据按钮是否可见
 * 输入参数：flag：是否可见
 * 输出参数:  NULL
 * 返回值:   NULL
 * 创建者:   yexin
 * 创建时间: 2024-08-05
 ***************************************************************************/
void BNEWasher::setButtonVisible(bool flag) {
  return ui->pushButton->setVisible(flag);
}

/**************************************************************************
 * 名称:  getWasherType
 * 功能:  获取Washer的类型
 * 输入参数：NULL
 * 输出参数:  NULL
 * 返回值:   Washer类型
 * 创建者:   yexin
 * 创建时间: 2024-08-05
 ***************************************************************************/
WasherType BNEWasher::getWasherType() { return this->nType; }

} // namespace Washer

} // namespace Ui
