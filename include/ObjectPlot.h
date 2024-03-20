/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-19 13:33:47
 * @FilePath: /cplusplus/submodule/data_plot/include/ObjectPlot.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <map>
#include <unordered_map>

#include "App.h"
#include "ObjectData.h"

struct ObjectPlot : App {
    enum { FRONT_CAMERA, FRONT_RADAR, FUSION, MAX };
    enum { FRONT, SIDE, ALWAYS, MAX_PLOT };
    struct PlotIndex {
        char name[64]{};
        int id{-1};
    };
    using App::App;

    ObjectPlot(std::string title, int w, int h);

    ObjectPlot(const ObjectPlot&) = delete;

    void operator=(const ObjectPlot&) = delete;

    static void startPLot();

    void Update() override;

    void dragAndDropPlot();

    std::vector<std::pair<int, std::string>> dnd_name{
        {FRONT, "DND_FRONT_CAMERA"}, {FRONT, "DND_FRONT_RADAR"}, {ALWAYS, "DND_FUSION"}};

    int count{0};

    std::unordered_map<std::string, std::map<int, ObjectData>> objs;

    double time{INT_MAX};

    static std::unique_ptr<ObjectPlot> objPlotPtr;
};
