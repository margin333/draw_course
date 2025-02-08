#include "bne_widget_factory.h"

namespace Ui {
namespace WidgetFactory {
BNEStatus BNEWidgetFactory::CreateBNEComboBox(QComboBox *&pComboBox, QWidget *parent, const QRect &rectPos,
                                              int nFontSize, int nWeightBold)
{
    if (pComboBox == nullptr) {
        pComboBox = new Ui::BNECombo::BNEComboBox(parent);
        pComboBox->move(rectPos.x(), rectPos.y());
        pComboBox->resize(rectPos.width(), rectPos.height());
        QFont fontLabel = pComboBox->font();
        fontLabel.setPixelSize(nFontSize);
        fontLabel.setWeight(nWeightBold);
        pComboBox->setFont(fontLabel);
        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

BNEStatus BNEWidgetFactory::CreateBNEComboBox(Ui::BNECombo::BNEComboBox *&pComboBox, QWidget *parent,
                                              const QRect &rectPos, int nFontSize, int nWeightBold)
{
    if (pComboBox == nullptr) {
        pComboBox = new Ui::BNECombo::BNEComboBox(parent);
        pComboBox->move(rectPos.x(), rectPos.y());
        pComboBox->resize(rectPos.width(), rectPos.height());
        QFont fontLabel = pComboBox->font();
        fontLabel.setPixelSize(nFontSize);
        fontLabel.setWeight(nWeightBold);
        pComboBox->setFont(fontLabel);
        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

BNEStatus BNEWidgetFactory::CreateBNEQLabel(QLabel *&pLabel, const QString &strText, QWidget *parent,
                                            const QRect &rectPos, Qt::Alignment nAlign, int nFontSize, int nWeightBold)
{
    if (pLabel == nullptr) {
        pLabel = new QLabel(strText, parent);
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



//------------------------------------------------------------------------------------------------------------------
BNEStatus BNEWidgetFactory::CreateBNESmplEdit(Ui::SimpleEdit::SimpleEdit *&pEdit, QWidget *parent, const QRect &rectPos,
                                              bool bSetReadOnly, int nFontSize, const QString &strText)
{
    if (pEdit == nullptr) {
        pEdit = new Ui::SimpleEdit::SimpleEdit(parent);
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

BNEStatus BNEWidgetFactory::CreateBNESmplSizeEdit(SimpleEdit::SimpleEdit *&pEdit, QWidget *parent, const QRect &rectPos, bool bSetReadOnly, int nFontSize, const QString &strText)
{
    if (pEdit == nullptr) {
        pEdit = new Ui::SimpleEdit::SimpleEdit(parent);
        pEdit->move(rectPos.x(), rectPos.y());
        pEdit->resize(rectPos.width(), rectPos.height());
        pEdit->setReadOnly(bSetReadOnly);
        pEdit->setFocusPolicy(Qt::ClickFocus);
        pEdit->setAlignment(Qt::AlignCenter);

        // 设置字体大小
        QFont fontEdit = pEdit->font();
        fontEdit.setPixelSize(nFontSize);
        pEdit->setFont(fontEdit);

        // 设置正整数输入的验证器
        QRegExp regex("^[1-9]\\d*$"); // 正整数正则表达式
        QRegExpValidator *validator = new QRegExpValidator(regex, pEdit);
        pEdit->setValidator(validator);

        // 取消之前可能存在的 editingFinished 连接
        QObject::disconnect(pEdit, &QLineEdit::editingFinished, nullptr, nullptr);

        // 当输入结束（失去焦点）时验证并更新文本
        QObject::connect(pEdit, &QLineEdit::editingFinished, [=]() {
            QString text = pEdit->text();
            if (!regex.exactMatch(text)) {
                pEdit->clear(); // 如果不符合正整数，清空输入
                QMessageBox::warning(parent, "输入错误", "请输入正整数");
            }
        });

        // 设置文本
        if (!strText.isEmpty()) {
            pEdit->setText(strText);
        }
        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

//------------------------------------------------------------------------------------------------------------------
BNEStatus BNEWidgetFactory::CreateBNEComEdit(Ui::SmplEdit::CompositeEdit *&pEdit, QWidget *parent, const QRect &rectPos,
                                             bool bSetReadOnly, int nFontSize, const QString &strText)
{
    if (pEdit == nullptr) {
        pEdit = new Ui::SmplEdit::CompositeEdit(parent);
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
//------------------------------------------------------------------------------------------------------------------
BNEStatus BNEWidgetFactory::CreateBNEScrollEdit(Ui::ScrollEdit::SmplScrollEdit *&pEdit, QWidget *parent,
                                                const QRect &rectPos, bool bSetReadOnly, int nFontSize,
                                                const QString &strText)
{
    if (pEdit == nullptr) {
        pEdit = new Ui::ScrollEdit::SmplScrollEdit(parent);
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
//------------------------------------------------------------------------------------------------------------------
BNEStatus BNEWidgetFactory::CreateBNEButton(Ui::RoundRectBtn::RoundRectButton *&pButton, const QString &strText,
                                            QWidget *parent, const QRect &rectPos, std::function<void()> func,
                                            int nFontSize, bool bBold)
{
    if (pButton == nullptr) {
        pButton = new Ui::RoundRectBtn::RoundRectButton(parent);
        pButton->setText(strText);
        pButton->move(rectPos.x(), rectPos.y());
        pButton->resize(rectPos.width(), rectPos.height());
        QFont fontBtn = pButton->font();
        fontBtn.setPixelSize(nFontSize);
        if (bBold) {
            fontBtn.setWeight(QFont::Bold);
        }

        pButton->setFont(fontBtn);
        QObject::connect(pButton, &Ui::RoundRectBtn::RoundRectButton::clicked, parent, func);
        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}
//------------------------------------------------------------------------------------------------------------------
BNEStatus BNEWidgetFactory::CreateBNEProgressBar(Ui::RoundRectProgress::RoundRectProgressBar *&pProgressbar,
                                                 QWidget *parent, const QRect &rectPos)
{
    if (pProgressbar == nullptr) {
        pProgressbar = new Ui::RoundRectProgress::RoundRectProgressBar(parent);
        pProgressbar->move(rectPos.x(), rectPos.y());
        pProgressbar->resize(rectPos.width(), rectPos.height());
        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

//------------------------------------------------------------------------------------------------------------------
BNEStatus BNEWidgetFactory::CreateBNERadioButton(Ui::SimpleRadio::SimpleRadioButton *&pRbtn, const QString &strText,
                                                 QWidget *parent, const QRect &rectPos, int nFontSize)
{
    if (pRbtn == nullptr) {
        pRbtn = new Ui::SimpleRadio::SimpleRadioButton(parent);
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

//------------------------------------------------------------------------------------------------------------------
BNEStatus BNEWidgetFactory::BNEWidgetFactory::CreateBNETableWidget(Ui::SortTable::SimpleTableWidget *&pTbW,
                                                                   QWidget *parent, const QRect &rectPos, int nRow,
                                                                   int nColumn)
{
    if (pTbW == nullptr) {
        pTbW = new Ui::SortTable::SimpleTableWidget(parent);
        pTbW->move(rectPos.x(), rectPos.y());
        pTbW->resize(rectPos.width(), rectPos.height());
        pTbW->setRowCount(nRow);
        pTbW->setColumnCount(nColumn);

        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}
BNEStatus BNEWidgetFactory::CreateBNETableWidget(Ui::SortTable::SimpleTableWidget *&pTbW, QWidget *parent,
                                                 const QRect &rectPos)
{
    if (pTbW == nullptr) {
        pTbW = new Ui::SortTable::SimpleTableWidget(parent);
        pTbW->move(rectPos.x(), rectPos.y());
        pTbW->resize(rectPos.width(), rectPos.height());

        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

BNEStatus BNEWidgetFactory::CreateBNETableWidget(Ui::PathTable::PathTableWidget *&pTbW, Ui::PathTable::PathMode mode,
                                                 QWidget *parent, const QRect &rectPos)
{
    if (pTbW == nullptr) {
        pTbW = new Ui::PathTable::PathTableWidget(mode, parent);
        pTbW->move(rectPos.x(), rectPos.y());
        pTbW->resize(rectPos.width(), rectPos.height());

        return BNE_APP_SUCCESS;
    } else {
        return BNE_APP_FIA_CTRL_ERROR;
    }
}

BNEStatus BNEWidgetFactory::CreateBNECheckBox(Ui::EzCheckBox::EzCheckBox *&pChx, const QString &strText,
                                              QWidget *parent, const QRect &rectPos, int nFontSize)
{
    if (pChx == nullptr) {
        pChx = new Ui::EzCheckBox::EzCheckBox(parent);
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
} // namespace WidgetFactory
} // namespace Ui
