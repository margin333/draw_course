#include "simpleplot.h"
namespace Ui {
namespace Plot {
SimplePlot::~SimplePlot() noexcept
{
    ClearCache();
    MyMessage msgEnd;
    msgEnd.type = 0;
    this->push(msgEnd);
    m_thread.join();
}

SimplePlot::SimplePlot(QWidget *parent)
    : QCustomPlot(parent)
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
    this->addGraph();
    this->graph()->setPen(QColor(47, 104, 212));

    this->graph()->setLineStyle(QCPGraph::lsLine);
    this->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));

    connect(this, &SimplePlot::Draw, this, &SimplePlot::updateGraph);

    m_thread = std::thread(&SimplePlot::threadFunc, this);
}

void SimplePlot::SetInitRangeX() { fInitRangeX = xAxis->range().size(); };

void SimplePlot::SetInitRangeY() { fInitRangeY = yAxis->range().size(); };

void SimplePlot::ClearCache()
{
    std::unique_lock<std::mutex> lck(mtx_);
    while (!queueTask_.empty()) {
        queueTask_.pop();
    }

    nIndexNext_ = 0;
    nIndexPre_ = 0;
    vecCacheX_.clear();
    vecCacheY_.clear();
};

void SimplePlot::push(const MyMessage &msg)
{
    std::unique_lock<std::mutex> lck(mtx_);
    queueTask_.push(msg);
    cv_.notify_all();
}

void SimplePlot::wait(MyMessage &msg)
{
    std::unique_lock<std::mutex> lck(mtx_);
    cv_.wait(lck, [&] { return !queueTask_.empty(); });
    msg = queueTask_.front();
    queueTask_.pop();
}

void SimplePlot::threadFunc()
{
    MyMessage msg;
    while (1) {
        this->wait(msg);
        if (msg.type == 0) {
            break;
        } else if (msg.type == 1) {
            std::unique_lock<std::mutex> lck(mtx_);
            vecCacheX_.append(msg.fValueX);
            vecCacheY_.append(msg.fValueY);
            emit this->Draw();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
};

void SimplePlot::updateGraph()
{
    if (!graph()) {
        return;
    }
    std::unique_lock<std::mutex> lck(mtx_);
    this->graph()->setData(vecCacheX_, vecCacheY_);
    this->graph()->rescaleKeyAxis();
    this->graph()->rescaleValueAxis();
    this->replot();
}

void SimplePlot::mouseMoveEvent(QMouseEvent *event)
{
    QCustomPlot::mouseMoveEvent(event);

    if (graph()->data()->isEmpty()) {
        return;
    }

    QPointF mousePos = event->localPos();

    fCurPosX_ = this->xAxis->pixelToCoord(mousePos.x());
    fCurPosY_ = this->yAxis->pixelToCoord(mousePos.y());

    //---------------------------------------------------------------------------------
    if (fCurPosX_ > xAxis->range().lower && fCurPosX_ < xAxis->range().upper && fCurPosY_ > yAxis->range().lower &&
        fCurPosY_ < yAxis->range().upper) {
        for (int i = 0; i < graph()->dataCount(); ++i) {

            if (graph()->dataMainKey(i) - fCurPosX_ < 0) {

                nIndexPre_ = i;

            } else {

                nIndexNext_ = i;
                break;
            }
        }

        if (nIndexNext_ < nIndexPre_) {
            return;
        }
        int nTmp =
            (abs(graph()->dataMainKey(nIndexPre_) - fCurPosX_) < abs(graph()->dataMainKey(nIndexNext_) - fCurPosX_)
                 ? nIndexPre_
                 : nIndexNext_);

        fCurPosX_ = graph()->dataMainKey(nTmp);
        fCurPosY_ = graph()->dataMainValue(nTmp);

        pVerticalLine_->start->setCoords(fCurPosX_, yAxis->range().lower);
        pVerticalLine_->end->setCoords(fCurPosX_, yAxis->range().upper);
        pHorizontalLine_->start->setCoords(this->xAxis->range().lower, fCurPosY_);
        pHorizontalLine_->end->setCoords(this->xAxis->range().upper, fCurPosY_);
        pVerticalLine_->setVisible(true);
        pHorizontalLine_->setVisible(true);

        QToolTip::showText(event->globalPos(),
                           QString(QStringLiteral("key: %1  --  value: %2"))
                               .arg(graph()->dataMainKey(nTmp))
                               .arg(graph()->dataMainValue(nTmp)),
                           this, rect());
        this->replot();
    }
}

void SimplePlot::wheelEvent(QWheelEvent *event)
{
    QCustomPlot::wheelEvent(event);
    if (graph()->data()->isEmpty()) {
        return;
    }

    QPointF mousePos = event->pos();

    fCurPosX_ = this->xAxis->pixelToCoord(mousePos.x());
    fCurPosY_ = this->yAxis->pixelToCoord(mousePos.y());

    //---------------------------------------------------------------------------------
    if (fCurPosX_ > xAxis->range().lower && fCurPosX_ < xAxis->range().upper && fCurPosY_ > yAxis->range().lower &&
        fCurPosY_ < yAxis->range().upper) {
        for (int i = 0; i < graph()->dataCount(); ++i) {

            if (graph()->dataMainKey(i) - fCurPosX_ < 0) {

                nIndexPre_ = i;

            } else {

                nIndexNext_ = i;
                break;
            }
        }

        if (nIndexNext_ < nIndexPre_) {
            return;
        }
        int nTmp =
            (abs(graph()->dataMainKey(nIndexPre_) - fCurPosX_) < abs(graph()->dataMainKey(nIndexNext_) - fCurPosX_)
                 ? nIndexPre_
                 : nIndexNext_);

        fCurPosX_ = graph()->dataMainKey(nTmp);
        fCurPosY_ = graph()->dataMainValue(nTmp);

        pVerticalLine_->start->setCoords(fCurPosX_, yAxis->range().lower);
        pVerticalLine_->end->setCoords(fCurPosX_, yAxis->range().upper);
        pHorizontalLine_->start->setCoords(this->xAxis->range().lower, fCurPosY_);
        pHorizontalLine_->end->setCoords(this->xAxis->range().upper, fCurPosY_);
        pVerticalLine_->setVisible(true);
        pHorizontalLine_->setVisible(true);
        QToolTip::showText(
            event->globalPos(),
            QString(QStringLiteral("X: %1 -- Y: %2")).arg(graph()->dataMainKey(nTmp)).arg(graph()->dataMainValue(nTmp)),
            this, rect());
        this->replot();
    }
}

void SimplePlot::leaveEvent(QEvent *e)
{
    QCustomPlot::leaveEvent(e);
    if (graph()->data()->isEmpty()) {
        return;
    }
    pVerticalLine_->setVisible(false);
    pHorizontalLine_->setVisible(false);
    this->replot();
}

void SimplePlot::keyPressEvent(QKeyEvent *event)
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
} // namespace Plot
} // namespace Ui
