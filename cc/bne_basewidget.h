#ifndef BNE_BASEWIDGET_H
#define BNE_BASEWIDGET_H

#include <functional>
#include <QLabel>
#include <QMessageBox>
#include <QButtonGroup>
#include <QDebug>
#include <QTimer>
#include <QApplication>

#include "bne_app_error.h"
#include "bne_app_enum.h"

#include "../../common/model/custom/composite_edit.h"
#include "../../common/model/custom/roundrect_button.h"
#include "../../common/model/custom/roundrect_progressbar.h"
#include "../../common/model/custom/simple_radiobutton.h"
#include "../../common/model/custom/smpl_scrolledit.h"
#include "../../common/model/custom/sort_tablewidget.h"
#include "../../common/model/custom/simple_edit.h"
#include "../../common/model/custom/ez_checkbox.h"
#include "../../common/model/custom/simple_tabwidget.h"

namespace Ui {
namespace BaseWidget {

class BNEBaseWidget : public QWidget {
    Q_OBJECT
public:
    BNEBaseWidget(QWidget *parent = nullptr);
    virtual ~BNEBaseWidget() override;

protected:
    BNEStatus CreateBNEQLabel(QLabel *&pLabel, const QString &strText, const QRect &rectPos, Qt::Alignment nAlign,
                              int nFontSize, int nWeightBold = QFont::Medium);
    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNESmplEdit(Ui::SimpleEdit::SimpleEdit *&pEdit, const QRect &rectPos, bool bSetReadOnly,
                                int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL, const QString &strText = nullptr);

    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNEComEdit(Ui::SmplEdit::CompositeEdit *&pEdit, const QRect &rectPos, bool bSetReadOnly,
                               int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL,
                               const QString &strText = nullptr); // 创建一个复合编辑框
    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNEScrollEdit(Ui::ScrollEdit::SmplScrollEdit *&pEdit, const QRect &rectPos, bool bSetReadOnly,
                                  int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL,
                                  const QString &strText = nullptr); // 创建一个滚动编辑框

    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNEButton(Ui::RoundRectBtn::RoundRectButton *&pButton, const QString &strText, const QRect &rectPos,
                              std::function<void()> func, int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL,
                              bool bBold = true); // 创建一个自定义按钮控件
    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNEProgressBar(Ui::RoundRectProgress::RoundRectProgressBar *&pProgressbar, const QRect &rectPos);

    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNERadioButton(Ui::SimpleRadio::SimpleRadioButton *&pRbtn, const QString &strText,
                                   const QRect &rectPos,
                                   int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL); // 创建一个自定义复选框控件

    //------------------------------------------------------------------------------------------------------------------
    BNEStatus CreateBNETableWidget(Ui::SortTable::SimpleTableWidget *&pTbW, const QRect &rectPos, int nRow,
                                   int nColumn);

    BNEStatus CreateBNECheckBox(Ui::EzCheckBox::EzCheckBox *&pTbW, const QString &strText, const QRect &rectPos,
                                int nFontSize = AppEnum::BNE_APP_FONTSIZE_SMALL);

signals:
};
} // namespace BaseWidget
} // namespace Ui
// namespace Ui
#endif // BNE_BASEWIDGET_H
