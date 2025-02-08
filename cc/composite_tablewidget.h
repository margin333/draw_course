#ifndef COMBOTABLEWIDGET_H
#define COMBOTABLEWIDGET_H
#include "bne_combobox.h"
#include <QApplication>

#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemDelegate>
namespace Ui {
namespace BNECombo {
enum SPORT { Football, Basketball, Volleyball, Badminton };

class SubComboBoxStyle : public QProxyStyle {
public:
    SubComboBoxStyle();
    void drawComplexControl(ComplexControl which, const QStyleOptionComplex *option, QPainter *painter,
                            const QWidget *widget = nullptr) const override;
    QRect subControlRect(ComplexControl whichControl, const QStyleOptionComplex *option, SubControl whichSubControl,
                         const QWidget *widget = nullptr) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                       const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget = nullptr) const override;
};

// 自定义的多选ComboBox类
class SubComboBox : public QComboBox {
    Q_OBJECT
public:
    SubComboBox(QWidget *parent = nullptr);

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

    int GetCurrentIndexUserData()
    {
        if (this->currentData().type() == QVariant::Int)
            return this->currentData().toInt();
        else {
            return -1;
        }
    };

    void SetCurrentIndexByUserData(int nValue)
    {
        int nRes = this->findData(nValue);
        if (nRes >= 0 && nRes < this->count()) {
            this->setCurrentIndex(nRes);
        }
        return;
    };

    void SetAlignments(Qt::Alignment align);
    Qt::Alignment GetAlignment() const;

    bool isViewShowing() const;
    QString currentText() const;

    virtual void showPopup() override;
    virtual void hidePopup() override;

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void popUp();

private:
    bool bShowView_;
    Qt::Alignment nFlagAlign_;
    SubComboBoxStyle style_;
    ComboListViewStyle styleList_;
    ComboScrollbarStyle styleScroll_;
};

class ListItemDelegate : public QItemDelegate {
public:
    ListItemDelegate(QObject *parent = nullptr)
        : QItemDelegate(parent)
    {
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(219, 234, 255));
        QRect rect = option.rect;
        painter->drawRect(rect);
        QItemDelegate::paint(painter, option, index);
    };
};

class ComboBoxDelegate : public QItemDelegate {
public:
    ComboBoxDelegate(QObject *parent = nullptr);

    ComboBoxDelegate(const QList<std::pair<QString, int>> &list, QObject *parent = nullptr);

    void SetComboBoxList(const QList<std::pair<QString, int>> &list) { listComboBox_ = list; };

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

private:
    QList<std::pair<QString, int>> listComboBox_;
};

class CompositeTableWidget : public QTableWidget {
    Q_OBJECT
public:
    CompositeTableWidget();
};
} // namespace BNECombo
} // namespace Ui
#endif // COMBOTABLEWIDGET_H
