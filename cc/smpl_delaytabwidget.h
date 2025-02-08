#ifndef DELAYTABWIDGET_H
#define DELAYTABWIDGET_H

#include "simple_tabwidget.h"
#include <QStackedWidget>

namespace Ui {
namespace DelayTab {

class DelayTabWidget : public Ui::SmplTabWdt::SimpleTabWidget {
    Q_OBJECT
public:
    DelayTabWidget(QWidget *parent = nullptr);

    int addTab(QWidget *child, const QString &label, std::function<bool()> funcInit = nullptr);

    int addTab(QWidget *child, const QIcon &icon, const QString &label, std::function<bool()> funcInit = nullptr);

    int insertTab(int index, QWidget *w, const QString &label, std::function<bool()> funcInit = nullptr);

    int insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label,
                  std::function<bool()> funcInit = nullptr);

private:
    std::map<int, std::function<bool()>> mapInitFunc_;
};
} // namespace DelayTab
} // namespace Ui
#endif // DELAYTABWIDGET_H
