#include "bne_rmtableview.h"

namespace Ui {
namespace RMTable {
RMTableViewStyle::RMTableViewStyle()
    : imgBaoPian_(":/img/bne_img/img_baopian.png")
    , imgXiePian_(":/img/bne_img/img_xiepian.png")
    , imgHouPian_(":/img/bne_img/img_houpian.png")
    , imgDuoPian_(":/img/bne_img/img_duopian.png")
    , imgWanQu_(":/img/bne_img/img_wanqu.png")
{
}

void RMTableViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                     const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (PE_Frame == element) {
        return;
    } else {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

void RMTableViewStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                   const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (CE_ItemViewItem == element) {
        const QStyleOptionViewItem *pOptionItem = qstyleoption_cast<const QStyleOptionViewItem *>(option);
        QStyleOptionViewItem optionTemp(*pOptionItem);

        const BNERMTableView *pView = qobject_cast<const BNERMTableView *>(widget);
        if (pView == nullptr) {
            return;
        }
        optionTemp.rect.adjust(2, 0, -1, 0);
        QRect rectOption = optionTemp.rect;

        QRect rectFrame = rectOption.adjusted(1, rectOption.height() / 15, 0, -rectOption.height() / 15);

        QRect rectIndex = optionTemp.rect.adjusted(0, 0, -0.85 * rectOption.width(), 0);
        QRect rectId = optionTemp.rect.adjusted(0.5 * rectOption.width(), 0, -2, 0);

        QRect rectState = optionTemp.rect.adjusted(0.15 * rectOption.width(), rectOption.height() / 5,
                                                   -0.55 * rectOption.width(), -rectOption.height() / 5);
        //---------------------------------------------------------------------------------------------------------------
        QModelIndex modelTmp = pOptionItem->index;
        const QAbstractItemModel *pItemModel = modelTmp.model();
        if (pItemModel == nullptr) {
            return;
        }
        QMap map = pItemModel->itemData(modelTmp);
        auto it = map.begin();
        ++it;
        if (it == map.end()) {
            return;
        }
        int nData = (*(int *)it->constData());
        nData = (0xFFF & nData);
        int nSlot = (nData & 0x3F);

        int bIsExistBox = ((nData >> 7) & 0x1);

        QPainterPath path;
        path.addRoundedRect(rectFrame, rectFrame.height() / 5, rectFrame.height() / 5);
        painter->setClipPath(path);
        if (bIsExistBox) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(235, 243, 255));
            painter->drawRect(rectFrame);

            painter->setPen(QPen(QColor(172, 182, 219), 2));
            painter->setBrush(Qt::NoBrush);
            painter->drawRoundedRect(rectFrame, rectFrame.height() / 5, rectFrame.height() / 5);
        } else {
            painter->save();
            painter->translate(rectFrame.x(), rectFrame.y());
            QRect tmpFrame(0, 0, rectFrame.width(), rectFrame.height());
            QLinearGradient gradientFrame(0, 0, 0, rectFrame.height());
            gradientFrame.setColorAt(0, QColor(255, 0, 0)); // 红色
            gradientFrame.setColorAt(1, QColor(255, 145, 120));
            painter->setPen(Qt::NoPen);
            painter->setBrush(gradientFrame);
            painter->drawRoundedRect(tmpFrame, tmpFrame.height() / 5, tmpFrame.height() / 5);
            painter->restore();
        }
        painter->setClipping(false);

        QFont fontId(pOptionItem->font);
        fontId.setBold(false);
        fontId.setPixelSize(option->rect.height() / 2);
        painter->setFont(fontId);
        painter->setPen(Qt::black);
        painter->drawText(rectId, Qt::AlignCenter, optionTemp.text);

        int nResult = nData - nSlot;

        bool bIsEllipse = false;
        bool bIsTransfering = false;
        bool bHasBox = true;

        painter->save();
        QTransform transform;
        transform.translate(rectState.x(), rectState.y());
        painter->setTransform(transform, false);

        QRect tmp(0, 0, rectState.width(), rectState.height());
        QLinearGradient gradientState(0, 0, 0, rectState.height());
        gradientState.setColorAt(0, QColor(2, 212, 13)); // 绿色
        gradientState.setColorAt(1, QColor(115, 250, 178));
        painter->setBrush(gradientState);

        painter->setPen(Qt::NoPen);

        switch (nResult) {
        case 1024: {
            bIsEllipse = true;
            bIsTransfering = false;
            painter->setPen(Qt::white);
            painter->setBrush(Qt::NoBrush);

            break;
        }
        case 1216:
            bIsEllipse = true;
            bIsTransfering = false;

            break;
        case 1152:
            painter->setPen(Qt::black);
            painter->setBrush(Qt::NoBrush);
            bIsEllipse = true;
            bIsTransfering = false;

            break;
        case 384:
            bIsEllipse = false;
            bIsTransfering = false;
            break;
        case 128:
        case 640:
        case 896:
            bIsEllipse = false;
            bIsTransfering = true;
            break;
        default:
            painter->resetTransform();
            painter->restore();
            return;
        }

        QPainterPath pathState;
        if (bIsEllipse) {
            pathState.addRoundedRect(tmp, rectState.height() / 2, rectState.height() / 2);
        } else {
            pathState.addRoundedRect(tmp, rectState.height() / 5, rectState.height() / 5);
        }
        painter->drawPath(pathState);

        if (bIsTransfering && pView) {
            int nTime = pView->GetCSTTime();
            painter->setBrush(Qt::white);
            QRect rectEllipseSmall(tmp.width() / 2 - tmp.height() / 8, tmp.height() / 2 - tmp.height() / 8,
                                   tmp.height() / 4, tmp.height() / 4);
            painter->drawEllipse(rectEllipseSmall.adjusted(-tmp.width() / 8, 0, -tmp.width() / 8, 0));
            painter->drawEllipse(rectEllipseSmall);
            painter->drawEllipse(rectEllipseSmall.adjusted(tmp.width() / 8, 0, tmp.width() / 8, 0));
            QRect rectEllipseBig(tmp.width() / 2 - tmp.height() / 4, tmp.height() / 2 - tmp.height() / 4,
                                 tmp.height() / 2, tmp.height() / 2);
            switch (nTime) {
            case 0:
                rectEllipseBig.adjust(-tmp.width() / 8, 0, -tmp.width() / 8, 0);
                break;
            case 1:
                break;
            case 2:
                rectEllipseBig.adjust(tmp.width() / 8, 0, tmp.width() / 8, 0);
                break;
            default:
                painter->resetTransform();
                painter->restore();
                return;
            }
            painter->drawEllipse(rectEllipseBig);
        }

        painter->resetTransform();
        painter->restore();

        QFont fontTmp(pOptionItem->font);
        fontTmp.setBold(true);
        fontTmp.setPixelSize(option->rect.height() * 2 / 3);
        painter->setFont(fontTmp);
        painter->setPen(Qt::black);
        painter->drawText(rectIndex, Qt::AlignCenter, QStringLiteral("%0").arg(nSlot));
        return;

    } else {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }
};

//---BNESlotTableView-----------------------------------------------------------------------------
BNERMTableView::BNERMTableView(QWidget *parent)
    : QTableView(parent)
    , bChecked_(true)
    , bDraw_(true)
    , nDrawWarning_(false)
    , nTime_(0)
    , pTimerTransfer_(nullptr)
{
    this->setStyle(&style_);
    this->setEditTriggers(QTableView::NoEditTriggers);
    this->setGridStyle(Qt::NoPen);

    this->verticalHeader()->hide();
    this->horizontalHeader()->hide();
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPalette palette = viewport()->palette();
    palette.setColor(QPalette::Base, Qt::transparent);
    viewport()->setPalette(palette);

    pModel_ = new QStandardItemModel();
    this->setModel(pModel_);

    pTimerTransfer_ = new QTimer(this);
    pTimerTransfer_->setInterval(800);

    this->setMouseTracking(true);
    connect(this, &QTableView::clicked, this, &BNERMTableView::showToolTip);

    connect(pTimerTransfer_, &QTimer::timeout, this, [&]() {
        // bDraw_ = true;
        if (IsChecked()) {
            ++nTime_;
            if (nTime_ > 2) {
                nTime_ = 0;
                // bDraw_ = false;
            }

            this->viewport()->update();
            this->update();

        } else {
            nDrawWarning_ = !nDrawWarning_;
            this->viewport()->update();
            this->update();
        }
    });
}

void BNERMTableView::showToolTip(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    int row = index.row();
    QVariant ds = listItem_.at(listItem_.count() - row - 1)->text();

    if (!ds.isValid())
        return;

    QToolTip::showText(QCursor::pos(), ds.toString());
}

void BNERMTableView::SetCSTListItem(int nIndex, int nValue, QString str)
{
    if (nIndex < 0 || nIndex > listItem_.count() - 1) {
        return;
    }
    listItem_.at(nIndex)->setData(nValue);
    listItem_.at(nIndex)->setText(str);
}

void BNERMTableView::SetCSTInfo(int nReserveRow, QList<std::pair<int, QString>> &listSlotItem)
{
    listItem_.clear();
    int nColomn = 0;

    if (listSlotItem.count() % nReserveRow == 0) {
        nColomn = listSlotItem.count() / nReserveRow;
    } else {
        nColomn = listSlotItem.count() / nReserveRow + 1;
    }

    auto itlistItem = listSlotItem.begin();
    int nIndexList = 0;

    pModel_->setRowCount(nReserveRow);
    pModel_->setColumnCount(nColomn);

    for (int i = 0; i < nColomn; ++i) {
        for (int j = nReserveRow - 1; j >= 0; --j) {
            QStandardItem *item = nullptr;
            item = new QStandardItem();
            if (itlistItem != listSlotItem.end()) {
                listItem_.append(item);
                item->setData(listSlotItem[nIndexList].first);
                item->setText(listSlotItem[nIndexList].second);
                ++itlistItem;
                ++nIndexList;
            }
            pModel_->setItem(j, i, item);
        }
    }

    this->setModel(pModel_);
    this->AutoAdjustTableItemHeight();

    if (!pTimerTransfer_->isActive()) {
        pTimerTransfer_->start();
    }
}

void BNERMTableView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
    QPainter painter(viewport());
    if (!IsChecked()) {
        QRect rectView(viewport()->rect());
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(150, 155, 163, 0.7 * 255));
        painter.drawRoundedRect(rectView, rectView.width() / 20, rectView.width() / 20);

        if (nDrawWarning_) {
            painter.setPen(Qt::red);
            painter.setBrush(Qt::red);

            QTransform trans;
            trans.translate(rect().center().x(), rect().center().y());
            painter.setTransform(trans, false);
            QPainterPath path2 = getThirdEquTrgPath(QPointF(0, 0), width() / 3, 5);

            for (int i = 0; i < 3; ++i) {

                trans.rotate(120);
                painter.setTransform(trans, false);
                painter.drawPath(path2);
            }

            QRect rectWarning(0 - 25, 0 - 25, 50, 50);
            painter.setPen(Qt::white);
            QFont font = painter.font();
            font.setBold(true);
            font.setPixelSize(width() / 5);
            painter.setFont(font);
            painter.drawText(rectWarning, Qt::AlignCenter, "!");
        }
    }
}

void BNERMTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    this->AutoAdjustTableItemHeight();
};

void BNERMTableView::mouseMoveEvent(QMouseEvent *event) { QTableView::mouseMoveEvent(event); }

void BNERMTableView::AutoAdjustTableItemHeight()
{
    // 获取垂直表头
    QHeaderView *vHeaderView = this->verticalHeader();
    // 获取垂直表头的高度
    int vHeaderHeight = vHeaderView->height();
    // 计算每行的理论高度，以保证均匀分配表头高度
    int cell_height = vHeaderHeight;
    if (vHeaderView->count() > 0) {
        cell_height = vHeaderHeight / vHeaderView->count();
    }

    // 如果每行的理论高度大于最大值
    if (cell_height > vHeaderView->maximumSectionSize()) {
        // 将表头的调整模式设置为交互式
        vHeaderView->setSectionResizeMode(QHeaderView::Interactive);

        // 将所有行的高度设置为最大值
        for (int row = 0; row < vHeaderView->count(); row++)
            vHeaderView->resizeSection(row, vHeaderView->maximumSectionSize());
    } else {
        // 如果理论高度小于等于最大值，将表头的调整模式设置为拉伸（Stretch）
        vHeaderView->setSectionResizeMode(QHeaderView::Stretch);
    }
}

} // namespace RMTable
} // namespace Ui
