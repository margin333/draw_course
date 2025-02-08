#ifndef WAITPROGRESSWIDGET_H
#define WAITPROGRESSWIDGET_H
#include "circularprogress.h"
#include "warningwidget.h"
#include "include/bne_app_error.h"
#include <QWidget>
#include <QEvent>
#include <QTimer>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QPushButton>
#include <QObject>
namespace Ui {
namespace Custom {
class WaitProgressWidget : public QWidget {
    Q_OBJECT

public:
    static WaitProgressWidget &getInstance(QWidget *parent = nullptr)
    {
        static WaitProgressWidget instance(parent);
        return instance;
    }

    BNEStatus Init(); // 初始化函数
    /*
     * 局部静态特性的方式实现单实例。
     * 静态局部变量只在当前函数内有效，其他函数无法访问。
     * 静态局部变量只在第一次被调用的时候初始化，也存储在静态存储区，生命周期从第一次被初始化起至程序结束止。
     * 根据C++11标准，静态局部变量的初始化是线程安全的，编译器会自动处理初始化的线程安全性。
     */
    void setBgColor(QColor color); // 设置背景颜色
    void start();                  // 开始
    void stop(bool isOk);          // 停止
    void SetText(const QString &text) { warningWidget_->SetText(text); }
    void IsButton(bool isOK) { warningWidget_->IsButton(isOK); } // 设置退出按钮是否有效

signals:
    void sig_finished(bool isOk); // 完成信号

private:
    WaitProgressWidget(QWidget *parent = nullptr); // 私有化构造函数
    ~WaitProgressWidget();
    WaitProgressWidget(const WaitProgressWidget &) = delete;            // 禁止复制
    WaitProgressWidget &operator=(const WaitProgressWidget &) = delete; // 禁止赋值

    void show(); // 限制外部调用show hide
    void hide();

protected:
    bool eventFilter(QObject *obj, QEvent *ev); // 事件过滤器
    void showEvent(QShowEvent *ev);             // 显示事件

private:
    QWidget *parentWidget_; // 父窗口
    QPushButton *quitButton;
    WarningWidget *warningWidget_; // 警告窗口
};

} // namespace Custom
} // namespace Ui

#endif // WAITPROGRESSWIDGET_H
