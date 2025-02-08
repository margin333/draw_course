#ifndef CIRCULARPROGRESS_H
#define CIRCULARPROGRESS_H

#include <QEvent>
#include <QProgressBar>
#include <QObject>

class CircularProgressDelegate;
class CircularProgress : public QProgressBar {
    Q_OBJECT

    Q_PROPERTY(qreal lineWidth WRITE setLineWidth READ lineWidth)
    Q_PROPERTY(qreal size WRITE setSize READ size)
    Q_PROPERTY(QColor color WRITE setColor READ color)

public:
    explicit CircularProgress(QWidget *parent = nullptr);
    ~CircularProgress();

    enum ProgressType {
        DeterminateProgress,  // 普通进度条
        IndeterminateProgress // 等待进度条
    };

    void setProgressType(ProgressType type);             // 设置进度条类型
    CircularProgress::ProgressType progressType() const; // 获取进度条类型

    void setUseThemeColors(bool value); // 设置是否使用主题颜色
    bool useThemeColors() const;        // 获取是否使用主题颜色

    void setLineWidth(qreal width); // 设置线宽
    qreal lineWidth() const;        // 获取线宽

    void setSize(int size); // 设置大小
    int size() const;       // 获取大小

    void setColor(const QColor &color); // 设置颜色
    QColor color() const;               // 获取颜色

    QSize sizeHint() const Q_DECL_OVERRIDE; // 获取大小提示

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE; // 绘制事件

private:
    void init();

private:
    CircularProgressDelegate *m_delegate; // 环形进度条代理
    ProgressType progress_Type;           // 进度条类型
    QColor m_color;                       // 颜色
    qreal m_penWidth;                     // 线宽
    int m_size;                           // 大小
    bool m_useThemeColors;                // 是否使用主题颜色
};
// 控制对环形进度条的访问和操作。通过设置代理事件，可以将外部的事件或请求传递给代理类进行处理，从而间接地操作环形进度条。
class CircularProgressDelegate : public QObject {
    Q_OBJECT

    Q_PROPERTY(qreal dashOffset WRITE setDashOffset READ dashOffset) // 设置虚线偏移
    Q_PROPERTY(qreal dashLength WRITE setDashLength READ dashLength) // 设置虚线长度
    Q_PROPERTY(int angle WRITE setAngle READ angle)                  // 设置角度

public:
    CircularProgressDelegate(CircularProgress *parent);
    ~CircularProgressDelegate();

    void setDashOffset(qreal offset); // 设置虚线偏移
    qreal dashOffset() const;         // 获取虚线偏移

    void setDashLength(qreal length); // 设置虚线长度
    qreal dashLength() const;         // 获取虚线长度

    void setAngle(int angle); // 设置角度
    int angle() const;        // 获取角度

private:
    Q_DISABLE_COPY(CircularProgressDelegate)

    CircularProgress *const progress_; // 环形进度条
    qreal m_dashOffset;                // 虚线偏移
    qreal m_dashLength;                // 虚线长度
    int m_angle;                       // 角度
};

#endif // CIRCULARPROGRESS_H
