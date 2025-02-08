#ifndef LOG_FORM_H
#define LOG_FORM_H

#include "../../common/model/custom/bne_widget_factory.h"
#include "../../common/model/custom/custom_font.h"
#include "../move_base.h"
#include "../touch_move.h"
#include <QDebug>
#include <QDialog>
#include <QListWidget>
#include <QMouseEvent>
#include <QMutex>
#include <QWidget>

class LogButton : public Ui::RoundRectBtn::RoundRectButton {
    Q_OBJECT

public:
    LogButton(QWidget *parent = nullptr)
        : Ui::RoundRectBtn::RoundRectButton(parent)
    {

        imgFixed_.load(":/MainMaskAlign/build/bin/image/bkg/infix.png");
        imgInfix_.load(":/MainMaskAlign/build/bin/image/bkg/fixed.png");
    }

    // 获取isPinned_的值
    bool GetIsPinned() { return isPinned_; }

protected:
    virtual void paintEvent(QPaintEvent *event) override
    {
        // QPushButton::paintEvent(event);
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        painter.save();
        QPainterPath path;
        path.addRect(rect());
        painter.setClipPath(path);

        QLinearGradient gradient(rect().x(), rect().y(), rect().x(), rect().height());

        if (!this->isEnabled()) {
            gradient.setColorAt(0, QColor(255, 0, 0, 128)); // 红色背景, 半透明
            gradient.setColorAt(0.515, QColor(255, 0, 0, 128));
            gradient.setColorAt(1, QColor(255, 0, 0, 128));
        } else {
            gradient.setColorAt(0, QColor(106, 157, 230));
            gradient.setColorAt(0.515, QColor(6, 57, 115));
            gradient.setColorAt(1, QColor(106, 157, 230));
        }

        // 创建渐变画刷
        QPen penGradient(gradient, 2);
        painter.setPen(Qt::NoPen);
        if (!isPinned_) {
            if (!imgFixed_.isNull()) { // 计算目标大小和希望的缩放比例
                painter.drawImage(rect(), imgInfix_);
            }
            painter.setPen(penGradient);

        } else {
            if (!imgInfix_.isNull()) { // 计算目标大小和希望的缩放比例
                painter.drawImage(rect(), imgFixed_);
            }
            painter.setPen(QPen(Qt::white, 2));
        }

        // painter.drawRoundRect(rect(), 14, 14);
        QString strTmp = this->text();

        painter.drawText(rect(), Qt::AlignCenter, strTmp);
        painter.restore();
    }

    virtual void mouseReleaseEvent(QMouseEvent *event) override
    {
        QPushButton::mouseReleaseEvent(event); // 调用QPushButton的鼠标按下事件处理函数
    }

    virtual void mousePressEvent(QMouseEvent *event) override
    {
        QPushButton::mousePressEvent(event); // 调用QPushButton的鼠标按下事件处理函数
        isPinned_ = !isPinned_;
    }; // 鼠标按下事件

private:
    QImage imgFixed_;
    QImage imgInfix_;
    bool isPinned_;
};

class ListWidgetEventFilter : public QObject {
    Q_OBJECT

signals:
    void WidgetTouchRightSignal();

public:
    ListWidgetEventFilter(QObject *parent = nullptr)
        : QObject(parent)
    {
        touchMove_ = new Ui::TouchMv::TouchMove(this);
    }
    void SetListWidget(QWidget *w) { this->listW = w; }
    void SetWidget(QListWidget *widget) { this->widget_ = widget; }
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched == listW) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // 将事件转换为鼠标事件

            if (mouseEvent->type() ==
                QEvent::MouseButtonPress) { // 记录下触摸点击开始位置
                                            //            touchPressPt_.setX(mouseEvent->globalX());
                                            //            touchPressPt_.setY(mouseEvent->globalY());
                mousePress_ = true;
                mousePoint_ = mouseEvent->globalPos() - this->widget_->pos();
                mouseEvent->accept();
            }
            if (mouseEvent->type() == QEvent::MouseButtonRelease) { // 记录下触摸点击释放位置
                //            touchReleasePt_.setX(mouseEvent->globalX());
                //            touchReleasePt_.setY(mouseEvent->globalY());
                mousePress_ = false;
            }
            if (mouseEvent->type() == QEvent::MouseMove) {
                if (mousePress_) {
                    this->widget_->raise();
                    this->widget_->move(mouseEvent->globalPos() - mousePoint_);
                    this->widget_->show();
                }
            }
        }
        //        int xOffsetRight = touchReleasePt_.x() - touchPressPt_.x();
        //        int yOffset = qAbs(touchReleasePt_.y() - touchPressPt_.y());

        //        if (watched == listW) {
        //            if (listW->rect().contains(mouseEvent->pos())) {
        //                if (xOffsetRight > 20 && mouseEvent->type() ==
        //                QEvent::MouseButtonRelease &&
        //                    yOffset < 50) { // log框向右滑动
        //                    qDebug() << "listwidget button log框向右滑动";
        //                    //                    TouchLogRSideRightProcess();
        //                    emit WidgetTouchRightSignal();
        //                    return true; // 返回true表示事件被过滤并处理，不再传递
        //                }
        //            }
        //        }
        return QObject::eventFilter(watched, event); // 传递给父类处理其他事件
    }

private:
    QWidget *listW;
    QPoint touchPressPt_;               // 触摸按下时屏幕坐标
    QPoint touchReleasePt_;             // 触摸释放时屏幕坐标
    Ui::TouchMv::TouchMove *touchMove_; // 界面移动切换类
    QListWidget *widget_;               //
    QPoint mousePoint_;
    bool mousePress_;
};

class MyListWidget : public QListWidget {
    Q_OBJECT
public:
    MyListWidget(QWidget *parent = nullptr);
    void SetMousePress(bool press) { canMove_ = press; }

protected:
    void mousePressEvent(QMouseEvent *e)
    {

        if (canMove_ && (e->button() == Qt::LeftButton)) {
            mousePress_ = true;
            mousePoint_ = e->globalPos() - static_cast<QWidget *>(this->parent())->pos();
        }
        //    e->accept();
        QWidget::mousePressEvent(e);
    }
    // set canMove_
    void mouseMoveEvent(QMouseEvent *e)
    {
        QListWidget::mouseMoveEvent(e);
        if (mousePress_ && canMove_) {
            static_cast<QWidget *>(this->parent())->raise();
            static_cast<QWidget *>(this->parent())->move(e->globalPos() - mousePoint_);
            static_cast<QWidget *>(this->parent())->show();
        }
    }
    void mouseReleaseEvent(QMouseEvent *e)
    {
        QListWidget::mouseReleaseEvent(e);
        mousePress_ = false;
    }

private:
    QPoint mousePoint_;
    bool mousePress_;
    bool canMove_ = true;
};

namespace Ui {
class LogForm;
} // namespace Ui

namespace Ui {
namespace LogF {
class LogForm : public QWidget {
    Q_OBJECT

public:
    static LogForm *GetInstance();
    ~LogForm();
    void SetLogInfo(const QString &log);
    void SetListWidgetDefaultGeo();

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    static QMutex mutex_;
    static LogForm *pInstance_;
    QPoint dragPos;
    bool isPinned_ = false; //
    QLabel *pinLabel_ = nullptr;
    //  LogButton *pinBtn_ = nullptr; // 开始按钮
    LogButton *pinBtn_;

private:
    explicit LogForm(QWidget *parent = nullptr);
    Ui::LogForm *ui;
    MyListWidget *listWidget_ = nullptr;
};

} // namespace LogF
} // namespace Ui
#endif // LOG_FORM_H
