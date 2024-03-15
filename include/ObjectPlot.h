/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-15 17:08:39
 * @FilePath: /cplusplus/submodule/data_plot/include/ObjectPlot.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <map>

#include "App.h"
#include "ObjectData.h"

struct ObjectPlot : App {
    using App::App;
    ObjectPlot(std::string title, int w, int h);
    void Update() override;
    void dragAndDropPlot();
    std::map<size_t, ObjectData> objs;
};
