#ifndef BNEWIDGETFACTORY_H
#define BNEWIDGETFACTORY_H

#include <QObject>
#include <functional>
#include <QLabel>
#include <QMessageBox>
#include <QButtonGroup>
#include <QTimer>
#include <QApplication>

#include "bne_app_error.h"
#include "bne_app_enum.h"

#include "bne_combobox.h"
#include "bne_customplot/multigraphplot.h"
#include "bne_customplot/simpleplot.h"
#include "composite_edit.h"
#include "roundrect_button.h"
#include "roundrect_progressbar.h"
#include "simple_radiobutton.h"
#include "smpl_scrolledit.h"
#include "sort_tablewidget.h"
#include "simple_edit.h"
#include "ez_checkbox.h"
#include "bne_pathtable.h"
#include "drawimage_widget.h"
#include "../../common/model/custom/bne_msgbox.h"


namespace Ui {
namespace WidgetFactory {
class BNEWidgetFactory {
public:
    static BNEWidgetFactory &GetIntance()
    {
        static BNEWidgetFactory sinstance;
        return sinstance;
    }
    virtual ~BNEWidgetFactory() { }

private:
    BNEWidgetFactory() = default;
    //    BNEWidgetFactory(const BNEWidgetFactory &) { }
    //    BNEWidgetFactory &operator=(const BNEWidgetFactory &) { return *this; }
    BNEWidgetFactory(const BNEWidgetFactory &) = delete;
    BNEWidgetFactory &operator=(const BNEWidgetFactory &) = delete;

public:
    BNEStatus CreateBNEComboBox(QComboBox *&pComboBox, QWidget *parent, const QRect &rectPos, int nFontSize,
                                int nWeightBold = QFont::Medium);

    BNEStatus CreateBNEComboBox(Ui::BNECombo::BNEComboBox *&pComboBox, QWidget *parent, const QRect &rectPos,
                                int nFontSize, int nWeightBold = QFont::Medium);

    BNEStatus CreateBNEQLabel(QLabel *&pLabel, const QString &strText, QWidget *parent, const QRect &rectPos,
                              Qt::Alignment nAlign, int nFontSize, int nWeightBold = QFont::Medium);


    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNESmplEdit(Ui::SimpleEdit::SimpleEdit *&pEdit, QWidget *parent, const QRect &rectPos,
                                bool bSetReadOnly, int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL,
                                const QString &strText = nullptr);

    BNEStatus CreateBNESmplSizeEdit(Ui::SimpleEdit::SimpleEdit *&pEdit, QWidget *parent, const QRect &rectPos,
                                bool bSetReadOnly, int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL,
                                const QString &strText = nullptr);


    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNEComEdit(Ui::SmplEdit::CompositeEdit *&pEdit, QWidget *parent, const QRect &rectPos,
                               bool bSetReadOnly, int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL,
                               const QString &strText = nullptr); // 创建一个复合编辑框
    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNEScrollEdit(Ui::ScrollEdit::SmplScrollEdit *&pEdit, QWidget *parent, const QRect &rectPos,
                                  bool bSetReadOnly, int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL,
                                  const QString &strText = nullptr); // 创建一个滚动编辑框

    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNEButton(Ui::RoundRectBtn::RoundRectButton *&pButton, const QString &strText, QWidget *parent,
                              const QRect &rectPos, std::function<void()> func,
                              int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL,
                              bool bBold = true); // 创建一个自定义按钮控件
    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNEProgressBar(Ui::RoundRectProgress::RoundRectProgressBar *&pProgressbar, QWidget *parent,
                                   const QRect &rectPos);

    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNERadioButton(Ui::SimpleRadio::SimpleRadioButton *&pRbtn, const QString &strText, QWidget *parent,
                                   const QRect &rectPos,
                                   int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL); // 创建一个自定义复选框控件

    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNETableWidget(Ui::SortTable::SimpleTableWidget *&pTbW, QWidget *parent, const QRect &rectPos,
                                   int nRow, int nColumn);

    BNEStatus CreateBNETableWidget(Ui::SortTable::SimpleTableWidget *&pTbW, QWidget *parent, const QRect &rectPos);

    BNEStatus CreateBNETableWidget(Ui::PathTable::PathTableWidget *&pTbW, Ui::PathTable::PathMode mode, QWidget *parent,
                                   const QRect &rectPos);

    BNEStatus CreateBNECheckBox(Ui::EzCheckBox::EzCheckBox *&pTbW, const QString &strText, QWidget *parent,
                                const QRect &rectPos, int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL);

signals:
};
} // namespace WidgetFactory
} // namespace Ui
#endif // BNEWIDGETFACTORY_H
