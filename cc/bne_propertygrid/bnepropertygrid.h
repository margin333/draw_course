#ifndef BNEPROPERTYGRID_H
#define BNEPROPERTYGRID_H

#include <QScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QProxyStyle>
#include <QDebug>
#include <QStyleOption>
#include <QEvent>
#include <QBitmap>
#include "qtgroupboxpropertybrowser.h"
#include "../../common/model/custom/composite_edit.h"
#include "../../common/model/custom/roundrect_button.h"
#include "../../common/model/custom/roundrect_progressbar.h"
#include "../../common/model/custom/simple_radiobutton.h"
#include "../../common/model/custom/smpl_scrolledit.h"
#include "../../common/model/custom/sort_tablewidget.h"
#include "../../common/model/custom/simple_edit.h"
#include "../../common/model/custom/bne_combobox.h"

class PropertyGridStyle : public QProxyStyle {
public:
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = 0) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = 0) const override;

    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;
};

class BNEPropertyGrid : public QScrollArea {
public:
    BNEPropertyGrid(QWidget *parent = nullptr);
    virtual ~BNEPropertyGrid();

    inline QtGroupBoxPropertyBrowser *GetPropertyBrowser() const { return pBrowser_; };

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    bool viewportEvent(QEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void AutoSetMask();

private:
    QtGroupBoxPropertyBrowser *pBrowser_;
    PropertyGridStyle style_;
};

#endif // BNEPROPERTYGRID_H
