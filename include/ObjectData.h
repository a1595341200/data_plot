/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:11:33
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-06-30 21:50:19
 * @FilePath: /cplusplus/submodule/data_plot/include/ObjectData.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "Helpers.h"
#include "implot.h"

class ObjectData {
public:
    enum : uint64_t { X, Y, VX, VY, CLASSPROB, RADAR_ID, CAMERA_ID, MAX };

    int id{};
    int subPlotId{-1};
    std::string label;
    int maxCount{1001};
    std::vector<std::vector<ImVec2>> data;
    ImVec4 color{RandomColor()};
    ObjectData();
    ObjectData(int _id, const std::string& _label, int _maxCount = 100);
    void reset();
    inline static std::unordered_map<uint64_t, std::string> id2yLabel{
        {X, "x"},
        {Y, "y"},
        {VX, "vx"},
        {VY, "vy"},
        {CLASSPROB, "class prob"},
        {RADAR_ID, "radar id"},
        {CAMERA_ID, "camera id"}};
};
