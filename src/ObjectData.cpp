/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:11:33
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-19 13:11:27
 * @FilePath: /cplusplus/submodule/data_plot/src/ObjectData.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "ObjectData.h"

#include <iostream>

ObjectData::ObjectData() {
    data.resize(MAX);
}
ObjectData::ObjectData(int _id, const std::string& _label, int _maxCount)
    : id(_id), label(_label), maxCount(_maxCount) {
    data.resize(MAX);
}
void ObjectData::reset() {
    subPlotId = -1;
    data.clear();
    data.resize(MAX);
}
