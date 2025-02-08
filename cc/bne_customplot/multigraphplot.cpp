#include "multigraphplot.h"

namespace Ui {
namespace Plot {

MultiGraphPlot::~MultiGraphPlot() noexcept
{
    ClearCache();
    {
        std::unique_lock<std::mutex> lock(mtx_);
        bStop_ = true;
        PlotData msgEnd;
        msgEnd.type = 0;
        queueTask_.push(msgEnd);
        cv_.notify_one();
    }

    thread_.join();
}

MultiGraphPlot::MultiGraphPlot(QWidget *parent)
    : QCustomPlot(parent)
    , bStop_(false)
    , nIndexPre_(-1)
    , nIndexNext_(-1)
    , fCurPosX_(0.0)
    , fCurPosY_(0.0)
    , fInitRangeX(10)
    , fInitRangeY(10)
    , pVerticalLine_(nullptr)
    , pHorizontalLine_(nullptr)
{
    pVerticalLine_ = new QCPItemLine(this);
    if (pVerticalLine_) {
        pVerticalLine_->setPen(QPen(QColor(255, 51, 0)));
        pVerticalLine_->setVisible(false);
    }
    pHorizontalLine_ = new QCPItemLine(this);
    if (pHorizontalLine_) {
        pHorizontalLine_->setPen(QPen(QColor(255, 51, 0)));
        pHorizontalLine_->setVisible(false);
    }
    if (this->xAxis && this->yAxis) {
        this->xAxis->setLabel("X-Axis");
        this->yAxis->setLabel("Y-Axis");
    }

    this->axisRect()->setBackground(QColor(245, 249, 255));

    this->xAxis->grid()->setPen(QPen(QColor(165, 174, 209), 1, Qt::DotLine));
    this->yAxis->grid()->setPen(QPen(QColor(165, 174, 209), 1, Qt::DotLine));
    this->xAxis->grid()->setSubGridPen(QPen(QColor(165, 174, 209), 1, Qt::DotLine));
    this->yAxis->grid()->setSubGridPen(QPen(QColor(165, 174, 209), 1, Qt::DotLine));
    this->xAxis->grid()->setSubGridVisible(true);
    this->yAxis->grid()->setSubGridVisible(true);

    //----------------------------------------------------------------------------------------
    this->yAxis2->setBasePen(QPen(QColor(165, 174, 209), 1));
    this->yAxis2->setTickPen(QPen(QColor(165, 174, 209), 1));
    this->yAxis2->setSubTickPen(QPen(QColor(165, 174, 209), 1));

    this->xAxis2->setBasePen(QPen(QColor(165, 174, 209), 1));
    this->xAxis2->setTickPen(QPen(QColor(165, 174, 209), 1));
    this->xAxis2->setSubTickPen(QPen(QColor(165, 174, 209), 1));

    this->yAxis->setBasePen(QPen(QColor(165, 174, 209), 1));
    this->yAxis->setTickPen(QPen(QColor(165, 174, 209), 1));
    this->yAxis->setSubTickPen(QPen(QColor(165, 174, 209), 1));

    this->xAxis->setBasePen(QPen(QColor(165, 174, 209), 1));
    this->xAxis->setTickPen(QPen(QColor(165, 174, 209), 1));
    this->xAxis->setSubTickPen(QPen(QColor(165, 174, 209), 1));

    //----------------------------------------------------------------------------------------
    this->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    this->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    this->xAxis->grid()->setZeroLinePen(QPen(QColor(165, 174, 209)));
    this->yAxis->grid()->setZeroLinePen(QPen(QColor(165, 174, 209)));

    //-----------------------------------------------------------------------------------------

    this->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    this->xAxis->setRange(0, 10, Qt::AlignCenter);
    this->yAxis->setRange(0, 10, Qt::AlignCenter);

    //--------------------------------------------------------------------------------------------
    connect(this, &MultiGraphPlot::Draw, this, &MultiGraphPlot::updateGraph);

    thread_ = std::thread(&MultiGraphPlot::threadFunc, this);
}

QCPGraph *MultiGraphPlot::AddGraph(QColor color, QCPAxis *keyAxis, QCPAxis *valueAxis)
{
    if (this->graphCount() < 4 && this->addGraph(keyAxis, valueAxis)) {
        this->graph()->setPen(color);

        this->graph()->setLineStyle(QCPGraph::lsLine);
        this->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));
        return this->graph();
    } else {
        return nullptr;
    }
}

void MultiGraphPlot::SetInitRangeX() { fInitRangeX = xAxis->range().size(); };

void MultiGraphPlot::SetInitRangeY() { fInitRangeY = yAxis->range().size(); };

void MultiGraphPlot::updateGraph()
{
    if (!graph()) {
        return;
    }
    double fRes = 0;
    for (int i = 0; i < this->graphCount(); ++i) {
        bool foundRange;
        QCPRange range = this->graph(i)->data()->valueRange(foundRange);
        if (foundRange) {
            double fTmp = abs(range.upper) > abs(range.lower) ? abs(range.upper) : abs(range.lower);

            if (fRes > fTmp) {
                fRes = fTmp;
            }
        }
    }
    this->graph()->rescaleKeyAxis();
    this->yAxis->setRange(-fRes, fRes);

    if (pVerticalLine_->visible()) {
        pVerticalLine_->start->setCoords(fCurPosX_, yAxis->range().lower);
        pVerticalLine_->end->setCoords(fCurPosX_, yAxis->range().upper);
    }

    if (pHorizontalLine_->visible()) {
        pHorizontalLine_->start->setCoords(this->xAxis->range().lower, fCurPosY_);
        pHorizontalLine_->end->setCoords(this->xAxis->range().upper, fCurPosY_);
    }
    this->replot();
}

void MultiGraphPlot::mouseMoveEvent(QMouseEvent *event)
{
    QCustomPlot::mouseMoveEvent(event);

    if (graph()->data()->isEmpty()) {
        return;
    }

    QPointF mousePos = event->localPos();

    fCurPosX_ = this->xAxis->pixelToCoord(mousePos.x());
    fCurPosY_ = this->yAxis->pixelToCoord(mousePos.y());

    if (fCurPosX_ > xAxis->range().lower && fCurPosX_ < xAxis->range().upper && fCurPosY_ > yAxis->range().lower &&
        fCurPosY_ < yAxis->range().upper) {

        vecPt_.clear();
        for (int i = 0; i < this->graphCount(); ++i) {
            for (int j = 0; j < graph(i)->dataCount(); ++j) {

                if (graph(i)->dataMainKey(j) - fCurPosX_ < 0) {

                    nIndexPre_ = j;

                } else {

                    nIndexNext_ = j;
                    break;
                }
            }

            if (nIndexNext_ < nIndexPre_) {
                return;
            }
            int nTmp = (abs(graph(i)->dataMainKey(nIndexPre_) - fCurPosX_) <
                                abs(graph(i)->dataMainKey(nIndexNext_) - fCurPosX_)
                            ? nIndexPre_
                            : nIndexNext_);

            vecPt_.emplace_back(std::pair<double, double>(graph(i)->dataMainKey(nTmp), graph(i)->dataMainValue(nTmp)));
        }

        std::sort(vecPt_.begin(), vecPt_.end(),
                  [&](std::pair<double, double> left, std::pair<double, double> right) -> bool {
                      return ((left.first - fCurPosX_) * (left.first - fCurPosX_) +
                              (left.second - fCurPosY_) * (left.second - fCurPosY_)) <
                             ((right.first - fCurPosX_) * (right.first - fCurPosX_) +
                              (right.second - fCurPosY_) * (right.second - fCurPosY_));
                  });
    };
    if (!vecPt_.empty()) {
        pVerticalLine_->start->setCoords(vecPt_.begin()->first, yAxis->range().lower);
        pVerticalLine_->end->setCoords(vecPt_.begin()->first, yAxis->range().upper);
        pHorizontalLine_->start->setCoords(this->xAxis->range().lower, vecPt_.begin()->second);
        pHorizontalLine_->end->setCoords(this->xAxis->range().upper, vecPt_.begin()->second);
        pVerticalLine_->setVisible(true);
        pHorizontalLine_->setVisible(true);

        QToolTip::showText(
            event->globalPos(),
            QString(QStringLiteral("key: %1  --  value: %2")).arg(vecPt_.begin()->first).arg(vecPt_.begin()->second),
            this, rect());
    } else {
        pVerticalLine_->setVisible(false);
        pHorizontalLine_->setVisible(false);
    }
    this->replot();
}

//----------------------------------------------------------------------------------

void MultiGraphPlot::wheelEvent(QWheelEvent *event)
{
    QCustomPlot::wheelEvent(event);
    if (graph()->data()->isEmpty()) {
        return;
    }
}

void MultiGraphPlot::leaveEvent(QEvent *e)
{
    QCustomPlot::leaveEvent(e);
    if (graph()->data()->isEmpty()) {
        return;
    }
    pVerticalLine_->setVisible(false);
    pHorizontalLine_->setVisible(false);
    this->replot();
}

void MultiGraphPlot::keyPressEvent(QKeyEvent *event)
{
    QCustomPlot::keyPressEvent(event);

    bool bTmp = false;

    switch (event->key()) {
    case Qt::Key_Space: {
        bTmp = true;
        if (this->graph() && !graph()->data()->isEmpty()) {
            this->graph()->rescaleKeyAxis();
            this->graph()->rescaleValueAxis();
            this->replot();
        } else {
            this->xAxis->setRange(-fInitRangeX / 2, fInitRangeX / 2);
            this->yAxis->setRange(-fInitRangeY / 2, fInitRangeY / 2);
        }

        break;
    }
    case Qt::Key_Left: {
        bTmp = true;
        this->xAxis->moveRange(-1.00);

        break;
    }
    case Qt::Key_Up: {
        bTmp = true;
        this->yAxis->moveRange(1.00);

        break;
    }
    case Qt::Key_Right: {
        bTmp = true;
        this->xAxis->moveRange(1.00);

        break;
    }
    case Qt::Key_Down: {
        bTmp = true;
        this->yAxis->moveRange(-1.00);
        break;
    }
    default:
        break;
    }
    if (bTmp) {
        pVerticalLine_->setVisible(false);
        pHorizontalLine_->setVisible(false);
        this->replot();
    }

    return;
}

void MultiGraphPlot::ClearCache()
{
    {
        std::unique_lock<std::mutex> lock(mtx_);
        std::queue<PlotData> zero;
        queueTask_.swap(zero);
    }

    for (int i = 0; i < this->graphCount(); ++i) {
        if (this->graph(i)) {
            std::unique_lock<std::mutex> lock(mtx_);
            this->graph(i)->setData(QVector<double>(), QVector<double>());
        }
    }
    emit this->Draw();
}

void MultiGraphPlot::push(const PlotData &msg)
{
    std::unique_lock<std::mutex> lock(mtx_);
    queueTask_.push(msg);
    cv_.notify_all();
}

void MultiGraphPlot::wait(PlotData &msg)
{
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [&]() -> bool { return (!queueTask_.empty()); });
    msg = queueTask_.front();
    queueTask_.pop();
}

void MultiGraphPlot::threadFunc()
{
    PlotData msg;
    while (!bStop_) {
        this->wait(msg);
        if (msg.type == 0) {
            break;
        }
        if (msg.nIndex < this->graphCount() && msg.nIndex >= 0) {
            std::unique_lock<std::mutex> lock(mtx_);
            this->graph(msg.nIndex)->addData(msg.fValueX, msg.fValueY);
            emit this->Draw();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
};
} // namespace Plot
} // namespace Ui
