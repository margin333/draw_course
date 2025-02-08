/**************************************************************************
 * Copyright(C),2024,BNE
 * 模块名称:  镜头驱动电机数据调整量显示   自定义控件
 * 模块功能:   根据驱动电机种类显示对应的电机形状，并显示镜头的校正计算结果
 * 创建者:     yexin
 * 创建日期:   2024-08-01
 ***************************************************************************/
#ifndef BNEWASHER_H
#define BNEWASHER_H

#include <QDebug>
#include <QPainter>
#include <QWidget>
#include <QtMath>
#include <vector>

namespace Ui {
class BNEWasher;
} // namespace Ui

namespace Ui {
namespace Washer {

// 电机种类
enum WasherType { Circle = 1, Rect };

class BNEWasher : public QWidget {
  Q_OBJECT

public:
  BNEWasher(WasherType type = WasherType::Circle, QWidget *parent = nullptr);
  ~BNEWasher();
  // 设置校正数据
  void setData(const std::vector<double> &data);
  // 获取校正数据。当作为其他类的成员时，可以直接调用此函数，获取数据
  std::vector<double> getData();
  // 设置按钮是否可见
  void setButtonVisible(bool flag);
  // 获取电机类型
  WasherType getWasherType();

private:
  Ui::BNEWasher *ui; // 界面ui
  WasherType nType;  // 电机类型

protected:
  // 事件过滤器，用于处理电机图形绘画事件（重载）
  bool eventFilter(QObject *watched, QEvent *event) override;

private:
  // 初始化
  void initialize();
  // 电机图形绘画
  void showPaint(WasherType type);
  // 绘制圆形电机
  void PaintCirle(QPainter *painter, QPointF center, qreal radius,
                  qreal fontSize);
  // 绘制矩形电机
  void PaintRect(QPainter *painter, qreal pointX, qreal pointY, qreal length,
                 qreal fontSize);

signals:
  // 向外部类发送校正数据的信号
  void sendDataSig(std::vector<double> data);
private slots:
  // 点击发送数据时，执行的函数
  void onClickedSendButton();
};

} // namespace Washer
} // namespace Ui
#endif // BNEWASHER_H
