/**************************************************************************
 * Copyright(C),2024,BNE
 * 模块名称:    自定义validator
 * 模块功能:    自定义QIntValidator和QDoubleValidator，用于进行用户输入范围限制
 * 创建者:      yexin
 * 创建日期:    2024-10-26
 ***************************************************************************/
#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <QValidator>

namespace BNE {
namespace UI {
namespace CustomValidator {

class CustomIntValidator : public QIntValidator {
    Q_OBJECT
public:
    explicit CustomIntValidator(QObject *parent = nullptr);
    CustomIntValidator(int bottom, int top, QObject *parent);  // 构造函数
    void setRange(int bottom, int top) override;               // 设置限制范围
    virtual State validate(QString &s, int &n) const override; // 每次输入会调用此函数进行合法性判断
    virtual void fixup(QString &input) const override;         // 丢失焦点时，状态不为accept时调用

private:
    int bottomDigits;          // 最小值位数
    int topDigits;             // 最大值位数
    QString bottomStr;         // 最小值的字符串映射
    QString topStr;            // 最大值的字符串映射
    mutable bool flag = false; // 是否修正
};

class CustomDoubleValidator : public QDoubleValidator {
    Q_OBJECT

public:
    explicit CustomDoubleValidator(QObject *parent = nullptr);
    CustomDoubleValidator(double bottom, double top, int decimals = 0, QObject *parent = nullptr); // 构造函数
    void setRange(double bottom, double top, int decimals = 0) override;                           // 设置限制范围
    virtual State validate(QString &s, int &n) const override; // 每次输入会调用此函数进行合法性判断
    virtual void fixup(QString &input) const override;         // 丢失焦点时，状态不为accept时调用
    ~CustomDoubleValidator();

private:
    CustomIntValidator integerValidator;

    int integerTop;    // 最大值的整数部分
    int integerBottom; // 最小值的整数部分

    QString decimalTopStr;     // 最大值小数部分的字符串映射
    QString decimalBottomStr;  // 最小值小数部分的字符串映射
    mutable bool flag = false; // 是否修正
};

} // namespace CustomValidator
} // namespace UI
} // namespace BNE
#endif // VALIDATOR_H
