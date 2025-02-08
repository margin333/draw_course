#include "circularprogress.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTimer>
/**************************************************************************
 * 名称: CircularProgress
 * 功能: CircularProgress类是一个自定义的圆形进度条，继承自QProgressBar。
 * 输入参数: QWidget *parent, 父部件
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
CircularProgress::CircularProgress(QWidget *parent) : QProgressBar(parent) {
  init();
}
/**************************************************************************
 * 名称: ~CircularProgress
 * 功能: 析构函数
 * 输入参数: NULL
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
CircularProgress::~CircularProgress() {}
/**************************************************************************
 * 名称: setProgressType
 * 功能: 设置进度条的类型
 * 输入参数: ProgressType type, 进度条的类型
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
void CircularProgress::setProgressType(ProgressType type) {
  progress_Type = type;
  update();
}
/**************************************************************************
 * 名称: progressType
 * 功能: 获取进度条的类型
 * 输入参数: NULL
 * 输出参数: NULL
 * 返回值:   ProgressType, 进度条的类型
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
CircularProgress::ProgressType CircularProgress::progressType() const {
  return progress_Type;
}
/**************************************************************************
 * 名称: setUseThemeColors
 * 功能: 设置是否使用主题颜色
 * 输入参数: bool value, 是否使用主题颜色
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
void CircularProgress::setUseThemeColors(bool value) {
  if (m_useThemeColors == value) {
    return;
  }

  m_useThemeColors = value;
  update();
}
/**************************************************************************
 * 名称: useThemeColors
 * 功能: 获取是否使用主题颜色
 * 输入参数: NULL
 * 输出参数: NULL
 * 返回值:   bool, 是否使用主题颜色
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
bool CircularProgress::useThemeColors() const { return m_useThemeColors; }
/**************************************************************************
 * 名称: setLineWidth
 * 功能: 设置线宽
 * 输入参数: qreal width, 线宽
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
void CircularProgress::setLineWidth(qreal width) {
  m_penWidth = width;
  update();
  updateGeometry();
}
/**************************************************************************
 * 名称: lineWidth
 * 功能: 获取线宽
 * 输入参数: NULL
 * 输出参数: NULL
 * 返回值:   qreal, 线宽
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
qreal CircularProgress::lineWidth() const { return m_penWidth; }
/**************************************************************************
 * 名称: setSize
 * 功能: 设置大小
 * 输入参数: int size, 大小
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
void CircularProgress::setSize(int size) {
  m_size = size;
  update();
  updateGeometry();
}
/**************************************************************************
 * 名称: size
 * 功能: 获取大小
 * 输入参数: NULL
 * 输出参数: NULL
 * 返回值:   int, 大小
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
int CircularProgress::size() const { return m_size; }
/**************************************************************************
 * 名称: setColor
 * 功能: 设置颜色
 * 输入参数: const QColor &color, 颜色
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
void CircularProgress::setColor(const QColor &color) {
  m_color = color;
  update();
}
/**************************************************************************
 * 名称: color
 * 功能: 获取颜色
 * 输入参数: NULL
 * 输出参数: NULL
 * 返回值:   QColor, 颜色
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
QColor CircularProgress::color() const {
  if (m_useThemeColors || !m_color.isValid()) {
    return QColor(37, 177, 218);
  } else {
    return m_color;
  }
}
/**************************************************************************
 * 名称: sizeHint
 * 功能:
 *获取大小提示,可以在布局管理器等场景中使用。布局管理器可以查询部件的首选大小，并根据布局策略进行调整和安排。
 * 输入参数: NULL
 * 输出参数: NULL
 * 返回值:   QSize, 大小
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
QSize CircularProgress::sizeHint() const {
  const qreal s = m_size + m_penWidth + 8;
  return QSize(s, s);
}
/**************************************************************************
 * 名称: paintEvent
 * 功能: 绘制事件
 * 输入参数: QPaintEvent *event, 事件
 * 输出参数: NULL
 * 返回值:   NULL
 * 创建者:   ZhouWeize
 * 创建时间: 2024-04-10
 ***************************************************************************/
void CircularProgress::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing); // 启用了抗锯齿功能。

  if (!isEnabled()) { // 在禁用状态下，仅执行绘图操作，并不执行后续的代码。
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(m_penWidth);
    pen.setColor(QColor(186, 186, 186));
    painter.setPen(pen);
    painter.drawLine(rect().center() - QPointF(20, 20),
                     rect().center() + QPointF(20, 20)); // 绘制一条线
    painter.drawLine(rect().center() + QPointF(20, -20),
                     rect().center() - QPointF(20, -20));
    return;
  }

  if (IndeterminateProgress == progress_Type) {
    painter.translate(width() / 2, height() / 2); // 平移坐标原点
    painter.rotate(m_delegate->angle());          // 旋转坐标系
  }

  QPen pen;
  pen.setCapStyle(Qt::RoundCap); // 设置线帽
  pen.setWidthF(m_penWidth);     // 设置线宽

  // 绘制底部环 ，底部灰色的环
  pen.setColor(QColor(165, 165, 165)); // 设置颜色
  painter.setPen(pen);
  painter.drawEllipse(QPoint(0, 0), m_size / 2, m_size / 2); // 绘制椭圆
  QLinearGradient gradient(0, 0, width(), 0);
  gradient.setColorAt(0.0, Qt::cyan);
  gradient.setColorAt(0.16, QColor(173, 216, 230)); // 淡蓝色
  gradient.setColorAt(0.32, Qt::green);
  gradient.setColorAt(0.48, Qt::yellow);
  gradient.setColorAt(0.64, QColor(255, 165, 0)); // orange
  gradient.setColorAt(0.80, Qt::red);
  gradient.setColorAt(1.0, Qt::magenta);
  QPen penProgress(Qt::cyan, m_penWidth); // 使用渐变色作为画笔颜色
  painter.setPen(penProgress);

  if (IndeterminateProgress == progress_Type) { // 等待进度条
    QVector<qreal> pattern;                     // 创建一个实数向量
    pattern << m_delegate->dashLength() * m_size / 50 << 30 * m_size / 50;

    penProgress.setDashOffset(m_delegate->dashOffset() * m_size /
                              50);       // 设置了画笔的虚线偏移量
    penProgress.setDashPattern(pattern); // 设置了画笔的虚线模式

    painter.setPen(penProgress);
    painter.drawEllipse(
        QPoint(0, 0), m_size / 2, // 就是原型进度条的大小
        m_size / 2); // 使用painter对象绘制一个椭圆。椭圆的中心点是坐标(0,
                     // 0)，宽度和高度都是m_size / 2。
  } else {
    painter.setPen(pen);

    const qreal x = (width() - m_size) / 2;
    const qreal y = (height() - m_size) / 2;

    const qreal a = 360 * (value() - minimum()) / (maximum() - minimum());

    QPainterPath path;
    path.arcMoveTo(x, y, m_size, m_size, 0);
    path.arcTo(x, y, m_size, m_size, 0, a);

    painter.drawPath(path);
  }
  //    painter.setBrush(Qt::red);
  //    painter.drawRect(rect());
}

void CircularProgress::init() // 初始化
{
  m_delegate = new CircularProgressDelegate(this);
  progress_Type = IndeterminateProgress; // 等待进度条
  m_penWidth = 6.25;
  m_size = 64;
  m_useThemeColors = true;
  //  通过QSizePolicy类的构造函数，将水平和垂直方向上的大小策略都设置为MinimumExpanding，表示部件在这两个方向上都希望尽可能地扩展。
  setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                            QSizePolicy::MinimumExpanding));

  QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
  group->setLoopCount(
      -1); // 设置并行动画组的循环次数为无限循环，即动画会一直循环播放

  QPropertyAnimation *animation; // 属性动画
  animation = new QPropertyAnimation(this);
  animation->setPropertyName("dashLength");
  animation->setTargetObject(m_delegate);
  animation->setEasingCurve(QEasingCurve::InOutQuad);
  animation->setStartValue(0.1);
  animation->setKeyValueAt(0.15, 0.2);
  animation->setKeyValueAt(0.6, 20);
  animation->setKeyValueAt(0.7, 20);
  animation->setEndValue(20);
  animation->setDuration(2050);

  group->addAnimation(animation); // 添加动画

  animation = new QPropertyAnimation(this);
  animation->setPropertyName("dashOffset");
  animation->setTargetObject(m_delegate);
  animation->setEasingCurve(QEasingCurve::InOutSine);
  animation->setStartValue(0);
  animation->setKeyValueAt(0.15, 0);
  animation->setKeyValueAt(0.6, -7);
  animation->setKeyValueAt(0.7, -7);
  animation->setEndValue(-25);
  animation->setDuration(2050);

  group->addAnimation(animation);

  animation = new QPropertyAnimation(this);
  animation->setPropertyName("angle");
  animation->setTargetObject(m_delegate);
  animation->setStartValue(0);
  animation->setEndValue(719);
  animation->setDuration(2050);

  group->addAnimation(animation);

  group->start();
}

CircularProgressDelegate::CircularProgressDelegate(CircularProgress *parent)
    : QObject(parent), progress_(parent), m_dashOffset(0), m_dashLength(89),
      m_angle(0) {
  Q_ASSERT(parent);
}

CircularProgressDelegate::~CircularProgressDelegate() {}

void CircularProgressDelegate::setDashOffset(qreal offset) {
  m_dashOffset = offset; // 设置偏移
  progress_->update();
}

qreal CircularProgressDelegate::dashOffset() const { return m_dashOffset; }

void CircularProgressDelegate::setDashLength(qreal length) {
  m_dashLength = length;
  progress_->update();
}

qreal CircularProgressDelegate::dashLength() const { return m_dashLength; }

void CircularProgressDelegate::setAngle(int angle) {
  m_angle = angle;
  progress_->update();
}

int CircularProgressDelegate::angle() const { return m_angle; }
