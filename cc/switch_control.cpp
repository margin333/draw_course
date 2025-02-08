#include "switch_control.h"
#include "ui_switch_control.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtGlobal>

namespace Ui {
namespace custom {

SwitchControl::SwitchControl(QWidget *parent)
    : QWidget(parent)
    , m_bChecked(false)
    , m_background(Qt::black)
    , m_checkedColor(QColor(0, 255, 0))
    , m_disabledColor(QColor(255, 0, 0))
    , m_thumbColor(Qt::white)
    , m_radius(10)
    , m_nHeight(20)
    , m_nMargin(0)
    , m_openText("Open") // Initialize with default values
    , m_closeText("Close")
{
    setCursor(Qt::PointingHandCursor);
    setMinimumHeight(m_nHeight);
    setMinimumWidth(m_nHeight * 2);
    setMaximumWidth(400);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

// 鼠标按下事件
void SwitchControl::mousePressEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if (event->buttons() & Qt::LeftButton) {
            event->accept();
        } else {
            event->ignore();
        }
    }
}

// 鼠标释放事件 - 切换开关状态、发射toggled()信号
void SwitchControl::mouseReleaseEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if ((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton)) {
            event->accept();
            m_bChecked = !m_bChecked;
            emit toggled(m_bChecked);
            m_timer.start(3);
        } else {
            event->ignore();
        }
    }
}

// 大小改变事件
void SwitchControl::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_radius = height() / 2; // 根据新高度调整半径
    m_nHeight = height();
    m_nX = m_nHeight / 2;
    m_nY = m_nHeight / 2;
}

// 切换状态 - 滑动
void SwitchControl::onTimeout()
{
    int width1 = this->width();
    int height = this->height();
    if (m_bChecked) {
        m_nX += 1;
        if (m_nX >= width() - m_nHeight / 2)
            m_timer.stop();
    } else {
        m_nX -= 1;
        if (m_nX <= m_nHeight / 2)
            m_timer.stop();
    }
    update();
}

// 返回开关状态 - 打开：true 关闭：false
bool SwitchControl::isToggled() const { return m_bChecked; }

// 设置开关状态
void SwitchControl::setToggle(bool checked)
{
    // 更新开关状态
    m_bChecked = checked;

    // 重置滑块的初始位置
    m_nX = m_bChecked ? (width() - m_nHeight / 2) : (m_nHeight / 2);

    // 启动定时器以滑动到目标位置
    m_timer.start(3);
}

// 设置背景颜色
void SwitchControl::setBackgroundColor(QColor color) { m_background = color; }

// 设置选中颜色
void SwitchControl::setCheckedColor(QColor color) { m_checkedColor = color; }

// 设置不可用颜色
void SwitchControl::setDisbaledColor(QColor color) { m_disabledColor = color; }

// Draw switch
void SwitchControl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    m_radius = height() / 2; // 根据新高度调整半径
    m_nHeight = height();

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    QColor background;
    QColor thumbColor;
    qreal dOpacity;
    QString stateStr;
    QRectF rect;
    QPen pen(QBrush(QColor(255, 255, 255)), 1);
    QFont font("黑体", 16, QFont::Normal);

    if (isEnabled()) {
        if (m_bChecked) {
            background = m_checkedColor;
            thumbColor = m_thumbColor;
            dOpacity = 0.600;
            stateStr = m_openText; // Use custom open text
        } else {
            background = m_disabledColor;
            thumbColor = m_thumbColor;
            dOpacity = 0.800;
            stateStr = m_closeText; // Use custom close text
        }

        // 创建 QFontMetrics 对象以获取文本的度量信息
        QFontMetrics fmt(font);
        int textWidth = fmt.horizontalAdvance(stateStr);
        int textHeight = fmt.height();

        // 计算可用宽度
        int availableWidth = width() - 2 * m_nHeight;           // 控件宽度减去 2 个控件高度
        if (textWidth > availableWidth && font.pointSize() > 5) { // 设置最小字体大小为 5
            int newFontSize = (availableWidth * 16) / textWidth;  // 根据可用宽度调整字体大小
            font.setPointSize(newFontSize);
            fmt = QFontMetrics(font); // 重新计算字体度量
            textWidth = fmt.horizontalAdvance(stateStr);
            textHeight = fmt.height();
        }

        int rectX = (width() - textWidth) / 2;
        int rectY = (height() - textHeight) / 2;

        rect = QRectF(rectX, rectY, textWidth, textHeight);
    } else {
        background = m_background;
        dOpacity = 0.260;
        thumbColor = m_disabledColor;
    }

    painter.setBrush(background);
    painter.setOpacity(dOpacity);
    path.addRoundedRect(QRectF(m_nMargin, m_nMargin, width() - 2 * m_nMargin, height() - 2 * m_nMargin), m_radius,
                        m_radius);
    painter.drawPath(path.simplified());

    painter.setBrush(thumbColor);
    painter.setOpacity(1.0);
    painter.drawEllipse(QRectF(m_nX - (m_nHeight / 2), m_nY - (m_nHeight / 2), height(), height()));

    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, stateStr);
}

// Add setter for open text
void SwitchControl::setOpenText(const QString &text)
{
    m_openText = text;
    update(); // Redraw the widget
}

// Add setter for close text
void SwitchControl::setCloseText(const QString &text)
{
    m_closeText = text;
    update(); // Redraw the widget
}

// Remaining methods unchanged...

SwitchControl::~SwitchControl() { }

} // namespace custom
} // namespace Ui
