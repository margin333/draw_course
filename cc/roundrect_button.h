#ifndef ROUNDRECT_BUTTON_H
#define ROUNDRECT_BUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QPainter>
namespace Ui {
namespace RoundRectBtn {

const QImage g_imgDown(":/FIA/build/bin/image/maintainManageBkg/FIA/img_tabbar_over.png");
const QImage g_imgUp(":/FIA/build/bin/image/maintainManageBkg/FIA/img_tabbar_no.png");

class RoundRectButton : public QPushButton {
    Q_OBJECT
public:
    RoundRectButton(QWidget *parent = nullptr);

    RoundRectButton(const QString &strText, QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

    virtual void mousePressEvent(QMouseEvent *event) override; // 鼠标按下事件

    virtual void mouseReleaseEvent(QMouseEvent *event) override; // 鼠标释放事件

private:
    bool bPress_; // 记录鼠标按下状态

    QColor colorBackground_;
    QColor colorBackgroundPressed_;
    QColor colorTextEnable_;
    QColor colorTextDisenable_;

    // QImage imgDownCached_;
    // QImage imgUpCached_;
};
} // namespace RoundRectBtn
} // namespace Ui
#endif // ROUNDRECT_BUTTON_H
