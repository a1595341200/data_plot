/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-20 11:24:32
 * @FilePath: /cplusplus/submodule/data_plot/src/ObjectPlot.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "ObjectPlot.h"

#include <thread>

ObjectPlot::ObjectPlot(std::string title, int w, int h) : App(title, w, h) {}

std::unique_ptr<ObjectPlot> ObjectPlot::objPlotPtr = nullptr;

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
}

void ObjectPlot::dragAndDropPlot() {
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 750), ImGuiCond_FirstUseEver);
    static int e = FRONT;
    ImGui::RadioButton("front", &e, FRONT);
    ImGui::SameLine();
    ImGui::RadioButton("side", &e, SIDE);
    if (ImGui::Button("Reset Data")) {
        objs.clear();
    }
    for (auto& [scene, name] : dnd_name) {
        if (scene != e && scene != ALWAYS) {
            continue;
        }
        ImGui::BeginChild(name.c_str(), ImVec2(120, 800));
        for (auto& obj : objs[name]) {
            if (obj.second.subPlotId != -1) {
                continue;
            }
            ImPlot::ItemIcon(obj.second.color);
            ImGui::SameLine();
            ImGui::Selectable(obj.second.label.c_str(), false, 0, ImVec2(120, 0));
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
                objs[tmp.name][tmp.id].reset();
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::SameLine();
    }

    ImGui::BeginChild("DND_RIGHT", ImVec2(-1, -1));
    ImPlotAxisFlags flags = ImPlotAxisFlags_AutoFit;

    for (int i = 1; i <= 4; ++i) {
        ImGui::NewLine();
        if (ImPlot::BeginPlot(("##DND" + std::to_string(i)).c_str(), ImVec2(-1, 200))) {
            ImPlot::SetupAxis(ImAxis_X1, nullptr, flags | ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr, flags | ImPlotAxisFlags_AutoFit);
            static double min{INT_MAX};
            min = std::min(min, time);
            ImPlot::SetupAxisLimits(ImAxis_X1, min, time, ImGuiCond_Always);
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
                default:
                    break;
            }
            for (auto& list : objs) {
                for (auto& obj : list.second) {
                    // if (obj.second.subPlotId == i && obj.second.data.size() > 0) {
                    if (obj.second.subPlotId != -1 && obj.second.data[ObjectData::X].size() > 0) {
                        ImPlot::SetNextLineStyle(obj.second.color, 4.0);
                        switch (i) {
                            case 1:
                                ImPlot::PlotLine(
                                    obj.second.label.c_str(), &obj.second.data[ObjectData::X][0].x,
                                    &obj.second.data[ObjectData::X][0].y,
                                    obj.second.data[ObjectData::X].size(), 0, 0, 2 * sizeof(float));
                                break;
                            case 2:
                                ImPlot::PlotLine(
                                    obj.second.label.c_str(), &obj.second.data[ObjectData::Y][0].x,
                                    &obj.second.data[ObjectData::Y][0].y,
                                    obj.second.data[ObjectData::Y].size(), 0, 0, 2 * sizeof(float));
                                break;
                            case 3:
                                ImPlot::PlotLine(
                                    obj.second.label.c_str(), &obj.second.data[ObjectData::VX][0].x,
                                    &obj.second.data[ObjectData::VX][0].y,
                                    obj.second.data[ObjectData::VX].size(), 0, 0,
                                    2 * sizeof(float));
                                break;
                            case 4:
                                ImPlot::PlotLine(
                                    obj.second.label.c_str(), &obj.second.data[ObjectData::VY][0].x,
                                    &obj.second.data[ObjectData::VY][0].y,
                                    obj.second.data[ObjectData::VY].size(), 0, 0,
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

    ImGui::EndChild();
}
