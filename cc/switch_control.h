#ifndef SWITCH_CONTROL_H
#define SWITCH_CONTROL_H

#include <QObject>
#include <QWidget>
#include <QTimer>

namespace Ui {
class SwitchControl;
} // namespace Ui

namespace Ui {
namespace custom {
class SwitchControl : public QWidget {
    Q_OBJECT

public:
    explicit SwitchControl(QWidget *parent = nullptr);
    ~SwitchControl();
    // 返回开关状态 - 打开：true 关闭：false
    bool isToggled() const;

    // 设置开关状态
    void setToggle(bool checked);

    // 设置背景颜色
    void setBackgroundColor(QColor color);

    // 设置选中颜色
    void setCheckedColor(QColor color);

    // 设置不可用颜色
    void setDisbaledColor(QColor color);

protected:
    // 绘制开关
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    // 鼠标按下事件
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // 鼠标释放事件 - 切换开关状态、发射toggled()信号
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    // 大小改变事件
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

signals:
    // 状态改变时，发射信号
    void toggled(bool checked);

private slots:
    // 状态切换时，用于产生滑动效果
    void onTimeout();

private:
    bool m_bChecked;        // 是否选中
    QColor m_background;    // 背景颜色
    QColor m_checkedColor;  // 选中颜色
    QColor m_disabledColor; // 不可用颜色
    QColor m_thumbColor;    // 拇指颜色
    qreal m_radius;         // 圆角
    qreal m_nX;             // 圆心x点坐标
    qreal m_nY;             // 圆心y点坐标
    qint16 m_nHeight;       // 高度
    qint16 m_nMargin;       // 外边距
    QTimer m_timer;         // 定时器
    QString m_openText;     // 打开显示的文本
    QString m_closeText;    // 关闭显示的文本
    void setCloseText(const QString &text);
    void setOpenText(const QString &text);
};

} // namespace custom
} // namespace Ui

#endif // SWITCH_CONTROL_H
