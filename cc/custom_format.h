/**************************************************************************
 * Copyright(C),2024,BNE
 * 模块名称:    自定义格式化字符串工具
 * 模块功能:    实现类似python的字符串格式化功能
 * 创建者:      yexin
 * 创建日期:    2024-12-30
 ***************************************************************************/
#ifndef CUSTOM_FORMAT_H
#define CUSTOM_FORMAT_H

#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <QString>
#include <regex>

namespace BNE {
namespace UI {
namespace Format {

inline std::ostream &operator<<(std::ostream &os, const QString &qs)
{
    os << qs.toStdString();
    return os;
}

class ArgBase {
public:
    ArgBase() { }
    virtual ~ArgBase() { }
    virtual void Format(std::ostringstream &ss, const std::string &fmt) = 0;
};

template <class T> class Arg : public ArgBase {
public:
    Arg(T arg)
        : m_arg(arg)
    {
    }
    virtual ~Arg() { }
    // 将参数写入输出流中
    virtual void Format(std::ostringstream &ss, const std::string &fmt)
    {
        if constexpr (std::is_same<T, double &>::value || std::is_same<T, float &>::value) { // 参数类型是double,float

            std::regex format_regex(R"(\.(\d+)f)"); // 正则匹配形如“.2f”字符串
            std::smatch match;

            if (std::regex_match(fmt, match, format_regex)) {
                int precision = std::stoi(match[1]); // 获取匹配到的精度
                ss << std::fixed << std::setprecision(precision) << m_arg << std::defaultfloat;
            } else {
                ss << m_arg;
            }
        } else { // 其他
            ss << m_arg;
        }
    }

private:
    T m_arg; // 需要写入字符串的参数
};

class ArgArray : public std::vector<ArgBase *> {
public:
    ArgArray() { }
    ~ArgArray()
    {
        std::for_each(begin(), end(), [](ArgBase *p) { delete p; });
    }
};

/**************************************************************************
 * 名称:  FormatItem
 * 功能:  将单个参数写入输出流中
 * 输入参数:  ss：输出流，item：输入的参数索引，args：存储所有参数的vector
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   yexin
 * 创建时间: 2024.12.30
 ***************************************************************************/
static void FormatItem(std::ostringstream &ss, const std::string &item, size_t &defaultIndex, const ArgArray &args)
{
    std::string indexStr;        // 索引字符串
    std::string fmt;             // 格式化部分 字符串
    size_t pos = item.find(':'); // split
    if (pos != -1) {
        indexStr = item.substr(0, pos);
        fmt = item.substr(pos + 1, item.length() - pos);
    } else {
        indexStr = item;
        fmt = "";
    }
    // 不输入索引默认从0开始自增
    if (indexStr.empty()) {
        args[defaultIndex++]->Format(ss, fmt);
        return;
    }
    int index = 0; // 参数下标
    char *endptr = nullptr;
    index = strtol(&indexStr[0], &endptr, 10);
    if (index < 0 || index >= args.size()) {
        return;
    }

    args[index]->Format(ss, fmt); // 将对应参数添加到输出流中
    return;
}

/**************************************************************************
 * 名称:  Format
 * 功能:  字符串格式化输出
 * 输入参数:  format：需要输出的字符串，args：格式化参数列表
 * 输出参数:  NULL
 * 返回值:    void
 * 创建者:   yexin
 * 创建时间: 2024.12.30
 ***************************************************************************/
template <typename... Args> std::string Format(const std::string &format, Args &&...args)
{
    if (sizeof...(args) == 0) { // 没有参数
        return format;
    }

    // ArgArray argArray{std::forward<Args>(args)...};
    ArgArray argArray;
    (argArray.push_back(new Arg<Args>(std::forward<Args>(args))), ...); // 将各参数存入vector中

    size_t start = 0;      // 开始位置
    size_t pos = 0;        // 记录每次查找到‘{’与‘}’的位置
    std::ostringstream ss; // 最终的输出流
    size_t defaultIndex = 0;
    while (true) {
        pos = format.find('{', start);
        if (pos == std::string::npos) { // 未找到
            ss << format.substr(start);
            break;
        }

        ss << format.substr(start, pos - start); // 将‘{’前字符串输出到ss中

        start = pos + 1;
        pos = format.find('}', start); // 寻找‘}’位置
        if (pos == std::string::npos) {
            ss << format.substr(start - 1);
            break;
        }

        FormatItem(ss, format.substr(start, pos - start), defaultIndex, argArray); // 根据{}中的索引值写入参数
        start = pos + 1;
    }

    return ss.str();
}
} // namespace Format
} // namespace UI
} // namespace BNE

#endif
