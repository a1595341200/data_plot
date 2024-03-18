/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-18 15:14:14
 * @FilePath: /cplusplus/submodule/data_plot/src/ObjectPlot.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "ObjectPlot.h"

ObjectPlot::ObjectPlot(std::string title, int w, int h) : App(title, w, h) {
    for (int i = 0; i < 20; ++i) {
        objs[i];
    }
}

void ObjectPlot::Update() {
    dragAndDropPlot();
}

void ObjectPlot::dragAndDropPlot() {
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 750), ImGuiCond_FirstUseEver);
    std::vector<std::string> dnd_name{"DND_LEFT", "DND_TEST"};

    for (auto& name : dnd_name) {
        ImGui::BeginChild(name.c_str(), ImVec2(100, 800));
        // if (ImGui::Button("Reset Data")) {
        //     for (auto& obj : objs) {
        //         obj.second.reset();
        //     }
        // }
        for (auto& obj : objs) {
            if (obj.second.subPlotId > 0) {
                continue;
            }
            ImPlot::ItemIcon(obj.second.color);
            ImGui::SameLine();
            ImGui::Selectable(obj.second.label.c_str(), false, 0, ImVec2(100, 0));
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                ImGui::SetDragDropPayload("MY_DND", &obj.first, sizeof(obj.first));
                ImPlot::ItemIcon(obj.second.color);
                ImGui::SameLine();
                ImGui::TextUnformatted(obj.second.label.c_str());
                ImGui::EndDragDropSource();
            }
        }
        ImGui::EndChild();
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
                int i = *(int*)payload->Data;
                objs[i].reset();
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::SameLine();
    }

    ImGui::BeginChild("DND_RIGHT", ImVec2(-1, -1));
    ImPlotAxisFlags flags = ImPlotAxisFlags_AutoFit;

    for (int i = 1; i <= 4; ++i) {
        ImGui::NewLine();
        ImPlotAxisFlags flags = ImPlotAxisFlags_AutoFit;
        if (ImPlot::BeginPlot(("##DND" + std::to_string(i)).c_str(), ImVec2(-1, 200))) {
            ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_AutoFit);
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
            for (auto& obj : objs) {
                // if (obj.second.subPlotId == i && obj.second.data.size() > 0) {
                if (obj.second.subPlotId != -1 && obj.second.data.size() > 0) {
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
                                obj.second.data[ObjectData::VX].size(), 0, 0, 2 * sizeof(float));
                            break;
                        case 4:
                            ImPlot::PlotLine(
                                obj.second.label.c_str(), &obj.second.data[ObjectData::VY][0].x,
                                &obj.second.data[ObjectData::VY][0].y,
                                obj.second.data[ObjectData::VY].size(), 0, 0, 2 * sizeof(float));
                            break;
                        default:
                            break;
                    }
                    if (ImPlot::BeginDragDropSourceItem(obj.second.label.c_str())) {
                        ImGui::SetDragDropPayload("MY_DND", &obj.first, sizeof(int));
                        ImPlot::ItemIcon(obj.second.color);
                        ImGui::SameLine();
                        ImGui::TextUnformatted(obj.second.label.c_str());
                        ImPlot::EndDragDropSource();
                    }
                }
            }
            if (ImPlot::BeginDragDropTargetPlot()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
                    auto tmp = *(int*)payload->Data;
                    objs[tmp].subPlotId = i;
                }
                ImPlot::EndDragDropTarget();
            }

            if (ImPlot::BeginDragDropTargetLegend()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
                    auto tmp = *(int*)payload->Data;
                    objs[tmp].subPlotId = i;
                }
                ImPlot::EndDragDropTarget();
            }
            ImPlot::EndPlot();
        }
    }

    ImGui::EndChild();
}
