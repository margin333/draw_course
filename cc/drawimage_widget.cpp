#include "drawimage_widget.h"
namespace Ui {
namespace ImgWidget {

DrawImageWidget::~DrawImageWidget() noexcept
{
    ClearCache();
    ImgInfo msgEnd;
    msgEnd.nState = 0;
    this->Push(msgEnd);
    thread_.join();
}

DrawImageWidget::DrawImageWidget(QWidget *parent)
    : QWidget{parent}
{
    this->resize(100, 100); // default size

    connect(this, &DrawImageWidget::DrawImg, this, [&]() { this->update(); });

    thread_ = std::thread(&DrawImageWidget::ThreadFunc, this);
}

void DrawImageWidget::ThreadFunc()
{
    ImgInfo msg;
    while (1) {
        this->Wait(msg);
        if (msg.nState == 0) {
            break;
        } else if (msg.nState == 1) {
            std::unique_lock<std::mutex> lck(mtx_);
            static int j = 0;
            ++j;
            imgReady_.swap(msg.img);
            emit this->DrawImg();
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
};

void DrawImageWidget::Push(const ImgInfo &msg)
{
    std::unique_lock<std::mutex> lck(mtx_);
    queueImg_.push(msg);
    cv_.notify_all();
}

void DrawImageWidget::Wait(ImgInfo &msg)
{
    std::unique_lock<std::mutex> lck(mtx_);
    cv_.wait(lck, [&] { return !queueImg_.empty(); });
    msg = queueImg_.front();
    queueImg_.pop();
}

void DrawImageWidget::ClearCache()
{
    std::unique_lock<std::mutex> lck(mtx_);
    while (!queueImg_.empty()) {
        queueImg_.pop();
    }
    QImage img;
    imgReady_.swap(img);
    emit this->DrawImg();
}

void DrawImageWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (!imgReady_.isNull()) {
        painter.drawImage(this->rect(), imgReady_, imgReady_.rect());
    } else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::gray);
        painter.drawRect(this->rect());
    }
    return;
}
} // namespace ImgWidget
} // namespace Ui
