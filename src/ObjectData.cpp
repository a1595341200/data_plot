/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:11:33
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-17 23:44:08
 * @FilePath: /cplusplus/submodule/data_plot/src/ObjectData.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "ObjectData.h"
#include <iostream>

ObjectData::ObjectData() {
    static int i = 0;
    id = i++;
    label = std::to_string(id) + "HZ";
    data.reserve(maxCount);
    for (int k = 0; k < maxCount; ++k) {
        float t = k * 1.0f / 999;
        data.push_back(ImVec2(t, 0.5f + 0.5f * sinf(2 * 3.14f * t * (id + 1))));
    }
}
ObjectData::ObjectData(int _id, const std::string& _label, int _maxCount)
    : id(_id), label(_label), maxCount(_maxCount) {
    data.reserve(maxCount);
}
void ObjectData::reset() {
    subPlotId = 0;
    data.clear();
    data.reserve(maxCount);
}
