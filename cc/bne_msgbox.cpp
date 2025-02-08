#include "bne_msgbox.h"
namespace Ui {
namespace MsgBox {
//-----------------------------------------------------------------------------------------------------
BNEMsgBox::BNEMsgBox(TYPE nStyle, const QString &strTitle, const QString &strMsg, QWidget *parent)
    : QDialog(parent)
    , pointMove_(0, 0)
    , bPressmouse_(false)
    , pLabelTitle_(nullptr)
    , pBtnClose_(nullptr)
    , pChildWidget_(nullptr)
    , pLabelIcon_(nullptr)
    , pLabelMsg_(nullptr)
    , pBox_(nullptr)
{
    this->setMinimumSize(300, 130);

    this->resize(700, 250);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    //----------------------------------------
    QGridLayout *pLayoutGrid0 = new QGridLayout(this);

    pLabelTitle_ = new QLabel(strTitle);
    QFont font = pLabelTitle_->font();
    font.setBold(true);
    font.setPixelSize(18);
    pLabelTitle_->setFont(font);
    pLabelTitle_->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    // pLabelTitle_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    pBtnClose_ = new MsgPushButton(this);
    pBtnClose_->setMaximumSize(20, 20);
    connect(pBtnClose_, &QPushButton::clicked, this, [&]() { this->done(MsgButton::CLOSE); });
    pBtnClose_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    pLayoutGrid0->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum), 0, 0, 1, 1,
                          Qt::AlignCenter);
    pLayoutGrid0->addWidget(pLabelTitle_, 0, 1, 1, 20, Qt::AlignLeft);
    pLayoutGrid0->addWidget(pBtnClose_, 0, 21, 1, 1, Qt::AlignRight);

    //--------------------------------------------------------
    pChildWidget_ = new MsgWidget();
    pChildWidget_->resize(100, 100);
    // pChildWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    pLayoutGrid0->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum), 1, 0, 3, 1,
                          Qt::AlignCenter);

    pLayoutGrid0->addWidget(pChildWidget_, 1, 1, 3, 20);
    pChildWidget_->setMinimumSize(200, 100);
    pLayoutGrid0->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum), 1, 21, 3, 1,
                          Qt::AlignCenter);

    pLayoutGrid0->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum), 4, 0, 1, 22,
                          Qt::AlignCenter);

    this->setLayout(pLayoutGrid0);

    QGridLayout *pLayoutGrid1 = new QGridLayout();

    if (nStyle != TYPE::NOICON) {
        pLabelIcon_ = new QLabel("  ");
        pLabelIcon_->setAlignment(Qt::AlignCenter);
        switch (nStyle) { // No = 0, Information = 1,Question = 2, Critical = 3,Warning   = 4
        case INFORMATION:
            pLabelIcon_->setPixmap(QPixmap(":/MainMaskAlign/build/bin/image/bkg/img_msgbox_information.png"));
            break;
        case QUESTION:
            pLabelIcon_->setPixmap(QPixmap(":/MainMaskAlign/build/bin/image/bkg/img_msgbox_question.png"));
            break;
        case WARNING:
            pLabelIcon_->setPixmap(QPixmap(":/MainMaskAlign/build/bin/image/bkg/img_msgbox_warning.png"));
            break;
        case CRITICAL:
            pLabelIcon_->setPixmap(QPixmap(":/MainMaskAlign/build/bin/image/bkg/img_msgbox_critical.png"));
            break;
        default:
            break;
        }
    }

    pLabelMsg_ = new MsgDrawWidget();
    pLabelMsg_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QFont font1 = pLabelMsg_->font();
    font1.setPixelSize(17);
    pLabelMsg_->setFont(font1);
    pLabelMsg_->setText(strMsg);

    pBox_ = new QDialogButtonBox;

    pLayoutGrid1->addWidget(pLabelIcon_, 0, 0, 2, 4, Qt::AlignCenter);
    pLayoutGrid1->addWidget(pLabelMsg_, 0, 4, 2, 16);
    pLayoutGrid1->addWidget(pBox_, 2, 0, 1, 20, Qt::AlignCenter);
    pChildWidget_->setLayout(pLayoutGrid1);
    //---------------------------------------
}

BNEMsgBox::~BNEMsgBox(void) { }

QPushButton *BNEMsgBox::AddBoxButton(MsgButton btn)
{
    if (pBox_->buttons().count() >= 6) {
        return nullptr;
    }

    QString str = "";
    switch (btn) {
    case NO:
        str = "NO";
        break;
    case YES:
        str = "YES";
        break;
    case IGNORE:
        str = "IGNORE";
        break;
    case RESET:
        str = "RESET";
        break;
    case RETRY:
        str = "RETRY";
        break;
    case SAVE:
        str = "SAVE";
        break;
    case NOBUTTON:
    default:
        return nullptr;
    }

    Ui::RoundRectBtn::RoundRectButton *pBtn = new Ui::RoundRectBtn::RoundRectButton(str);
    if (pBtn) {
        pBtn->setMinimumHeight(30);
        QFont font = pBtn->font();
        font.setBold(true);
        font.setPixelSize(16);
        pBtn->setFont(font);

        pBox_->addButton(pBtn, QDialogButtonBox::ActionRole);
        connect(pBtn, &QPushButton::clicked, [&, btn]() { this->done(btn); });
        return pBtn;
    } else {
        return nullptr;
    }
}

void BNEMsgBox::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);

    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    painter.setClipPath(path);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(120, 183, 255), 1));

    painter.setBrush(QColor(120, 183, 255));
    painter.drawRect(rect());

    QPainterPath path1;
    path1.addRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);
    painter.setBrush(QColor(235, 243, 255));
    painter.drawPath(path1);

    return;
}

void BNEMsgBox::resizeEvent(QResizeEvent *e)
{

    QDialog::resizeEvent(e);

    QFontMetrics fontMetrics(pLabelMsg_->font());
    QRect tmp(pLabelMsg_->rect());
    tmp.adjust(5, 5, -5, -5);
    int height = fontMetrics.height();
    int nLeft = 0;
    int nN = 0;
    for (const QChar &ch : pLabelMsg_->text()) {
        int width = fontMetrics.horizontalAdvance(ch);
        if (nLeft + width >= tmp.width()) {
            nLeft = 0;
            ++nN;
        }

        if (ch == '\n' || ch == '\r') {
            nLeft = 0;
            ++nN;
        }

        nLeft += width;
    }
    pLabelMsg_->setFixedHeight(((nN + 1) * (height + 5) + 10));
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    this->setMask(path.toFillPolygon().toPolygon());

    return;
}

void BNEMsgBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        bPressmouse_ = true;
        pointMove_ = e->pos();
        ;
    }
}

void BNEMsgBox::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    if (bPressmouse_) {
        QPoint posMove = e->globalPos();

        this->move(posMove - pointMove_);
    }
}

void BNEMsgBox::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    bPressmouse_ = false;
}

BNEMsgBox::MsgButton BNEMsgBox::ShowMessageBox(TYPE type, QWidget *parent, const QString &title, const QString &text,
                                               MsgButtons buttons, MsgButton defaultButton, int nMSec)
{
    BNEMsgBox msgBox(type, title, text, parent);

    uint mask = BNEMsgBox::NOBUTTON;
    if (!(mask & buttons)) {
        while (mask <= BNEMsgBox::SAVE) {
            uint sb = buttons & mask;
            mask <<= 1;
            if (!sb)
                continue;
            QPushButton *button = msgBox.AddBoxButton((BNEMsgBox::MsgButton)sb);

            if (defaultButton != BNEMsgBox::NOBUTTON && sb == defaultButton) {
                button->setDefault(true);
                button->setFocus();
            }
        }
    }

    if (nMSec > 0) {
        QTimer *timer = new QTimer(&msgBox);
        timer->setSingleShot(true);
        timer->setInterval(nMSec);
        connect(timer, &QTimer::timeout, &msgBox, [&]() { msgBox.done(CLOSE); });
        timer->start();
    }

    return (BNEMsgBox::MsgButton)msgBox.exec();
}

BNEMsgBox::MsgButton BNEMsgBox::Information(QWidget *parent, const QString &title, const QString &text,
                                            MsgButtons buttons, MsgButton defaultButton, int nMSec)
{
    return ShowMessageBox(INFORMATION, parent, title, text, buttons, defaultButton, nMSec);
};

BNEMsgBox::MsgButton BNEMsgBox::Question(QWidget *parent, const QString &title, const QString &text, MsgButtons buttons,
                                         MsgButton defaultButton, int nMSec)
{
    return ShowMessageBox(QUESTION, parent, title, text, buttons, defaultButton, nMSec);
}

BNEMsgBox::MsgButton BNEMsgBox::Warning(QWidget *parent, const QString &title, const QString &text, MsgButtons buttons,
                                        MsgButton defaultButton, int nMSec)
{
    return ShowMessageBox(WARNING, parent, title, text, buttons, defaultButton, nMSec);
}

BNEMsgBox::MsgButton BNEMsgBox::Critical(QWidget *parent, const QString &title, const QString &text, MsgButtons buttons,
                                         MsgButton defaultButton, int nMSec)
{
    return ShowMessageBox(CRITICAL, parent, title, text, buttons, defaultButton, nMSec);
}

//--------------------------------------------------------------------------

MsgWidget ::MsgWidget(QWidget *parent)
    : QWidget(parent)
{
}

void MsgWidget ::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);

    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    painter.setClipPath(path);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(228, 237, 250), 1));

    painter.setBrush(QColor(228, 237, 250));
    painter.drawRect(rect());

    QPainterPath path1;
    path1.addRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);
    painter.setBrush(QColor(255, 255, 255));
    painter.drawPath(path1);

    return;
};

void MsgWidget ::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    this->setMask(path.toFillPolygon().toPolygon());

    return;
};

//-----------------------------------------------

MsgPushButton::MsgPushButton(QWidget *parent)
    : QPushButton(parent)
    , bPress_(false)
    , img_(":/MainMaskAlign/build/bin/image/bkg/img_msgbox_close.png")
    , imgPress_(":/MainMaskAlign/build/bin/image/bkg/img_msgbox_close.png")
{
    if (!imgPress_.isNull()) {
        {
            for (int y = 0; y < imgPress_.height(); y++) {
                for (int x = 0; x < imgPress_.width(); x++) {
                    QRgb pixel = imgPress_.pixel(x, y);
                    int alpha = qAlpha(pixel);
                    if (alpha == 255) {
                        imgPress_.setPixel(x, y, qRgba(255, 0, 0, 255));
                    }
                }
            }
        }
    }
};

MsgPushButton::MsgPushButton(const QString &str, QWidget *parent)
    : QPushButton(str, parent)
    , bPress_(false)
    , img_(":/close.png")
    , imgPress_(":/close.png")
{
    if (!imgPress_.isNull()) {
        {
            for (int y = 0; y < imgPress_.height(); y++) {
                for (int x = 0; x < imgPress_.width(); x++) {
                    QRgb pixel = imgPress_.pixel(x, y);
                    int alpha = qAlpha(pixel);
                    if (alpha == 255) {
                        imgPress_.setPixel(x, y, qRgba(255, 0, 0, 255));
                    }
                }
            }
        }
    }
};

void MsgPushButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    QRect targetRect = rect();
    QRectF sourceRect = img_.rect();
    painter.setPen(Qt::NoPen);
    if (!bPress_) {
        if (!img_.isNull()) {
            painter.drawImage(targetRect, img_, sourceRect);
        }
    } else {
        if (!imgPress_.isNull()) {
            painter.drawImage(targetRect, imgPress_, sourceRect);
        }
    }

    return;
};

void MsgPushButton::resizeEvent(QResizeEvent *e)
{
    int newSize = e->size().height();
    if (this->size() != QSize(newSize, newSize)) {
        this->setFixedSize(newSize, newSize);
    }
    QPushButton::resizeEvent(e);
};

void MsgPushButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);
    bPress_ = true;
    update();
}
void MsgPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);
    bPress_ = false;
    update();
}

//-----------------------------------------------------------------------
MsgDrawWidget::MsgDrawWidget(QWidget *parent)
    : QWidget{parent}
{
    QFont font = this->font();
    font.setPixelSize(16);
    this->setFont(font);
}

void MsgDrawWidget::setText(const QString &strText) { strDraw_ = strText; }

void MsgDrawWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);

    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    painter.setClipPath(path);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //-------------------------------------------------------------------
    painter.save();
    painter.translate(5, 5);

    painter.restore();
    painter.setPen(QPen(Qt::black, 1));

    QFontMetrics fontMetrics(font());

    painter.save();
    painter.translate(5, 5);
    QRect tmp(rect());
    tmp.adjust(5, 5, -5, -5);
    int nLeft = 0;
    int nN = 0;
    int height = fontMetrics.height();

    QString strLine = "";
    QString strTmpWord = "";
    for (const QChar &ch : text() + " ") {
        if (ch == '\n' || ch == '\r') {
            //            strLine.append(strTmpWord);
            //            int width = fontMetrics.horizontalAdvance(strLine);
            //            painter.drawText(QRect(0, 0, width, height), strLine);
            //            painter.translate(0, height);
            //            strLine.clear();
            //            strTmpWord.clear();

            int width = fontMetrics.horizontalAdvance(strLine + strTmpWord);

            if (width >= tmp.width()) {
                painter.drawText(QRect(0, 0, fontMetrics.horizontalAdvance(strLine), height + 2), strLine);
                painter.translate(0, height);
                painter.drawText(QRect(0, 0, fontMetrics.horizontalAdvance(strTmpWord), height + 2), strTmpWord);

            } else {
                painter.drawText(QRect(0, 0, fontMetrics.horizontalAdvance(strLine + strTmpWord), height + 2),
                                 strLine + strTmpWord);
            }
            painter.translate(0, height);
            strLine.clear();
            strTmpWord.clear();
            ++nN;
            continue;
        }

        if (ch == " ") {
            strTmpWord.append(ch);
            int width = fontMetrics.horizontalAdvance(strLine + strTmpWord);

            if (width >= tmp.width()) {
                painter.drawText(QRect(0, 0, fontMetrics.horizontalAdvance(strLine), height + 2), strLine);
                painter.translate(0, height);
                ++nN;
                strLine.clear();
            }
            strLine.append(strTmpWord);
            strTmpWord.clear();
        } else {
            int width = fontMetrics.horizontalAdvance(strTmpWord + ch);
            if (width >= tmp.width()) {
                if (!strLine.isEmpty()) {
                    painter.drawText(QRect(0, 0, fontMetrics.horizontalAdvance(strLine), height + 2), strLine);
                    painter.translate(0, height);
                    strLine.clear();
                }

                painter.drawText(QRect(0, 0, fontMetrics.horizontalAdvance(strTmpWord), height + 2), strTmpWord);
                painter.translate(0, height);
                ++nN;
                strTmpWord.clear();
            }

            strTmpWord.append(ch);
            continue;
        }
    }
    strLine.append(strTmpWord);
    if (!strLine.isEmpty()) {
        painter.drawText(QRect(0, 0, fontMetrics.horizontalAdvance(strLine), height + 2), strLine);
    }

    painter.restore();
    return;
}

void MsgDrawWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    QFontMetrics fontMetrics(this->font());
    QRect tmp(this->rect());
    tmp.adjust(5, 5, -5, -5);
    int height = fontMetrics.height();
    int nLeft = 0;
    int nN = 0;
    for (const QChar &ch : this->text()) {
        int width = fontMetrics.horizontalAdvance(ch);
        if (nLeft + width >= tmp.width()) {
            nLeft = 0;
            // painter.translate(0, height + 10);
            ++nN;
        }

        if (ch == '\n' || ch == '\r') { // 检查换行符
            nLeft = 0;
            ++nN;
        }

        // painter.translate(nLeft, 0);
        // painter.drawText(QRect(nLeft, 0, width, height), ch);
        nLeft += width;
    }
    this->setFixedHeight(((nN + 1) * (height + 5) + 10));

    QPainterPath path;
    path.addRoundedRect(rect(), 10, 10);
    this->setMask(path.toFillPolygon().toPolygon());
    return;
}
} // namespace MsgBox
} // namespace Ui
