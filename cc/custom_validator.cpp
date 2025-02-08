#include "custom_validator.h"
#include <QDebug>
#include <QtMath>

namespace BNE {
namespace UI {
namespace CustomValidator {
/**************************************************************************
 * 名称: CustomIntValidator
 * 功能:  构造函数
 * 输入参数: parent：父对象指针
 * 输出参数:  NULL
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
CustomIntValidator::CustomIntValidator(QObject *parent)
    : QIntValidator{parent}
{
}

/**************************************************************************
 * 名称: CustomIntValidator
 * 功能:  有参构造函数
 * 输入参数: bottom：最小值，top：最大值，parent：父对象指针
 * 输出参数:  NULL
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
CustomIntValidator::CustomIntValidator(int bottom, int top, QObject *parent)
    : QIntValidator{parent}
{
    CustomIntValidator::setRange(bottom, top);
}

/**************************************************************************
 * 名称: setRange
 * 功能:  设置validator限制范围
 * 输入参数: bottom：最小值，top：最大值
 * 输出参数:  NULL
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
void CustomIntValidator::setRange(int bottom, int top)
{
    if (bottom > top)
        bottom = top;
    // 获取最小值和最大值的位数

    bottomStr = QString::number(bottom);
    topStr = QString::number(top);
    bottomDigits = bottomStr.count();
    topDigits = topStr.count();
    QIntValidator::setRange(bottom, top);
}

/**************************************************************************
 * 名称: validate
 * 功能:  判断用户输入是否合法
 * 输入参数: s：用户输入，n：当前输入的位数
 * 输出参数:  输入是否合法的状态值
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
QValidator::State CustomIntValidator::validate(QString &s, int &n) const
{
    if (s.isEmpty())
        return QValidator::Intermediate;
    if (flag) { // 经过修正的值直接accept
        flag = false;
        return QValidator::Acceptable;
    }
    bool isInt = false;
    s = QString::number(s.toInt(&isInt));
    if (!isInt) // 非法字符会转换失败
        return QValidator::Invalid;
    // 输入位数超出了最大值位数
    if (s.count() > topDigits)
        return QValidator::Invalid;
    // 最大值位数比最小值位数大超过一位
    if (topDigits - bottomDigits > 1) {
        if (s.count() == topDigits) {            // 输入的是最后一位
            if (QString::compare(s, topStr) > 0) // 比最大值大
                return QValidator::Invalid;
            else
                return QValidator::Acceptable; // 焦点离开时，fix函数会处理比最小值小的情况
        } else
            return QValidator::Intermediate; // 其他情况允许输入
    } else {
        QString bStr(bottomDigits, '9');
        QString tStr(topDigits, '0');
        for (int i = 0; i < s.count(); i++) {
            bStr[i] = s[i];
            tStr[i] = s[i];
        }
        qDebug() << "bot  :::" << bStr;
        qDebug() << "top :::" << tStr;
        // 最大值和最小值位数相同时
        if (topDigits == bottomDigits) {
            // 使用compare函数时为了规避tStr数值超过int的最大值的风险，后续同理
            if (QString::compare(bStr, bottomStr) >= 0 && QString::compare(tStr, topStr) <= 0)
                return QValidator::Intermediate;
            else
                return QValidator::Invalid;
        } else {
            // 最大值位数比最小值位数多一位
            if (s.count() == topDigits) {
                if (QString::compare(tStr, topStr) > 0) // 不超过最大值
                    return QValidator::Invalid;
                else
                    return QValidator::Acceptable;
            }
            // 无法判断此次输入是位数为最小值位数的数，还是作为位数为最大值位数的数的一部分，因此使用或
            if (QString::compare(bStr, bottomStr) >= 0 || QString::compare(tStr, topStr) <= 0)
                return QValidator::Intermediate;
            else
                return QValidator::Invalid;
        }
    }
}

/**************************************************************************
 * 名称: fixxup
 * 功能:  修正函数，在丢失焦点时，状态为Intermediate时调用
 * 输入参数: input：用户输入的值
 * 输出参数:  NULL
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
void CustomIntValidator::fixup(QString &input) const
{
    // 修正完后，仍需调用一次validate函数
    flag = true;
    if (input.isEmpty()) {
        input = QString::number(bottom());
        return;
    }
    int intValue = input.toInt();
    if (intValue < bottom()) // 小于最小值时修正为最小值
        input = bottomStr;
    else if (intValue > top())
        input = topStr;
}

// double
/**************************************************************************
 * 名称: CustomDoubleValidator
 * 功能:  构造函数
 * 输入参数: parent：父对象指针
 * 输出参数:  NULL
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
CustomDoubleValidator::CustomDoubleValidator(QObject *parent)
    : QDoubleValidator{parent}
{
}

/**************************************************************************
 * 名称: CustomDoubleValidator
 * 功能:  有参构造函数
 * 输入参数: bottom：最小值，top：最大值，decimals：小数位数，parent：父对象指针
 * 输出参数:  NULL
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
CustomDoubleValidator::CustomDoubleValidator(double bottom, double top, int decimals, QObject *parent)
    : QDoubleValidator{parent}
{
    CustomDoubleValidator::setRange(bottom, top, decimals);
}

/**************************************************************************
 * 名称: ~CustomDoubleValidator
 * 功能:  析构函数
 * 输入参数: NULL
 * 输出参数:  NULL
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
CustomDoubleValidator::~CustomDoubleValidator() { }

/**************************************************************************
 * 名称: setRange
 * 功能:  设置validator限制范围
 * 输入参数: bottom：最小值，top：最大值，decimals：小数位数
 * 输出参数:  NULL
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
void CustomDoubleValidator::setRange(double bottom, double top, int decimals)
{
    decimalBottomStr = QString(decimals, '0');
    decimalTopStr = QString(decimals, '0');

    if (top - bottom < 0)
        bottom = top;

    QStringList strList = QString::number(bottom).split('.');
    integerBottom = strList[0].toInt();
    if (strList.count() > 1) // 位数不足和位数过多时要进行补0或截取
        decimalBottomStr.replace(0, strList[1].count(), strList[1]);

    strList = QString::number(top).split('.');
    integerTop = strList[0].toInt();
    if (strList.count() > 1)
        decimalTopStr.replace(0, strList[1].count(), strList[1]);

    // qDebug() << "bbbb::" << decimalBottomStr;
    // qDebug() << "ttt::" << decimalTopStr;

    integerValidator.setRange(integerBottom, integerTop); // 设置整数validator
    QDoubleValidator::setRange(bottom, top, decimals);
}

/**************************************************************************
 * 名称: validate
 * 功能:  判断用户输入的合法性
 * 输入参数: s：用户输入，n：当前输入位数
 * 输出参数:  NULL
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
QValidator::State CustomDoubleValidator::validate(QString &s, int &n) const
{
    if (s.isEmpty()) {
        return QValidator::Intermediate;
    }
    if (flag) { // 修正值直接accept
        flag = false;
        return QValidator::Acceptable;
    }

    int dotPos = s.indexOf(".");
    if (s.indexOf(".", dotPos + 1) != -1) // 不允许输入两个‘.’
        return QValidator::Invalid;
    if (dotPos > 0) // 当前输入的是小数部分
    {
        QString decStr = s.right(s.count() - dotPos - 1); // 当前输入的小数部分字符串
        int intNum = s.leftRef(dotPos).toInt();           // 当前输入的整数部分数值
        // qDebug() << "decstr::::::::" << decStr << "s   ::::" << s;

        if (s.back() == '.') { // 用户输入的是‘.’
            if (intNum < integerBottom || intNum > integerTop) {
                return QValidator::Invalid; // 整数部分不满足要求，不允许输入‘.’
            } else {
                return QValidator::Intermediate;
            }
        } else { // 用户输入的是小数部分
            // qDebug() << "decstr  count::::::::" << decStr.count();
            if (decStr.count() > decimals()) // 输入超过了限制的小数位数，不允许输入
                return QValidator::Invalid;
            if (integerBottom == integerTop) { // 限制范围的最小值和最大值的整数数值相同时
                QString botStr(decimals(), '9');
                QString topStr(decimals(), '0');

                for (int i = 0; i < decStr.count(); i++) {
                    botStr[i] = decStr[i];
                    topStr[i] = decStr[i];
                }
                // 整数部分相同，只需要比较输入值在最小值的小数部分和最大值的小数部分之间即可
                if (QString::compare(botStr, decimalBottomStr) >= 0 && QString::compare(topStr, decimalTopStr) <= 0)
                    return QValidator::Intermediate;
                else
                    return QValidator::Invalid;

            } else {                           // 最小值和最大值的整数部分不同
                if (intNum == integerBottom) { // 用户输入的整数部分数值等于最小值的整数部分时
                    QString tmpStr(decimals(), '9');
                    for (int i = 0; i < decStr.count(); i++)
                        tmpStr[i] = decStr[i];
                    // qDebug() << "tmp111:" << tmpStr << "dec:111" << decStr;
                    if (QString::compare(tmpStr, decimalBottomStr) >= 0) // 只需要判断用户输入的值大于最小值小数部分
                        return QValidator::Intermediate;
                    else
                        return QValidator::Invalid;
                } else if (intNum == integerTop) { // 用户输入的整数部分数值等于最大值的整数部分时
                    QString tmpStr(decimals(), '0');
                    for (int i = 0; i < decStr.count(); i++)
                        tmpStr[i] = decStr[i];
                    // qDebug() << "tmp222:" << tmpStr << "dec:222" << decStr;
                    if (QString::compare(tmpStr, decimalTopStr) <= 0) // 只需判断用户输入的值小于最大值小数部分
                        return QValidator::Intermediate;
                    else
                        return QValidator::Invalid;
                } else { // 用户输入的整数部分数值在最小值和最大值之间，则小数部分可以随意输入，不超过位数限制即可
                    return QValidator::Intermediate;
                }
            }
        }
    } else {                                    // 当前输入的是整数部分
        return integerValidator.validate(s, n); // 返回整数validator的判断状态
    }
}

/**************************************************************************
 * 名称: fixup
 * 功能:  修正函数，焦点丢失时，修正用户输入
 * 输入参数: input：用户输入
 * 输出参数:  NULL
 * 返回值:   BNEStatus
 * 创建者:   yexin
 * 创建时间: 2024-11-06
 ***************************************************************************/
void CustomDoubleValidator::fixup(QString &input) const
{
    flag = true;
    double value = input.toDouble();
    if (value < bottom())
        input = QString::number(bottom()); // 比最小值小，修正为最小值
    else if (value > top())
        input = QString::number(top()); // 比最大值大，修正为最大值
    else if (input.indexOf(".") > 0) {
        QStringList strList = input.split('.');
        int decCount = decimals() - strList[1].count();
        if (decCount > 0) // 位数不足时，末尾补0
            input.append(QString(decCount, '0'));
    }
}
} // namespace CustomValidator
} // namespace UI
} // namespace BNE
