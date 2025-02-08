#include "bne_basewidget.h"

namespace Ui {
namespace BaseWidget {
BNEBaseWidget::BNEBaseWidget(QWidget *parent)
    : QWidget(parent)
{
}

BNEBaseWidget::~BNEBaseWidget() { }
BNEStatus BNEBaseWidget::CreateBNEQLabel(QLabel *&pLabel, const QString &strText, const QRect &rectPos,
                                         Qt::Alignment nAlign, int nFontSize, int nWeightBold)
{
    if (pLabel == nullptr) {
        pLabel = new QLabel(strText, this);
        pLabel->move(rectPos.x(), rectPos.y());
        pLabel->resize(rectPos.width(), rectPos.height());
        QFont fontLabel = pLabel->font();
        fontLabel.setPixelSize(nFontSize);
        fontLabel.setWeight(nWeightBold);
        pLabel->setFont(fontLabel);

        pLabel->setAlignment(nAlign);
        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

BNEStatus BNEBaseWidget::CreateBNESmplEdit(Ui::SimpleEdit::SimpleEdit *&pEdit, const QRect &rectPos, bool bSetReadOnly,
                                           int nFontSize, const QString &strText)
{
    if (pEdit == nullptr) {
        pEdit = new Ui::SimpleEdit::SimpleEdit(this);
        pEdit->move(rectPos.x(), rectPos.y());
        pEdit->resize(rectPos.width(), rectPos.height());
        pEdit->setReadOnly(bSetReadOnly);
        pEdit->setFocusPolicy(Qt::ClickFocus);
        pEdit->setAlignment(Qt::AlignCenter);
        QFont fontEdit = pEdit->font();
        fontEdit.setPixelSize(nFontSize);
        pEdit->setFont(fontEdit);

        if (!strText.isEmpty()) {
            pEdit->setText(strText);
        }
        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

BNEStatus BNEBaseWidget::CreateBNEComEdit(Ui::SmplEdit::CompositeEdit *&pEdit, const QRect &rectPos, bool bSetReadOnly,
                                          int nFontSize, const QString &strText)
{
    if (pEdit == nullptr) {
        pEdit = new Ui::SmplEdit::CompositeEdit(this);
        pEdit->move(rectPos.x(), rectPos.y());
        pEdit->resize(rectPos.width(), rectPos.height());
        pEdit->setReadOnly(bSetReadOnly);
        pEdit->setFocusPolicy(Qt::ClickFocus);
        pEdit->setAlignment(Qt::AlignCenter);
        QFont fontEdit = pEdit->font();
        fontEdit.setPixelSize(nFontSize);
        pEdit->setFont(fontEdit);

        if (!strText.isEmpty()) {
            pEdit->setText(strText);
        }
        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

BNEStatus BNEBaseWidget::CreateBNEScrollEdit(Ui::ScrollEdit::SmplScrollEdit *&pEdit, const QRect &rectPos,
                                             bool bSetReadOnly, int nFontSize, const QString &strText)
{
    if (pEdit == nullptr) {
        pEdit = new Ui::ScrollEdit::SmplScrollEdit(this);
        pEdit->move(rectPos.x(), rectPos.y());
        pEdit->resize(rectPos.width(), rectPos.height());
        pEdit->setReadOnly(bSetReadOnly);
        pEdit->setAlignment(Qt::AlignCenter);
        QFont fontEdit = pEdit->font();
        fontEdit.setPixelSize(nFontSize);
        pEdit->setFont(fontEdit);

        if (!strText.isEmpty()) {
            pEdit->setText(strText);
        }

        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

/**************************************************************************
 * 名称:  CreateBNEButton
 * 功能:  创建大按钮
 * 输入参数:  unique_ptr<Ui::OctagonButton::OctagonButton> &pButton大按钮的指针,
 *const QString &strText按钮上的显示文字, QFont font字体风格, int nX横坐标, int
 *nY纵坐标, int nWidth宽度, int nHeight高度, std::function<void()> func点击事件
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:    jiwei
 * 创建时间:  2024.03.08
 ***************************************************************************/
BNEStatus BNEBaseWidget::CreateBNEButton(Ui::RoundRectBtn::RoundRectButton *&pButton, const QString &strText,
                                         const QRect &rectPos, std::function<void()> func, int nFontSize, bool bBold)
{
    if (pButton == nullptr) {
        pButton = new Ui::RoundRectBtn::RoundRectButton(this);
        pButton->setText(strText);
        pButton->move(rectPos.x(), rectPos.y());
        pButton->resize(rectPos.width(), rectPos.height());
        QFont fontBtn = pButton->font();
        fontBtn.setPixelSize(nFontSize);
        if (bBold) {
            fontBtn.setWeight(QFont::Bold);
        }

        pButton->setFont(fontBtn);
        connect(pButton, &Ui::RoundRectBtn::RoundRectButton::clicked, this, func);
        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

BNEStatus BNEBaseWidget::CreateBNEProgressBar(Ui::RoundRectProgress::RoundRectProgressBar *&pProgressbar,
                                              const QRect &rectPos)
{
    if (pProgressbar == nullptr) {
        pProgressbar = new Ui::RoundRectProgress::RoundRectProgressBar(this);
        pProgressbar->move(rectPos.x(), rectPos.y());
        pProgressbar->resize(rectPos.width(), rectPos.height());
        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

BNEStatus BNEBaseWidget::CreateBNERadioButton(Ui::SimpleRadio::SimpleRadioButton *&pRbtn, const QString &strText,
                                              const QRect &rectPos, int nFontSize)
{
    if (pRbtn == nullptr) {
        pRbtn = new Ui::SimpleRadio::SimpleRadioButton(this);
        pRbtn->setText(strText);
        pRbtn->move(rectPos.x(), rectPos.y());
        pRbtn->resize(rectPos.width(), rectPos.height());
        QFont fontRadio = pRbtn->font();
        fontRadio.setPixelSize(nFontSize);
        pRbtn->setFont(fontRadio);

        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

BNEStatus BNEBaseWidget::CreateBNETableWidget(Ui::SortTable::SimpleTableWidget *&pTbW, const QRect &rectPos, int nRow,
                                              int nColumn)
{
    if (pTbW == nullptr) {
        pTbW = new Ui::SortTable::SimpleTableWidget(this);
        pTbW->move(rectPos.x(), rectPos.y());
        pTbW->resize(rectPos.width(), rectPos.height());
        pTbW->setRowCount(nRow);
        pTbW->setColumnCount(nColumn);

        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}
/**************************************************************************
 * 名称: CurrentFunction
 * 功能:
 * 输入参数:
 * 输出参数: NULL
 * 返回值:
 * 创建者:   ZhouWeize
 * 创建时间: 2024-05-21
 ***************************************************************************/
BNEStatus BNEBaseWidget::CreateBNECheckBox(Ui::EzCheckBox::EzCheckBox *&pChx, const QString &strText,
                                           const QRect &rectPos, int nFontSize)
{
    if (pChx == nullptr) {
        pChx = new Ui::EzCheckBox::EzCheckBox(this);
        pChx->setText(strText);
        pChx->move(rectPos.x(), rectPos.y());
        pChx->resize(rectPos.width(), rectPos.height());
        QFont fontChx = pChx->font();
        fontChx.setPixelSize(nFontSize);
        pChx->setFont(fontChx);

        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}
} // namespace BaseWidget
} // namespace Ui
// namespace Ui
