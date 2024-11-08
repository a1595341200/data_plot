/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-11-08 13:27:21
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-11-08 13:32:05
 * @FilePath: /cplusplus/submodule/data_plot/common/ObjNearestSide.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef NEARSIDE_H
#define NEARSIDE_H

#include <cstdint>

// 枚举类，表示物体最近的一侧
enum class ObjNearestSide : uint8_t {
    // 未知的一侧
    UnknownSide = 0u,
    // 后侧
    RearSide,
    // 前侧
    FrontSide,
    // 右侧
    RightSide,
    // 左侧
    LeftSide,
    // 中心
    BoxCenter,
};

#endif /* NEARSIDE_H */
