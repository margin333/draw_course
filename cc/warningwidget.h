#ifndef WARNINGWIDGET_H
#define WARNINGWIDGET_H
#include "circularprogress.h"
#include "octagon_button.h"
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <QObject>
#include <QMouseEvent>

class WarningWidget : public QWidget {
    Q_OBJECT

public:
    WarningWidget(QWidget *parent = nullptr);           // 构造函数
    void SetWindowWH(const int h, const int w);         // 设置宽高
    void Init();                                        // 初始化
    void SetText(const QString &text) { text_ = text; } // 设置文本
    void IsButton(bool isOk);                           // 设置按钮有效性

signals:
    void sig_quit();

private:
    QString text_;
    CircularProgress *progress_; // 环形进度条
    int height_;
    int width_;
    void paintEvent(QPaintEvent *event) override;
    Ui::OctagonButton::OctagonButton *quitButton;
};

#endif // WARNINGWIDGET_H
