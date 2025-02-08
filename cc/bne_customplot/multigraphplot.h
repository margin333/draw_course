#ifndef MULTIGRAPHPLOT_H
#define MULTIGRAPHPLOT_H
#include <queue>
#include <thread>
#include <chrono>

#include "qcustomplot.h"

namespace Ui {
namespace Plot {

class PlotData {
public:
    int type = 0;
    unsigned int nIndex = 0;
    double fValueX;
    double fValueY;
};

class MultiGraphPlot : public QCustomPlot {
    Q_OBJECT
public:
    MultiGraphPlot(QWidget *parent = nullptr);
    virtual ~MultiGraphPlot() noexcept;

    QCPGraph *AddGraph(QColor color, QCPAxis *keyAxis = nullptr, QCPAxis *valueAxis = nullptr);

    inline int MaxSize_Graph() { return 4; }

    void SetInitRangeX();
    void SetInitRangeY();

    void ClearCache();

    void push(const PlotData &msg);

    void wait(PlotData &msg);

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
    using QCustomPlot::addGraph;

private:
    std::thread thread_;
    // 队列
    std::queue<PlotData> queueTask_;
    // 互斥变量
    std::mutex mtx_;
    // 条件变量
    std::condition_variable cv_;
    //------------------------------------------------------

    bool bStop_;

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

    std::vector<std::pair<double, double>> vecPt_;
};
} // namespace Plot
} // namespace Ui
#endif // MULTIGRAPHPLOT_H
