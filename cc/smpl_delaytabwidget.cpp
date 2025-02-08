#include "smpl_delaytabwidget.h"

namespace Ui {
namespace DelayTab {

DelayTabWidget::DelayTabWidget(QWidget *parent)
    : Ui::SmplTabWdt::SimpleTabWidget{parent}
{
    connect(this, &DelayTabWidget::currentChanged, this, [&](int index) -> bool {
        std::map<int, std::function<bool()>>::iterator it = mapInitFunc_.find(index);
        if (it == mapInitFunc_.end())
            return false;
        if (it->second()) {
            mapInitFunc_.erase(it);
            return true;
        } else {
            return false;
        }
    });
}

int DelayTabWidget::addTab(QWidget *child, const QString &label, std::function<bool()> funcInit)
{
    int index = this->insertTab(-1, child, label, funcInit);
    return index;
}

int DelayTabWidget::addTab(QWidget *child, const QIcon &icon, const QString &label, std::function<bool()> funcInit)
{
    int index = this->insertTab(-1, child, icon, label, funcInit);
    return index;
}

int DelayTabWidget::insertTab(int index, QWidget *w, const QString &label, std::function<bool()> funcInit)
{
    index = this->insertTab(index, w, QIcon(), label, funcInit);
    return index;
}

int DelayTabWidget::insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label,
                              std::function<bool()> funcInit)
{
    if (!widget)
        return -1;

    QStackedWidget *foundStackedWidget = this->findChild<QStackedWidget *>();
    index = foundStackedWidget->insertWidget(index, widget);

    if (funcInit != nullptr) {
        mapInitFunc_.insert(std::pair<int, std::function<bool()>>(index, funcInit));
    }

    tabBar()->insertTab(index, icon, label);
    showEvent(nullptr);
    tabInserted(index);
    return index;
};
} // namespace DelayTab
} // namespace Ui
