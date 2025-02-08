/**************************************************************************
 * Copyright(C),2023,BNE
 * 模块名称:    设置字体样式类
 * 模块功能:    提供静态方法供各子界面设置字体样式
 * 创建者:      zhangzy
 * 创建日期:    2023-10-11
 ***************************************************************************/
#ifndef CUSTOMFONT_H_
#define CUSTOMFONT_H_

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QColor>
#include <QVector>
#include <QMap>
#include "include/bne_app_error.h"
#include "include/bne_app_enum.h"
#include "include/bne_app_define.h"

namespace Ui {
namespace CustomF {
class CustomFont {
public:
    explicit CustomFont();
    CustomFont(const CustomFont &) = delete;             // 不合成拷贝构造函数
    CustomFont &operator=(const CustomFont &) = delete;  // 不合成拷贝赋值运算符
    CustomFont(const CustomFont &&) = delete;            // 不合成移动构造函数
    CustomFont &operator=(const CustomFont &&) = delete; // 合成移动赋值运算符
    ~CustomFont();

public:
    static int InsertFontStyleMap(); // 读取字体文件，插入Map
    static BNEStatus SetLabelStyle(AppEnum::FontTypeEnum fontStyle, int fontSize, QColor fontColor,
                                   const QVector<QLabel *> labelVct); // 统一设置Label不同显示样式
    static BNEStatus SetEditStyle(AppEnum::FontTypeEnum fontStyle, int fontSize, QColor fontColor,
                                  const QVector<QLineEdit *> editVct); // 统一设置lineEdit不同显示样式
    static BNEStatus SetButtonStyle(AppEnum::FontTypeEnum fontStyle,
                                    const QVector<QPushButton *> buttonVct); // 设置按钮组字体及样式
    static BNEStatus GetFontStyleMap(QMap<int, QStringList> &map);           // 获取字体风格Map

private:
    static QMap<int, QStringList> fontStyleMap_; // 存储alibaba惠普体REGULAR，LIGHT，MEDIUM，BOLD，HEAVY风格
};
} // namespace CustomF
} // namespace Ui

#endif // CUSTOMFONT_H
