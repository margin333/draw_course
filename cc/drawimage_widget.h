#ifndef DRAWIMAGEWIDGET_H
#define DRAWIMAGEWIDGET_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <QDebug>
#include <QPainter>
#include <QWidget>

namespace Ui {
namespace ImgWidget {

struct ImgInfo {
    uint8_t nState = 1;
    QImage img;
};

class DrawImageWidget : public QWidget {
    Q_OBJECT
public:
    explicit DrawImageWidget(QWidget *parent = nullptr);

    void ClearCache();

    virtual ~DrawImageWidget() noexcept;

    void Push(const ImgInfo &msg);

    void Wait(ImgInfo &msg);

signals:
    void DrawImg();

protected:
    void ThreadFunc();

    void paintEvent(QPaintEvent *e) override;

private:
    std::thread thread_;
    // 队列
    std::queue<ImgInfo> queueImg_;
    // 互斥变量

    QImage imgReady_;

    std::mutex mtx_;
    // 条件变量
    std::condition_variable cv_;
};

} // namespace ImgWidget
} // namespace Ui
#endif // DRAWIMAGEWIDGET_H
