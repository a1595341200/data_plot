/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-29 15:11:56
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
    void dragClassProb();
    void dragBev();
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

    void plotRect(float x, float y, float heading, float length, float width, float height, int nearSide);

     std::vector<ImVec2> rotatedRect(
        double x, double y, double half_length, double half_width, double angle,
        uint8_t nearest_side = 0);

    std::vector<std::pair<int, std::string>> dnd_name{
        {FRONT, "DND_FRONT_CAMERA"}, {FRONT, "DND_FRONT_RADAR"}, {SIDE, "DND_SIDE_CAMERA"},
        {SIDE, "DND_FL_RADAR"},      {SIDE, "DND_FR_RADAR"},     {SIDE, "DND_RL_RADAR"},
        {SIDE, "DND_RR_RADAR"},      {ALWAYS, "DND_FUSION"}};

    int count{0};

    std::unordered_map<std::string, std::map<int, ObjectData>> objs;

    double time{INT_MAX};

    static std::unique_ptr<ObjectPlot> objPlotPtr;
    std::unordered_map<std::string, std::string> dndNameToRealName{
        {"DND_FRONT_CAMERA", "camera"}, {"DND_FRONT_RADAR", "radar"},
        {"DND_FUSION", "fusion"},       {"DND_SIDE_CAMERA", "side_camera"},
        {"DND_FL_RADAR", "fl_radar"},   {"DND_FR_RADAR", "fr_radar"},
        {"DND_RL_RADAR", "rl_radar"},   {"DND_RR_RADAR", "rr_radar"}};
    std::function<void(std::function<void()> func)> basePlot;
};
