#ifndef BNERMTABLEVIEW_H
#define BNERMTABLEVIEW_H

#include <QTableView>

#include "qcoreevent.h"
#include <QTableView>
#include <QToolTip>
#include <QHeaderView>
#include <QPainter>
#include <QStandardItemModel>
#include <QPainter>
#include <QProxyStyle>
#include <QTimer>
#include <QDebug>
#include <cmath>

namespace Ui {
namespace RMTable {
class RMTableViewStyle : public QProxyStyle {
public:
    RMTableViewStyle();

protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;

private:
    QImage imgBaoPian_;
    QImage imgXiePian_;
    QImage imgHouPian_;
    QImage imgDuoPian_;
    QImage imgWanQu_;
};

class BNERMTableView : public QTableView {
    Q_OBJECT
public:
    BNERMTableView(QWidget *parent);

    void SetCSTInfo(int nReserveRow, QList<std::pair<int, QString>> &listSlotItem);
    void SetCSTListItem(int nIndex, int nValue, QString str);
    inline void SetCheckState(bool bCheckState) { bChecked_ = bCheckState; };
    inline int GetCSTTime() const { return nTime_; };
    inline int IsChecked() const { return bChecked_; };

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void showToolTip(const QModelIndex &index);
    void AutoAdjustTableItemHeight();
    QPainterPath getThirdEquTrgPath(QPointF pointCenter, double fLength, double fRadius)
    {
        double fX = pointCenter.x();
        double fY = pointCenter.y();

        double fLengthHalfSide = fLength / 2;
        double fDiameter = 2 * fRadius;

        double PAI = 3.1415926;
        double fRadio = tan(PAI * 60 / 180);

        QPainterPath path1;
        path1.moveTo(0, 0);
        path1.lineTo(0, fLengthHalfSide / fRadio);
        path1.lineTo(fLengthHalfSide - fRadius * fRadio, fLengthHalfSide / fRadio);
        path1.arcTo(fLengthHalfSide - fRadius - fRadius * fRadio, fLengthHalfSide / fRadio - fDiameter, fDiameter,
                    fDiameter, 270, 60);
        path1.closeSubpath();
        path1.translate(fX, fY);

        QPainterPath path2;
        path2.moveTo(0, 0);
        path2.lineTo(0, fLengthHalfSide / fRadio);
        path2.lineTo((fLengthHalfSide - fRadius * fRadio) * -1, fLengthHalfSide / fRadio);
        path2.arcTo(-1 * (fLengthHalfSide - fRadius - fRadius * fRadio) - 2 * fRadius,
                    fLengthHalfSide / fRadio - fDiameter, fDiameter, fDiameter, 270, -60);

        path2.closeSubpath();
        path2.translate(fX, fY);

        QPainterPath pathRes = path1.united(path2);
        return pathRes;
    }

private:
    bool bChecked_;
    bool bDraw_;
    bool nDrawWarning_;
    int nTime_;
    QTimer *pTimerTransfer_;

    QList<QStandardItem *> listItem_;
    QStandardItemModel *pModel_ = nullptr;
    RMTableViewStyle style_;
};
} // namespace RMTable
} // namespace Ui

#endif // BNERMTABLEVIEW_H
