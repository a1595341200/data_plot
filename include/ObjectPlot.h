/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-04-02 14:54:06
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
#include "ObjectData.h"

struct DndName {
    std::string dndName;
    std::string realName;
    bool show{};
};

struct ObjPlotSelect {
    std::string yLabel;
    bool show;
};

struct ObjectPlot : App {
    enum {
        FRONT_CAMERA,
        FRONT_RADAR,
        FUSION,
        EGOMOTION,
        FL_RADAR,
        FR_RADAR,
        RL_RADAR,
        RR_RADAR,
        SIDE_CAMERA,
        MAX
    };
    enum { FRONT, SIDE, ALWAYS, MAX_PLOT };
    struct PlotIndex {
        uint64_t name{};
        int id{-1};
    };
    using App::App;

    ObjectPlot(std::string title, int w, int h);

    ObjectPlot(const ObjectPlot&) = delete;

    void operator=(const ObjectPlot&) = delete;

    static void startPLot();

    void Update() override;
    void init();
    void dragAndDropPlot(bool* open);
    void dragClassProb(bool* open);
    void dragBev(bool* open);
    template <typename T>
    void plot(
        T* front_cam_obj, T* front_radar_obj, const T& output, T* const side_cam_obj,
        T* fl_radar_obj, T* fr_rdara_obj, T* rl_radar_obj, T* rr_rdara_obj);
    template <typename T>
    void convertData(
        std::unordered_map<std::string, std::map<int, ObjectData>>& objs, const std::string& name,
        T* objList);

    template <typename T>
    void addCamera(T* front_cam_obj);

    template <typename T>
    void addRadar(T* front_radar_obj);

    template <typename T>
    void addFusion(const T& output);

    template <typename T>
    void addSideCamera(T* side_cam_obj);

    template <typename T>
    void addFl_radar(T* fl_radar_obj);

    template <typename T>
    void addfr_radar(T* fr_rdara_obj);

    template <typename T>
    void addrl_radar(T* rl_radar_obj);

    template <typename T>
    void addrr_radar(T* rr_rdara_obj);

    void plotSelf();

    void plotRect(
        float x, float y, float heading, float length, float width, float height, int nearSide);

    std::vector<ImVec2> rotatedRect(
        double x, double y, double half_length, double half_width, double angle,
        uint8_t nearest_side = 0);

    int count{0};

    std::unordered_map<uint64_t, std::map<int, ObjectData>> objs;

    double time{INT_MAX};

    static std::unique_ptr<ObjectPlot> objPlotPtr;
    std::function<void(std::function<void()> func)> basePlot;
    std::unordered_map<uint64_t, DndName> mDndNameMap{
        {FRONT_CAMERA, {"DND_FRONT_CAMERA", "camera", false}},
        {FRONT_RADAR, {"DND_FRONT_RADAR", "radar", false}},
        {SIDE_CAMERA, {"DND_SIDE_CAMERA", "side_camera", false}},
        {FL_RADAR, {"DND_FL_RADAR", "fl_radar", false}},
        {FR_RADAR, {"DND_FR_RADAR", "fr_radar", false}},
        {RL_RADAR, {"DND_RL_RADAR", "rl_radar", false}},
        {RR_RADAR, {"DND_RR_RADAR", "rr_radar", false}},
        {FUSION, {"DND_FUSION", "fusion", true}}};
    std::unordered_map<uint64_t, ObjPlotSelect> mObjPlotMap;
};
