#ifndef CENTERDELAYTABWIDGET_H
#define CENTERDELAYTABWIDGET_H

#include "center_tabwidget.h"
#include <QStackedWidget>

namespace Ui {
namespace CenterDelayTab {

class CenterDelayTabWidget : public Ui::TabBar::CenterTabWidget {
    Q_OBJECT
public:
    CenterDelayTabWidget(QWidget *parent = nullptr);

    int addTab(QWidget *child, const QString &label, std::function<bool()> funcInit = nullptr);

    int addTab(QWidget *child, const QIcon &icon, const QString &label, std::function<bool()> funcInit = nullptr);

    int insertTab(int index, QWidget *w, const QString &label, std::function<bool()> funcInit = nullptr);

    int insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label,
                  std::function<bool()> funcInit = nullptr);

private:
    std::map<int, std::function<bool()>> mapInitFunc_;
};
} // namespace CenterDelayTab
} // namespace Ui
#endif // CENTERDELAYTABWIDGET_H
