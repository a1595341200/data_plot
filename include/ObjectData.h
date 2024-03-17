/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:11:33
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-17 23:00:12
 * @FilePath: /cplusplus/submodule/data_plot/include/ObjectData.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include "implot.h"
#include <string>
#include "Helpers.h"

class ObjectData {
public:
    int id{};
    int subPlotId{-1};
    int maxCount{1001};
    std::string label;
    ImVector<ImVec2> data;
    ImVec4 color{RandomColor()};
    ObjectData();
    ObjectData(int _id, const std::string& _label, int _maxCount = 100);
    void reset();
};
