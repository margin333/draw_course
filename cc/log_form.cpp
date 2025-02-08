#include "log_form.h"
#include "ui_log_form.h"
#include <QDateTime>
#include <QDebug>
#include <QScrollBar>

namespace Ui {
namespace LogF {

QMutex LogForm::mutex_;
LogForm *LogForm::pInstance_ = nullptr;

LogForm::LogForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogForm)
{
    ui->setupUi(this);

    listWidget_ = new MyListWidget(this);
    listWidget_->setGeometry(10, 10, 548, 205);
    listWidget_->setObjectName("listwidget");
    listWidget_->setStyleSheet("QListWidget#listwidget {border: 1px solid "
                               "rgb(114, 159, 207);background-color: rgb(255, "
                               "255, 255);border-radius:8px;}");
    QMap<int, QStringList> styleMap;
    styleMap.clear();
    Ui::CustomF::CustomFont::GetFontStyleMap(styleMap);
    if (styleMap.size() <= 0) {
        return;
    }
    QFont font;
    font.setFamily(styleMap.value(int(AppEnum::BNE_APP_FONT_MEDIUM)).first());
    font.setPixelSize(AppEnum::BNE_APP_FONTSIZE_MIDDLE);
    font.setWeight(QFont::Medium);

    listWidget_->setFont(font);
    pinBtn_ = new LogButton(this); // 创建LogButton对象

    pinBtn_->setGeometry(520, 10, 26, 26);
    // 连接按钮的点击信号到相应的槽函数
    connect(pinBtn_, &QPushButton::clicked, [&]() {
        if (pinBtn_->GetIsPinned()) {
            listWidget_->SetMousePress(false);
        } else {
            listWidget_->SetMousePress(true);
        }
    });

    listWidget_->stackUnder(pinBtn_);
}

LogForm *LogForm::GetInstance()
{
    if (pInstance_ == nullptr) {
        QMutexLocker mutexLocker(&mutex_);
        if (pInstance_ == nullptr) {
            pInstance_ = new LogForm();
        }
    }
    return pInstance_;
}

LogForm::~LogForm() { delete ui; }

void LogForm::SetLogInfo(const QString &log)
{
    QString curTime = QDateTime::currentDateTime().time().toString("hh:mm:ss");
    QString logInfo = "    " + curTime + " " + log;
    QMutexLocker mutexLocker(&mutex_);
    //    ui->listWidget->addItem(logInfo);
    //    ui->listWidget->scrollToBottom();
    if (listWidget_) {
        listWidget_->addItem(logInfo);
        listWidget_->scrollToBottom();
    }
}

void LogForm::SetListWidgetDefaultGeo() { listWidget_->setGeometry(672, 672, 568, 128); }

void LogForm::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    this->setFocus();
    // MoveBase::mousePressEvent(e);
}

} // namespace LogF
} // namespace Ui

MyListWidget::MyListWidget(QWidget *parent)
    : QListWidget(parent)
{
}
