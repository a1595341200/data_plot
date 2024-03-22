/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-22 17:15:23
 * @FilePath: /cplusplus/submodule/data_plot/src/ObjectPlot.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "ObjectPlot.h"

#include <thread>

ObjectPlot::ObjectPlot(std::string title, int w, int h) : App(title, w, h) {
    basePlot = [&](std::function<void()> func) {
        static int e = FRONT;
        ImGui::RadioButton("front", &e, FRONT);
        ImGui::SameLine();
        ImGui::RadioButton("side", &e, SIDE);
        if (ImGui::Button("Reset Data")) {
            objs.clear();
        }

        for (auto& sceneName : dnd_name) {
            int& scene = sceneName.first;
            std::string& name = sceneName.second;
            if (scene != e && scene != ALWAYS) {
                continue;
            }
            ImGui::BeginChild(name.c_str(), ImVec2(120, 1080));
            // ImGui::BeginChild(name.c_str());
            for (auto& obj : objs[name]) {
                if (obj.second.subPlotId != -1) {
                    continue;
                }
                ImPlot::ItemIcon(obj.second.color);
                ImGui::SameLine();
                ImGui::Selectable(obj.second.label.c_str(), false, 0, ImVec2(120, 0));
                // ImGui::Selectable(obj.second.label.c_str(), false, 0);
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                    PlotIndex index{"", obj.first};
                    strcpy(index.name, name.c_str());
                    ImGui::SetDragDropPayload("MY_DND", &index, sizeof(PlotIndex));
                    ImPlot::ItemIcon(obj.second.color);
                    ImGui::SameLine();
                    ImGui::TextUnformatted(obj.second.label.c_str());
                    ImGui::EndDragDropSource();
                }
            }

            ImGui::EndChild();
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
                    auto tmp = *(PlotIndex*)payload->Data;
                    objs[tmp.name][tmp.id].subPlotId = -1;
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::SameLine();
        }

        // ImGui::BeginChild("DND_RIGHT", ImVec2(-1, -1));
        ImGui::BeginChild("DND_RIGHT");

        func();

        ImGui::EndChild();
    };
}

std::unique_ptr<ObjectPlot> ObjectPlot::objPlotPtr = nullptr;

template <typename T>
void ObjectPlot::convertData(
    std::unordered_map<std::string, std::map<int, ObjectData>>& objs, const std::string& name,
    T* objList) {}

void ObjectPlot::startPLot() {
    std::thread t([&] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        objPlotPtr = std::make_unique<ObjectPlot>("Object Plot", 1920, 1080);
        objPlotPtr->Run();
    });
    t.detach();
}

void ObjectPlot::Update() {
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    if (ImGui::TreeNode("StateX")) {
        dragAndDropPlot();
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("ClassProb")) {
        dragClassProb();
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Bev")) {
        dragBev();
        ImGui::TreePop();
    }
}

void ObjectPlot::dragAndDropPlot() {
    basePlot([&]() {
        static bool show_x = true;
        static bool show_y = true;
        static bool show_vx = true;
        static bool show_vy = true;
        static bool show_radar_id = true;
        static bool show_camera_id = true;
        ImGui::SameLine();
        ImGui::Checkbox("show x", &show_x);
        ImGui::SameLine();
        ImGui::Checkbox("show y", &show_y);
        ImGui::SameLine();
        ImGui::Checkbox("show vx", &show_vx);
        ImGui::SameLine();
        ImGui::Checkbox("show vy", &show_vy);
        ImGui::SameLine();
        ImGui::Checkbox("show radar id", &show_radar_id);
        ImGui::SameLine();
        ImGui::Checkbox("show camera id", &show_camera_id);
        ImGui::SameLine();
        for (int i = 1; i <= 6; ++i) {
            switch (i) {
                case 1:
                    if (!show_x) {
                        continue;
                    };
                    break;
                case 2:
                    if (!show_y) {
                        continue;
                    };
                    break;
                case 3:
                    if (!show_vx) {
                        continue;
                    };
                    break;
                case 4:
                    if (!show_vy) {
                        continue;
                    };
                case 5:
                    if (!show_camera_id) {
                        continue;
                    };
                case 6:
                    if (!show_radar_id) {
                        continue;
                    };
                    break;
                default:
                    break;
            }
            ImGui::NewLine();
            // if (ImPlot::BeginPlot(("##DND" + std::to_string(i)).c_str(), ImVec2(-1, 200))) {
            if (ImPlot::BeginPlot(("##DND" + std::to_string(i)).c_str())) {
                static double min{INT_MAX};
                min = std::min(min, time);
                switch (i) {
                    case 1:
                        ImPlot::SetupAxes("time", "x");
                        break;
                    case 2:
                        ImPlot::SetupAxes("time", "y");
                        break;
                    case 3:
                        ImPlot::SetupAxes("time", "vx");
                        break;
                    case 4:
                        ImPlot::SetupAxes("time", "vy");
                        break;
                    case 5:
                        ImPlot::SetupAxes("time", "camera id");
                        break;
                    case 6:
                        ImPlot::SetupAxes("time", "radar id");
                        break;
                    default:
                        break;
                }
                for (auto& list : objs) {
                    for (auto& obj : list.second) {
                        if (obj.second.subPlotId != -1 &&
                            obj.second.data[ObjectData::X].size() > 0) {
                            ImPlot::SetNextLineStyle(obj.second.color, 4.0);
                            switch (i) {
                                case 1:
                                    ImPlot::PlotLine(
                                        obj.second.label.c_str(),
                                        &obj.second.data[ObjectData::X][0].x,
                                        &obj.second.data[ObjectData::X][0].y,
                                        obj.second.data[ObjectData::X].size(), 0, 0,
                                        2 * sizeof(float));
                                    break;
                                case 2:
                                    ImPlot::PlotLine(
                                        obj.second.label.c_str(),
                                        &obj.second.data[ObjectData::Y][0].x,
                                        &obj.second.data[ObjectData::Y][0].y,
                                        obj.second.data[ObjectData::Y].size(), 0, 0,
                                        2 * sizeof(float));
                                    break;
                                case 3:
                                    ImPlot::PlotLine(
                                        obj.second.label.c_str(),
                                        &obj.second.data[ObjectData::VX][0].x,
                                        &obj.second.data[ObjectData::VX][0].y,
                                        obj.second.data[ObjectData::VX].size(), 0, 0,
                                        2 * sizeof(float));
                                    break;
                                case 4:
                                    ImPlot::PlotLine(
                                        obj.second.label.c_str(),
                                        &obj.second.data[ObjectData::VY][0].x,
                                        &obj.second.data[ObjectData::VY][0].y,
                                        obj.second.data[ObjectData::VY].size(), 0, 0,
                                        2 * sizeof(float));
                                    break;
                                case 5:
                                    ImPlot::PlotLine(
                                        obj.second.label.c_str(),
                                        &obj.second.data[ObjectData::CAMERA_ID][0].x,
                                        &obj.second.data[ObjectData::CAMERA_ID][0].y,
                                        obj.second.data[ObjectData::CAMERA_ID].size(), 0, 0,
                                        2 * sizeof(float));
                                    break;
                                case 6:
                                    ImPlot::PlotLine(
                                        obj.second.label.c_str(),
                                        &obj.second.data[ObjectData::RADAR_ID][0].x,
                                        &obj.second.data[ObjectData::RADAR_ID][0].y,
                                        obj.second.data[ObjectData::RADAR_ID].size(), 0, 0,
                                        2 * sizeof(float));
                                    break;
                                default:
                                    break;
                            }
                            if (ImPlot::BeginDragDropSourceItem(obj.second.label.c_str())) {
                                PlotIndex index{"", obj.first};
                                strcpy(index.name, list.first.c_str());
                                ImGui::SetDragDropPayload("MY_DND", &index, sizeof(PlotIndex));
                                ImPlot::ItemIcon(obj.second.color);
                                ImGui::SameLine();
                                ImGui::TextUnformatted(obj.second.label.c_str());
                                ImPlot::EndDragDropSource();
                            }
                        }
                    }
                }

                if (ImPlot::BeginDragDropTargetPlot()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
                        auto tmp = *(PlotIndex*)payload->Data;
                        objs[tmp.name][tmp.id].subPlotId = 1;
                    }
                    ImPlot::EndDragDropTarget();
                }

                if (ImPlot::BeginDragDropTargetLegend()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
                        auto tmp = *(PlotIndex*)payload->Data;
                        objs[tmp.name][tmp.id].subPlotId = 1;
                    }
                    ImPlot::EndDragDropTarget();
                }
                ImPlot::EndPlot();
            }
        }
    });
}

void ObjectPlot::dragClassProb() {
    basePlot([&]() {
        // if (ImPlot::BeginPlot("##DND", ImVec2(-1, 200))) {
        if (ImPlot::BeginPlot("##DND")) {
            static double min{INT_MAX};
            min = std::min(min, time);
            for (auto& list : objs) {
                for (auto& obj : list.second) {
                    if (obj.second.subPlotId != -1 && obj.second.data[ObjectData::X].size() > 0) {
                        ImPlot::SetNextLineStyle(obj.second.color, 4.0);
                        ImPlot::PlotLine(
                            obj.second.label.c_str(), &obj.second.data[ObjectData::CLASSPROB][0].x,
                            &obj.second.data[ObjectData::CLASSPROB][0].y,
                            obj.second.data[ObjectData::CLASSPROB].size(), 0, 0, 2 * sizeof(float));
                        if (ImPlot::BeginDragDropSourceItem(obj.second.label.c_str())) {
                            PlotIndex index{"", obj.first};
                            strcpy(index.name, list.first.c_str());
                            ImGui::SetDragDropPayload("MY_DND", &index, sizeof(PlotIndex));
                            ImPlot::ItemIcon(obj.second.color);
                            ImGui::SameLine();
                            ImGui::TextUnformatted(obj.second.label.c_str());
                            ImPlot::EndDragDropSource();
                        }
                    }
                }
            }

            if (ImPlot::BeginDragDropTargetPlot()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
                    auto tmp = *(PlotIndex*)payload->Data;
                    objs[tmp.name][tmp.id].subPlotId = 1;
                }
                ImPlot::EndDragDropTarget();
            }

            if (ImPlot::BeginDragDropTargetLegend()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
                    auto tmp = *(PlotIndex*)payload->Data;
                    objs[tmp.name][tmp.id].subPlotId = 1;
                }
                ImPlot::EndDragDropTarget();
            }
            ImPlot::EndPlot();
        }
    });
}

void ObjectPlot::dragBev() {
    ImPlot::SetNextAxisLimits(ImAxis_X1, -60, 200);
    ImPlot::SetNextAxisLimits(ImAxis_Y1, -40, 40);
    if (ImPlot::BeginPlot("##CustomRend", ImVec2(-1, 900))) {
        static float i = 1;
        ImVec2 rmin = ImPlot::PlotToPixels(ImPlotPoint(2.5f + i, 7.5f));
        ImVec2 rmax = ImPlot::PlotToPixels(ImPlotPoint(7.5f + i, 2.5f));
        ImPlot::PushPlotClipRect();
        ImPlot::GetPlotDrawList()->AddRect(rmin, rmax, IM_COL32(128, 0, 255, 255));
        ImPlot::PlotText("Vertical Text", 2.5f + i, 7.5f, ImVec2(0, 0));
        ImPlot::PopPlotClipRect();
        i += 1;
        ImPlot::EndPlot();
    }
}

template <typename T>
void ObjectPlot::plot(
    T* front_cam_obj, T* front_radar_obj, const T& output, T* const side_cam_obj, T* fl_radar_obj,
    T* fr_rdara_obj, T* rl_radar_obj, T* rr_rdara_obj) {
    addCamera(front_cam_obj);
    addRadar(front_radar_obj);
    addFusion(output);
    addSideCamera(side_cam_obj);
    addFl_radar(fl_radar_obj);
    addfr_radar(fr_rdara_obj);
    addrl_radar(rl_radar_obj);
    addrr_radar(rr_rdara_obj);
    count++;
}

template <typename T>
void ObjectPlot::addCamera(T* front_cam_obj) {
    convertData(objs, "DND_FRONT_CAMERA", front_cam_obj);
}

template <typename T>
void ObjectPlot::addFusion(const T& output) {}

template <typename T>
void ObjectPlot::addRadar(T* front_radar_obj) {
    convertData(objs, "DND_FRONT_RADAR", front_radar_obj);
}

template <typename T>
void ObjectPlot::addSideCamera(T* side_cam_obj) {
    convertData(objs, "DND_SIDE_CAMERA", side_cam_obj);
}

template <typename T>
void ObjectPlot::addFl_radar(T* fl_radar_obj) {
    convertData(objs, "DND_FL_RADAR", fl_radar_obj);
}

template <typename T>
void ObjectPlot::addfr_radar(T* fr_rdara_obj) {
    convertData(objs, "DND_FR_RADAR", fr_rdara_obj);
}

template <typename T>
void ObjectPlot::addrl_radar(T* rl_radar_obj) {
    convertData(objs, "DND_RL_RADAR", rl_radar_obj);
}

template <typename T>
void ObjectPlot::addrr_radar(T* rr_rdara_obj) {
    convertData(objs, "DND_RR_RADAR", rr_rdara_obj);
}
