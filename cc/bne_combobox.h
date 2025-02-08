#ifndef BNECOMBOBOX_H
#define BNECOMBOBOX_H

#include <QComboBox>
#include <QProxyStyle>
#include <QListView>
#include <QScrollBar>
#include <QLineEdit>
#include <QDebug>
#include <QPainter>
#include "style_instance.h"

//--------ComboListViewStyle-------------------------------------------------------
namespace Ui {
namespace BNECombo {

class ComboListViewStyle : public QProxyStyle {
public:
    ComboListViewStyle();

    ~ComboListViewStyle();

    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const override;

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};

//--------ComboScrollbarStyle-------------------------------------------------------
class ComboScrollbarStyle : public QProxyStyle {
public:
    ComboScrollbarStyle();

    ~ComboScrollbarStyle(){};

    void drawComplexControl(ComplexControl element, const QStyleOptionComplex *option, QPainter *painter,
                            const QWidget *widget = nullptr) const override;

    QRect subControlRect(ComplexControl element, const QStyleOptionComplex *option, SubControl whichSubControl,
                         const QWidget *widget = nullptr) const override;

private:
    QImage imgScrollSliderOver_;
    QImage imgScrollSlider_;
    QImage imgScrollSubLine_;
    QImage imgScrollAddLine_;
};

//--------CustomComboListView-------------------------------------------------------

class CustomComboListView : public QListView {
public:
    CustomComboListView(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    ComboListViewStyle styleListview_;
    ComboScrollbarStyle styleScrollbar_;
};
//-------------------------------------------------------------------------------------
class CustomComboBoxStyle : public QProxyStyle {
public:
    CustomComboBoxStyle();
    void drawComplexControl(ComplexControl which,
                            const QStyleOptionComplex *option, // 自定义控件风格中绘制复杂的控件元素
                            QPainter *painter, const QWidget *widget = nullptr) const override;
    QRect subControlRect(ComplexControl whichControl, const QStyleOptionComplex *option,
                         SubControl whichSubControl, // 获取不同元素的区域
                         const QWidget *widget = nullptr) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, // 绘制基础元素控件
                       const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, // 绘制控制元素控件
                     const QWidget *widget = nullptr) const override;
};

class BNEComboBox : public QComboBox {
    Q_OBJECT
public:
    BNEComboBox(QWidget *parent);
    void showPopup() override;

    template <typename T> bool AddPairList(const QList<std::pair<QString, T>> &list)
    {
        if (list.empty()) {
            return false;
        }
        this->clear();
        for (const std::pair<QString, T> &pair : list) {
            this->addItem(pair.first, QVariant(pair.second));
        }
        return true;
    }

    int GetCurrentIndexUserData();
    void SetCurrentIndexByUserData(int nValue);

    void SetAlignment(Qt::Alignment align);
    Qt::Alignment GetAlignment() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Qt::Alignment nFlagAlign_;
};

} // namespace BNECombo
} // namespace Ui
#endif // BNECOMBOBOX_H
