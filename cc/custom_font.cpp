#include <QCoreApplication>
#include <QFontDatabase>
#include <QDebug>
#include "custom_font.h"

QMap<int, QStringList> Ui::CustomF::CustomFont::fontStyleMap_ = {}; // 初始化fontStyleMap_

Ui::CustomF::CustomFont::CustomFont() { }

Ui::CustomF::CustomFont::~CustomFont() { }

/**************************************************************************
 * 名称:    InsertFontStyleMap
 * 功能:    初始化插入字体到Map中，最先调用
 * 输入参数:NULL
 * 输出参数:NULL
 * 返回值:  字体Map数量
 * 创建者:  zhangzy
 * 创建时间: 2023-10-11
 ***************************************************************************/
int Ui::CustomF::CustomFont::InsertFontStyleMap()
{
    QString fontDir = QCoreApplication::applicationDirPath() + "/font/ALIBABA-PUHUITI-REGULAR.TTF";
    int fontIDRegular = QFontDatabase::addApplicationFont(fontDir); // 添加阿里巴巴惠普体字体
    if (fontIDRegular >= 0) {
        QStringList fontRegular = QFontDatabase::applicationFontFamilies(fontIDRegular); // 通过fontID获取字体名称
        fontStyleMap_.insert(fontIDRegular, fontRegular);                                // 插入Map
    }

    fontDir = QCoreApplication::applicationDirPath() + "/font/ALIBABA-PUHUITI-LIGHT.TTF";
    int fontIDLight = QFontDatabase::addApplicationFont(fontDir);
    if (fontIDLight >= 0) {
        QStringList fontLight = QFontDatabase::applicationFontFamilies(fontIDLight);
        fontStyleMap_.insert(fontIDLight, fontLight);
    }

    fontDir = QCoreApplication::applicationDirPath() + "/font/ALIBABA-PUHUITI-MEDIUM.TTF";
    int fontIDMedium = QFontDatabase::addApplicationFont(fontDir);
    if (fontIDMedium >= 0) {
        QStringList fontMedium = QFontDatabase::applicationFontFamilies(fontIDMedium);
        fontStyleMap_.insert(fontIDMedium, fontMedium);
    }

    fontDir = QCoreApplication::applicationDirPath() + "/font/ALIBABA-PUHUITI-HEAVY.TTF";
    int fontIDBold = QFontDatabase::addApplicationFont(fontDir);
    if (fontIDBold >= 0) {
        QStringList fontBold = QFontDatabase::applicationFontFamilies(fontIDBold);
        fontStyleMap_.insert(fontIDBold, fontBold);
    }
    return fontStyleMap_.size();
}

/**************************************************************************
 * 名称:  SetLabelStyle
 * 功能:  设置Label显示样式
 * 输入参数:  fontStyle：0：regular；1：light；2：medium；fontSize：字体大小；
 *          fontColor：字体颜色；labelVct：label组
 * 输出参数:NULL
 * 返回值:  BNE_APP_SUCCESS：设置成功；BNE_APP_FONTERROR：设置失败
 * 创建者:  zhangzy
 * 创建时间: 2023-09-26
 ***************************************************************************/
BNEStatus Ui::CustomF::CustomFont::SetLabelStyle(AppEnum::FontTypeEnum fontStyle, int fontSize, QColor fontColor,
                                                 const QVector<QLabel *> labelVct)
{
    if (fontStyleMap_.isEmpty()) {
        qDebug() << "tty字体文件加载失败";
        return BNE_APP_FONTERROR;
    }

    QFont font;
    if (fontStyle == AppEnum::BNE_APP_FONT_REGULAR) { // 设置regular字体Label数组样式
        if (fontStyleMap_.value(fontStyle).isEmpty()) {
            qDebug() << "ttyRegular字体文件加载失败";
            return BNE_APP_FONTERROR;
        }
        font.setFamily(fontStyleMap_.value(fontStyle).first()); // 设置字体
        font.setWeight(QFont::Normal);
    } else if (fontStyle == AppEnum::BNE_APP_FONT_LIGHT) { // 设置light字体Label数组样式
        if (fontStyleMap_.value(fontStyle).isEmpty()) {
            qDebug() << "ttyLight字体文件加载失败";
            return BNE_APP_FONTERROR;
        }
        font.setFamily(fontStyleMap_.value(fontStyle).first()); // 设置字体
        font.setWeight(QFont::Light);
    } else if (fontStyle == AppEnum::BNE_APP_FONT_MEDIUM) { // 设置medium字体Label数组样式
        if (fontStyleMap_.value(fontStyle).isEmpty()) {
            qDebug() << "ttyMedium字体文件加载失败";
            return BNE_APP_FONTERROR;
        }
        font.setFamily(fontStyleMap_.value(fontStyle).first()); // 设置字体
        font.setWeight(QFont::Medium);
    }
    font.setPixelSize(fontSize); // 设置字体大小

    for (int i = 0; i < labelVct.count(); ++i) {
        labelVct.at(i)->setStyleSheet(QString("color:rgba(%1,%2,%3,1)")
                                          .arg(fontColor.red())
                                          .arg(fontColor.green())
                                          .arg(fontColor.blue())); // 设置字体颜色
        labelVct.at(i)->setFont(font);
    }

    return BNE_APP_SUCCESS;
}

BNEStatus Ui::CustomF::CustomFont::SetEditStyle(AppEnum::FontTypeEnum fontStyle, int fontSize, QColor fontColor,
                                                const QVector<QLineEdit *> editVct)
{
    if (fontStyleMap_.isEmpty()) {
        qDebug() << "tty字体文件加载失败";
        return BNE_APP_FONTERROR;
    }

    QFont font;
    if (fontStyle == AppEnum::BNE_APP_FONT_REGULAR) { // 设置regular字体Label数组样式
        if (fontStyleMap_.value(fontStyle).isEmpty()) {
            qDebug() << "ttyRegular字体文件加载失败";
            return BNE_APP_FONTERROR;
        }
        font.setFamily(fontStyleMap_.value(fontStyle).first()); // 设置字体
        font.setWeight(QFont::Normal);
    } else if (fontStyle == AppEnum::BNE_APP_FONT_LIGHT) { // 设置light字体Label数组样式
        if (fontStyleMap_.value(fontStyle).isEmpty()) {
            qDebug() << "ttyLight字体文件加载失败";
            return BNE_APP_FONTERROR;
        }
        font.setFamily(fontStyleMap_.value(fontStyle).first()); // 设置字体
        font.setWeight(QFont::Light);
    } else if (fontStyle == AppEnum::BNE_APP_FONT_MEDIUM) { // 设置medium字体Label数组样式
        if (fontStyleMap_.value(fontStyle).isEmpty()) {
            qDebug() << "ttyMedium字体文件加载失败";
            return BNE_APP_FONTERROR;
        }
        font.setFamily(fontStyleMap_.value(fontStyle).first()); // 设置字体
        font.setWeight(QFont::Medium);
    }
    font.setPixelSize(fontSize); // 设置字体大小

    for (int i = 0; i < editVct.count(); ++i) {
        editVct.at(i)->setStyleSheet(QString("color:rgba(%1,%2,%3,1)")
                                         .arg(fontColor.red())
                                         .arg(fontColor.green())
                                         .arg(fontColor.blue())); // 设置字体颜色
        editVct.at(i)->setFont(font);
    }

    return BNE_APP_SUCCESS;
}

/**************************************************************************
 * 名称:  SetButtonStyle
 * 功能:  设置按钮组字体样式
 * 输入参数:  fontStyle：0：regular；1：light；2：medium；buttonVct：按钮组
 * 输出参数:NULL
 * 返回值:  BNE_APP_SUCCESS：设置成功；BNE_APP_FONTERROR：设置失败
 * 创建者:  zhangzy
 * 创建时间:2023-09-28
 ***************************************************************************/
BNEStatus Ui::CustomF::CustomFont::SetButtonStyle(AppEnum::FontTypeEnum fontStyle,
                                                  const QVector<QPushButton *> buttonVct)
{
    if (fontStyleMap_.isEmpty()) {
        qDebug() << "tty字体文件加载失败";
        return BNE_APP_FONTERROR;
    }

    QFont font;
    if (fontStyle == AppEnum::BNE_APP_FONT_REGULAR) { // 设置regular字体
        if (fontStyleMap_.value(fontStyle).isEmpty()) {
            qDebug() << "ttyRegular字体文件加载失败";
            return BNE_APP_FONTERROR;
        }
        font.setFamily(fontStyleMap_.value(fontStyle).first()); // 设置字体
    } else if (fontStyle == AppEnum::BNE_APP_FONT_LIGHT) {      // 设置light字体
        if (fontStyleMap_.value(fontStyle).isEmpty()) {
            qDebug() << "ttyLight字体文件加载失败";
            return BNE_APP_FONTERROR;
        }
        font.setFamily(fontStyleMap_.value(fontStyle).first());
    } else if (fontStyle == AppEnum::BNE_APP_FONT_MEDIUM) { // 设置medium字体
        if (fontStyleMap_.value(fontStyle).isEmpty()) {
            qDebug() << "ttyMedium字体文件加载失败";
            return BNE_APP_FONTERROR;
        }
        font.setFamily(fontStyleMap_.value(fontStyle).first());
    } else if (fontStyle == AppEnum::BNE_APP_FONT_BOLD) { // 设置bold字体
        if (fontStyleMap_.value(fontStyle).isEmpty()) {
            qDebug() << "ttyBold字体文件加载失败";
            return BNE_APP_FONTERROR;
        }
        font.setFamily(fontStyleMap_.value(fontStyle).first());
    }

    font.setPixelSize(AppEnum::BNE_APP_FONTSIZE_LARGE);
    font.setWeight(QFont::DemiBold);
    for (int i = 0; i < buttonVct.count(); ++i) {
        buttonVct.at(i)->setFont(font);
    }
    return BNE_APP_SUCCESS;
}

BNEStatus Ui::CustomF::CustomFont::GetFontStyleMap(QMap<int, QStringList> &map)
{
    if (map.size() > 0) {
        map.clear();
    }
    map = fontStyleMap_;
    return BNE_APP_SUCCESS;
}
