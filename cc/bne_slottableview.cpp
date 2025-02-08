#include "bne_slottableview.h"
namespace Ui {
namespace SlotTable {
SlotTableViewStyle::SlotTableViewStyle()
    : imgBaoPian_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_baopian.png")
    , imgXiePian_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_xiepian.png")
    , imgHouPian_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_houpian.png")
    , imgDuoPian_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_duopian.png")
    , imgWanQu_(":/FIA/build/bin/image/maintainManageBkg/FIA/img_wanqu.png")
{
}

void SlotTableViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter,
                                       const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (PE_Frame == element) {
        return;
    } else {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

void SlotTableViewStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                                     const QWidget *widget) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (CE_ItemViewItem == element) {
        const QStyleOptionViewItem *pOptionItem = qstyleoption_cast<const QStyleOptionViewItem *>(option);
        QStyleOptionViewItem optionTemp(*pOptionItem);

        const BNESlotTableView *pView = qobject_cast<const BNESlotTableView *>(widget);
        if (pView == nullptr) {
            return;
        }
        optionTemp.rect.adjust(2, 0, -2, 0);
        QRect rectOption = optionTemp.rect;

        QRect rectFrame = rectOption.adjusted(1, rectOption.height() / 15, -1, -rectOption.height() / 15);

        QRect rectIndex = optionTemp.rect.adjusted(0, 0, -0.8 * rectOption.width(), 0);

        QRect rectState = optionTemp.rect.adjusted(0.2 * rectOption.width(), rectOption.height() / 5,
                                                   -0.1 * rectOption.width(), -rectOption.height() / 5);

        QPainterPath path;
        path.addRoundedRect(rectFrame, rectFrame.height() / 5, rectFrame.height() / 5);
        painter->setClipPath(path);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(235, 243, 255));
        painter->drawRect(rectFrame);

        painter->setPen(QPen(QColor(172, 182, 219), 2));
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(rectFrame, rectFrame.height() / 5, rectFrame.height() / 5);

        //---------------------------------------------------------------------------------------------------------------
        QModelIndex modelTmp = pOptionItem->index;
        const QAbstractItemModel *pItemModel = modelTmp.model();
        if (pItemModel == nullptr) {
            return;
        }
        int nRowCurrent = modelTmp.row();
        int nColumnCurrent = modelTmp.column();
        int nRowCount = pItemModel->rowCount();

        QMap map = pItemModel->itemData(modelTmp);
        auto it = map.begin();

        if (it == map.end()) {
            return;
        }
        int nData = (*(int *)it->constData());

        int nResult = -1;
        nResult = (0x1FFF & nData);
        int nStateSlot = -1;

        nStateSlot = nData & (0xF);

        if (nStateSlot < 0 || nStateSlot > 6) {
            return;
        }

        QFont fontTmp(pOptionItem->font);
        fontTmp.setBold(true);
        fontTmp.setPixelSize(option->rect.height() - 1);
        painter->setFont(fontTmp);
        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);

        painter->drawText(rectIndex, Qt::AlignCenter,
                          QStringLiteral("%0").arg(nColumnCurrent * nRowCount + nRowCount - (nRowCurrent)));

        //--------------------------------------------------
        painter->save();
        QTransform transform;
        transform.translate(rectState.x(), rectState.y());
        painter->setTransform(transform, false);

        QRect tmp(0, 0, rectState.width(), rectState.height());
        QLinearGradient gradientState(0, 0, 0, rectState.height());

        if (nStateSlot > 1) {
            gradientState.setColorAt(0, QColor(255, 0, 0));
            gradientState.setColorAt(1, QColor(255, 145, 120));
            painter->setPen(Qt::NoPen);
            painter->setBrush(gradientState);

            QPainterPath pathState;
            pathState.addRoundedRect(tmp, rectState.height() / 2, rectState.height() / 2);
            painter->setClipPath(pathState);
            QImage imgRes;
            switch (nStateSlot) {
            case 2:
                imgRes = imgHouPian_;
                break;
            case 3: {
                transform.setMatrix(1, 0, transform.m13(), -0.5, 1, transform.m23(), transform.m31() - (1 + 7),
                                    transform.m32(), transform.m33());

                painter->setPen(Qt::NoPen);
                painter->setBrush(gradientState);
                for (int i = 0; i < rectState.width() / 8 + 5; ++i) {

                    transform.setMatrix(1, 0, transform.m13(), -0.5, 1, transform.m23(), transform.m31() + (1 + 7),
                                        transform.m32(), transform.m33());
                    painter->setTransform(transform, false);
                    painter->drawRect(QRect(0, 0, 7, rectState.height()));
                }
                painter->resetTransform();
                painter->restore();
                return;
            }
            case 4:
                imgRes = imgWanQu_;
                break;
            case 5:
                imgRes = imgDuoPian_;
                break;
            case 6:
                imgRes = imgBaoPian_;
                break;
            default:
                painter->restore();
                painter->resetTransform();
                return;
            }
            if (imgRes.isNull()) {
                painter->resetTransform();
                painter->restore();
                return;
            }

            QImage imgDraw =
                imgRes.scaled(tmp.width() / 5, tmp.width() * imgRes.height() / imgRes.width() / 5, Qt::KeepAspectRatio);

            painter->drawPath(pathState);
            if (!imgDraw.isNull()) {
                painter->save();

                transform.translate(tmp.width() / 2 - imgDraw.width() / 2, tmp.height() / 2 - imgDraw.height() / 2);
                painter->setTransform(transform, false);

                painter->drawImage(imgDraw.rect(), imgDraw);

                painter->restore();
            }
            painter->resetTransform();
            painter->restore();
            return;
        }

        // 无片和正常片------------------------------------------
        bool bIsEllipse = false;
        bool bHasPoint = false;
        painter->setPen(Qt::NoPen);

        switch (nResult) {
            //--------------
        case 17: // (曝光状态)0000  (工位)0001  (卡槽状态)0001
        case 33: // 0000  0010  0001
        case 49: // 0000  0011  0001
            bIsEllipse = false;
            gradientState.setColorAt(0, QColor(54, 54, 54));
            gradientState.setColorAt(1, QColor(115, 115, 115));
            bHasPoint = true;
            break;
        case 273: // 0001 0001 0001
        case 289: // 0001 0010 0001
        case 305: // 0001 0011 0001
            bIsEllipse = false;
            gradientState.setColorAt(0, QColor(2, 212, 13));
            gradientState.setColorAt(1, QColor(115, 250, 178));
            bHasPoint = true;
            break;
        case 785: // 0011 0001 0001
        case 801: // 0011 0010 0001
        case 817: // 0011 0011 0001
            bIsEllipse = false;
            gradientState.setColorAt(0, QColor(255, 0, 0));
            gradientState.setColorAt(1, QColor(255, 145, 120));
            bHasPoint = true;
            break;
            //-------------------
        // case 1:
        case 65: // 0000 0100 0001
            bIsEllipse = false;
            gradientState.setColorAt(0, QColor(54, 54, 54));
            gradientState.setColorAt(1, QColor(115, 115, 115));
            bHasPoint = false;
            break;
        case 0: // 0000 0000 0000
            bIsEllipse = true;
            painter->setPen(Qt::black);
            gradientState.setColorAt(0, QColor(Qt::transparent));
            gradientState.setColorAt(1, QColor(Qt::transparent));
            bHasPoint = false;
            break;
        // case 513:
        case 577: // 0010 0100 0001
            bIsEllipse = false;
            gradientState.setColorAt(0, QColor(0, 69, 189));
            gradientState.setColorAt(1, QColor(37, 121, 206));
            bHasPoint = false;
            break;
        case 321: // 0001 0100 0001
            bIsEllipse = false;
            gradientState.setColorAt(0, QColor(2, 212, 13));
            gradientState.setColorAt(1, QColor(115, 250, 178));
            bHasPoint = false;
            break;
        case 833: // 0011 0100 0001
            bIsEllipse = false;
            gradientState.setColorAt(0, QColor(255, 0, 0));
            gradientState.setColorAt(1, QColor(255, 145, 120));
            bHasPoint = false;
            break;
            //------------------------------
        case 1: // 0000 0000 0001
            bIsEllipse = true;
            gradientState.setColorAt(0, QColor(54, 54, 54));
            gradientState.setColorAt(1, QColor(115, 115, 115));
            bHasPoint = false;
            break;
        case 769: // 0011 0000 0001
            bIsEllipse = true;
            gradientState.setColorAt(0, QColor(255, 0, 0));
            gradientState.setColorAt(1, QColor(255, 145, 120));
            bHasPoint = false;
            break;
        case 257: // 0001 0000 0001
            bIsEllipse = true;
            gradientState.setColorAt(0, QColor(2, 212, 13));
            gradientState.setColorAt(1, QColor(115, 250, 178));
            bHasPoint = false;
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

        painter->setBrush(gradientState);
        painter->drawPath(pathState);

        //----------------------------------------------------------

        if (bHasPoint && pView) {
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
                painter->restore();
                return;
            }
            painter->drawEllipse(rectEllipseBig);
        }
        painter->resetTransform();
        painter->restore();
        return;

    } else {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }
};

//---BNESlotTableView-----------------------------------------------------------------------------
BNESlotTableView::BNESlotTableView(QWidget *parent)
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

    // 连接定时器的超时信号到槽函数
    connect(pTimerTransfer_, &QTimer::timeout, this, [&]() {
        // bDraw_ = true;
        if (IsChecked()) {
            ++nTime_;
            if (nTime_ > 2) {
                nTime_ = 0;
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

void BNESlotTableView::SetCSTListItem(int nIndex, int nValue)
{
    if (nIndex < 0 || nIndex > listItem_.count() - 1) {
        return;
    }
    listItem_.at(nIndex)->setData(nValue);
}

void BNESlotTableView::SetCSTInfo(int nReserveRow, QList<int> &listSlotItem)
{
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
                item->setData(listSlotItem[nIndexList]);
                ++itlistItem;
                ++nIndexList;
            }
            pModel_->setItem(j, i, item);
        }
    }

    //  设置模型到QTableView
    this->setModel(pModel_);
    this->AutoAdjustTableItemHeight();

    if (!pTimerTransfer_->isActive()) {
        pTimerTransfer_->start();
    }
}
QPainterPath BNESlotTableView::GetThirdEquTrgPath(QPointF pointCenter, double fLength, double fRadius)
{
    double fX = pointCenter.x();
    double fY = pointCenter.y();

    double fLengthHalfSide = fLength / 2;
    double fDiameter = 2 * fRadius;

    double PAI = 3.1415926;
    double fRadio = tan(PAI * 60 / 180);

    QPainterPath path1;
    path1.moveTo(0, 0);
    path1.lineTo(0, fLengthHalfSide / fRadio);
    path1.lineTo(fLengthHalfSide - fRadius * fRadio, fLengthHalfSide / fRadio);
    path1.arcTo(fLengthHalfSide - fRadius - fRadius * fRadio, fLengthHalfSide / fRadio - fDiameter, fDiameter,
                fDiameter, 270, 60);
    path1.closeSubpath();
    path1.translate(fX, fY);

    QPainterPath path2;
    path2.moveTo(0, 0);
    path2.lineTo(0, fLengthHalfSide / fRadio);
    path2.lineTo((fLengthHalfSide - fRadius * fRadio) * -1, fLengthHalfSide / fRadio);
    path2.arcTo(-1 * (fLengthHalfSide - fRadius - fRadius * fRadio) - 2 * fRadius, fLengthHalfSide / fRadio - fDiameter,
                fDiameter, fDiameter, 270, -60);

    path2.closeSubpath();
    path2.translate(fX, fY);

    QPainterPath pathRes = path1.united(path2);
    return pathRes;
}

void BNESlotTableView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);

    if (!IsChecked()) {
        QPainter painter(viewport());
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
            QPainterPath path2 = GetThirdEquTrgPath(QPointF(0, 0), width() / 3, 5);

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

void BNESlotTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    this->AutoAdjustTableItemHeight();
};

void BNESlotTableView::AutoAdjustTableItemHeight()
{
    QHeaderView *vHeaderView = this->verticalHeader();

    int vHeaderHeight = vHeaderView->height();

    int cell_height = vHeaderHeight;
    if (vHeaderView->count() > 0) {
        cell_height = vHeaderHeight / vHeaderView->count();
    }

    if (cell_height > vHeaderView->maximumSectionSize()) {
        vHeaderView->setSectionResizeMode(QHeaderView::Interactive);

        for (int row = 0; row < vHeaderView->count(); row++)
            vHeaderView->resizeSection(row, vHeaderView->maximumSectionSize());
    } else {
        vHeaderView->setSectionResizeMode(QHeaderView::Stretch);
    }
}

} // namespace SlotTable
} // namespace Ui
