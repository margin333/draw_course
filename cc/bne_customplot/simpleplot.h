#ifndef SIMPLEPLOT_H
#define SIMPLEPLOT_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>

#include <mutex>
#include <condition_variable>
#include <queue>
#include <QDebug>
#include <functional>

#include "qcustomplot.h"

class MyMessage {
public:
    int type;
    double fValueX;
    double fValueY;
    std::function<void()> func;
};

namespace Ui {

namespace Plot {
class SimplePlot : public QCustomPlot {
    Q_OBJECT
public:
    SimplePlot(QWidget *parent = nullptr);
    void SetInitRangeX();
    void SetInitRangeY();

    void ClearCache();

    virtual ~SimplePlot() noexcept;

    void push(const MyMessage &msg);

    void wait(MyMessage &msg);

signals:
    void Draw();

protected:
    void threadFunc();

    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

protected slots:
    void updateGraph();

private:
    std::thread m_thread;
    // 队列
    std::queue<MyMessage> queueTask_;
    // 互斥变量
    std::mutex mtx_;
    // 条件变量
    std::condition_variable cv_;
    //------------------------------------------------------

    int nIndexPre_;
    int nIndexNext_;

    double fCurPosX_;
    double fCurPosY_;

    double fInitRangeX;
    double fInitRangeY;
    QCPItemLine *pVerticalLine_;
    QCPItemLine *pHorizontalLine_;

    QVector<double> vecCacheX_;
    QVector<double> vecCacheY_;
};

} // namespace Plot

} // namespace Ui

#endif // SIMPLEPLOT_H
