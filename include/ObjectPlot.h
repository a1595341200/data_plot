/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-11-08 14:11:24
 * @FilePath: /cplusplus/submodule/data_plot/include/ObjectPlot.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <functional>
#include <map>
#include <unordered_map>

#include "App.h"
#include "DataCenter.h"
#include "DndName.h"
#include "ObjectData.h"
#include "SensorType.h"

struct ObjPlotSelect {
    std::string yLabel;
    bool show;
};

struct ObjectPlot : App {
    uint64_t name{};
    int id{-1};
    using App::App;

    ObjectPlot(std::string title, int w, int h);

    ObjectPlot(const ObjectPlot&) = delete;

    void operator=(const ObjectPlot&) = delete;

    static void startPLot();

    void Update() override;
    void Init();
    void dragAndDropPlot(bool* open);
    void dragClassProb(bool* open);
    void dragBev(bool* open);
    template <typename T>
    void plot(
        T* front_cam_obj, T* front_radar_obj, const T& output, T* const side_cam_obj,
        T* fl_radar_obj, T* fr_rdara_obj, T* rl_radar_obj, T* rr_rdara_obj);

    void plotSelf();

    void plotRect(
        float x, float y, float heading, float length, float width, float height, int nearSide);

    std::shared_ptr<std::unordered_map<uint64_t, std::map<int, ObjectData>>> objs;

    std::shared_ptr<uint64_t> frameCount;

    static std::unique_ptr<ObjectPlot> objPlotPtr;

    double time{};

    std::function<void(std::function<void()> func)> basePlot;

    std::unordered_map<uint64_t, DndName> mDndNameMap{
        {SensorType::FRONT_CAMERA, {"DND_FRONT_CAMERA", "camera", false}},
        {SensorType::FRONT_RADAR, {"DND_FRONT_RADAR", "radar", false}},
        {SensorType::SIDE_CAMERA, {"DND_SIDE_CAMERA", "side_camera", false}},
        {SensorType::FL_RADAR, {"DND_FL_RADAR", "fl_radar", false}},
        {SensorType::FR_RADAR, {"DND_FR_RADAR", "fr_radar", false}},
        {SensorType::RL_RADAR, {"DND_RL_RADAR", "rl_radar", false}},
        {SensorType::RR_RADAR, {"DND_RR_RADAR", "rr_radar", false}},
        {SensorType::FUSION, {"DND_FUSION", "fusion", true}}};
    std::unordered_map<uint64_t, ObjPlotSelect> mObjPlotMap;
    std::shared_ptr<DataCenter> mDataCenter{std::make_shared<DataCenter>()};
};
