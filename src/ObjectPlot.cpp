/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-18 10:24:21
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
        if (ImGui::Button("Reset Data")) {
            for (auto& obj : objs) {
                obj.second.reset();
            }
        }
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
                objs[i].subPlotId = i;
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
        // switch (i) {
        //     case 1:
        //         ImGui::SameLine();
        //         break;
        //     case 2:
        //         ImGui::SameLine();
        //         break;
        //     case 3:
        //         ImGui::NewLine();
        //         break;
        //     case 4:
        //         ImGui::SameLine();
        //         break;

        //     default:
        //         break;
        // };

        ImPlotAxisFlags flags = ImPlotAxisFlags_AutoFit;
        if (ImPlot::BeginPlot(("##DND" + std::to_string(i)).c_str(), ImVec2(-1, 200))) {
            ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_AutoFit);
            ImPlot::SetupAxes("x", "y");

            for (auto& obj : objs) {
                if (obj.second.subPlotId == i && obj.second.data.size() > 0) {
                    ImPlot::SetNextLineStyle(obj.second.color, 4.0);
                    ImPlot::PlotLine(
                        obj.second.label.c_str(), &obj.second.data[0].x, &obj.second.data[0].y,
                        obj.second.data.size(), 0, 0, 2 * sizeof(float));
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
