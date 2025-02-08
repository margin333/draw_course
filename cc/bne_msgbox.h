#ifndef BNEMSGBOX_H
#define BNEMSGBOX_H

#include "roundrect_button.h"

#include <QDebug>
#include <QTimer>
#include <QDialog>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>

typedef uint8_t MsgButtons;

namespace Ui {
namespace MsgBox {
class MsgPushButton;
class MsgWidget;

class MsgDrawWidget : public QWidget {
    Q_OBJECT
public:
    explicit MsgDrawWidget(QWidget *parent = nullptr);

    void setText(const QString &strText);

    const QString text() { return strDraw_; };

protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    QImage imgDraw_;
    QString strDraw_;

signals:
};
//-------------------------------------------------------------------------------------
class BNEMsgBox : public QDialog {
    Q_OBJECT
public:
    enum TYPE { NOICON, INFORMATION, QUESTION, WARNING, CRITICAL };

    enum MsgButton {
        CLOSE = 0x00,
        NOBUTTON = 0x01,
        NO = 0x02,
        YES = 0x04,
        IGNORE = 0x08,
        RESET = 0x10,
        RETRY = 0x20,
        SAVE = 0x40
    };

private:
    BNEMsgBox(TYPE nStyle, const QString &strTitle, const QString &strMsg, QWidget *parent = nullptr);

public:
    static MsgButton Information(QWidget *parent, const QString &title, const QString &text, MsgButtons buttons = YES,
                                 MsgButton defaultButton = CLOSE, int nMSec = 0);

    static MsgButton Question(QWidget *parent, const QString &title, const QString &text,
                              MsgButtons buttons = MsgButtons(YES | NO), MsgButton defaultButton = CLOSE,
                              int nMSec = 0);

    static MsgButton Warning(QWidget *parent, const QString &title, const QString &text, MsgButtons buttons = YES,
                             MsgButton defaultButton = CLOSE, int nMSec = 0);

    static MsgButton Critical(QWidget *parent, const QString &title, const QString &text, MsgButtons buttons = YES,
                              MsgButton defaultButton = CLOSE, int nMSec = 0);
    ~BNEMsgBox(void);

    BNEMsgBox(const BNEMsgBox &) = delete;
    BNEMsgBox &operator=(const BNEMsgBox &) = delete;

protected:
    QPushButton *AddBoxButton(MsgButton btn);

    static MsgButton ShowMessageBox(TYPE type, QWidget *parent, const QString &title, const QString &text,
                                    MsgButtons buttons, MsgButton defaultButton, int nMSec);

protected:
    void resizeEvent(QResizeEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

private:
    QPoint pointMove_;
    bool bPressmouse_;

    QLabel *pLabelTitle_;
    MsgPushButton *pBtnClose_;

    MsgWidget *pChildWidget_;
    QLabel *pLabelIcon_;
    MsgDrawWidget *pLabelMsg_;
    QDialogButtonBox *pBox_;
};

//----------------------------------------------------------------------------------------------------------------------------------------
class MsgWidget : public QWidget {
public:
    MsgWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e) override;

    void resizeEvent(QResizeEvent *e) override;
};

//------------------------------------------------------------------------------------------------------------------------------------------
class MsgPushButton : public QPushButton {
public:
    MsgPushButton(QWidget *parent = nullptr);

    MsgPushButton(const QString &str, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e) override;

    void resizeEvent(QResizeEvent *e) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool bPress_;
    QImage img_;
    QImage imgPress_;
};
} // namespace MsgBox
} // namespace Ui
//-----------------------------------------------------------------

#endif // BNEMSGBOX_H
