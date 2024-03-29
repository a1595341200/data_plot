/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-29 17:41:17
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
    if (ImGui::TreeNode("test")) {
        ImVector<char*> Items;
        bool ScrollToBottom;
        bool AutoScroll;
        char InputBuf[256];

        if (ImGui::BeginChild(
                "ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None,
                ImGuiWindowFlags_HorizontalScrollbar)) {
            if (ImGui::BeginPopupContextWindow()) {
                if (ImGui::Selectable("Clear"))
                    ClearLog();
                ImGui::EndPopup();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));  // Tighten spacing

            for (const char* item : Items) {
                if (!Filter.PassFilter(item))
                    continue;

                // Normally you would store more information in your item than just a string.
                // (e.g. make Items[] an array of structure, store color/type etc.)
                ImVec4 color;
                bool has_color = false;
                if (strstr(item, "[error]")) {
                    color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                    has_color = true;
                } else if (strncmp(item, "# ", 2) == 0) {
                    color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                    has_color = true;
                }
                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item);
                if (has_color)
                    ImGui::PopStyleColor();
            }

            // Keep up at the bottom of the scroll region if we were already at the bottom at the
            // beginning of the frame. Using a scrollbar or mouse-wheel will take away from the
            // bottom edge.
            if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags =
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll |
            ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        // if (ImGui::InputText(
        //         "Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub,
        //         (void*)this)) {
        //     char* s = InputBuf;
        //     Strtrim(s);
        //     if (s[0])
        //         ExecCommand(s);
        //     strcpy(s, "");
        //     reclaim_focus = true;
        // }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1);  // Auto focus previous widget

        ImGui::End();
    }
}
ImGui::EndChild();
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
        for (int i = 1; i <= 7; ++i) {
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
    // ImPlot::SetNextAxisLimits(ImAxis_X1, -60, 200);
    // ImPlot::SetNextAxisLimits(ImAxis_Y1, -40, 40);
    if (ImPlot::BeginPlot("##CustomRend", ImVec2(-1, 900))) {
        plotSelf();
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

void ObjectPlot::plotSelf() {
    auto points = rotatedRect(-1.15, 0, 2.425, 0.96, 1, 1);
    ImPlot::SetNextLineStyle(ImVec4(255, 0, 0, 255), 4.0);
    ImPlot::PlotLine("", &points[0].x, &points[0].y, 5, 0, 0, 2 * sizeof(float));
}

void ObjectPlot::plotRect(
    float x, float y, float heading, float length, float width, float height, int nearSide) {
    std::vector<ImVec2> points;
    points.emplace_back(-1.15, 0.96);
    points.emplace_back(3.7, 0.96);
    points.emplace_back(3.7, -0.96);
    points.emplace_back(-1.15, -0.96);
    points.emplace_back(-1.15, 0.96);
    ImPlot::SetNextLineStyle(ImVec4(255, 0, 0, 255), 4.0);
    ImPlot::PlotLine("", &points[0].x, &points[0].y, 5, 0, 0, 2 * sizeof(float));
}

std::vector<ImVec2> ObjectPlot::rotatedRect(
    double x, double y, double half_length, double half_width, double angle, uint8_t nearest_side) {
    double c = cos(angle);
    double s = sin(angle);
    if (nearest_side == 0) {
    } else if (nearest_side == 1) {
        x = x + half_length * c;
        y = y + half_length * s;
    } else if (nearest_side == 2) {
        x = x - half_length * c;
        y = y - half_length * s;
    } else if (nearest_side == 3) {
        x = x - half_width * s;
        y = y + half_width * c;
    } else if (nearest_side == 4) {
        x = x + half_width * s;
        y = y - half_width * c;
    }
    double r1x = -half_length * c - half_width * s;
    double r1y = -half_length * s + half_width * c;
    double r2x = half_length * c - half_width * s;
    double r2y = half_length * s + half_width * c;
    return {
        {x + r1x, y + r1y},
        {x + r2x, y + r2y},
        {x - r1x, y - r1y},
        {x - r2x, y - r2y},
        {x + r1x, y + r1y}};
}
