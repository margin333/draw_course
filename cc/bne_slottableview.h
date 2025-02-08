#ifndef BNESLOTTABLEVIEW_H
#define BNESLOTTABLEVIEW_H

#include "qcoreevent.h"
#include <QTableView>
#include <QHeaderView>
#include <QPainter>
#include <QStandardItemModel>
#include <QPainter>
#include <QProxyStyle>
#include <QTimer>
#include <QDebug>
#include <cmath>

namespace Ui {
namespace SlotTable {
class SlotTableViewStyle : public QProxyStyle {
public:
    SlotTableViewStyle();

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

class BNESlotTableView : public QTableView {
    Q_OBJECT
public:
    BNESlotTableView(QWidget *parent);

    void SetCSTInfo(int nReserveRow, QList<int> &listSlotItem);
    void SetCSTListItem(int nIndex, int nValue);
    inline void SetCheckState(bool bCheckState) { bChecked_ = bCheckState; };
    inline int GetCSTTime() const { return nTime_; };
    inline int IsChecked() const { return bChecked_; };

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void AutoAdjustTableItemHeight();
    QPainterPath GetThirdEquTrgPath(QPointF pointCenter, double fLength, double fRadius);

private:
    bool bChecked_;
    bool bDraw_;
    bool nDrawWarning_;
    int nTime_;
    QTimer *pTimerTransfer_;

    QList<QStandardItem *> listItem_;
    QStandardItemModel *pModel_ = nullptr;
    SlotTableViewStyle style_;
};
} // namespace SlotTable
} // namespace Ui
#endif // BNESLOTTABLEVIEW_H
