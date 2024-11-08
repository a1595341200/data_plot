/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-11-08 13:29:57
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-11-08 13:38:49
 * @FilePath: /cplusplus/submodule/data_plot/common/Utils.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef UTILS_H
#define UTILS_H
#include <imgui.h>

#include <cstdint>
#include <vector>

namespace Utils {
using Point2D = ImVec2;
std::vector<Point2D> rotatedRect(
    double x, double y, double half_length, double half_width, double angle,
    uint8_t nearest_side = 0);

template <typename T>
auto convertEnumToVal(T&& t) -> std::underlying_type_t<T> {
    return static_cast<std::underlying_type_t<T>>(t);
}
}  // namespace Utils
#endif /* UTILS_H */
