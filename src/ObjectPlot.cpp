/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-29 22:52:28
 * @FilePath: /cplusplus/submodule/data_plot/src/ObjectPlot.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "ObjectPlot.h"

#include <thread>

#include "AppLog.h"

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

            if (ImGui::BeginChild(name.c_str(), ImVec2(120, 1080))) {
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

        if (ImGui::BeginChild("DND_RIGHT")) {
            func();
        }
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
    static bool show_StateX{};
    static bool show_ClassProb{};
    static bool show_Bev{};
    static bool show_Test{};
    static bool show_MenuBar = true;
    ImGui::SetWindowSize(ImVec2(900, 900));
    if (ImGui::Begin("Example: Simple layout", &show_MenuBar, ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("plot")) {
                ImGui::MenuItem("StateX", NULL, &show_StateX);
                // ImGui::SeparatorText("Mini apps");
                ImGui::MenuItem("ClassProb", NULL, &show_ClassProb);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Test")) {
                ImGui::MenuItem("debug", NULL, &show_Test);
                // ImGui::SeparatorText("Concepts");
                ImGui::EndMenu();
            }
            // if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
            ImGui::EndMenuBar();
        }
        if (show_StateX) {
            dragAndDropPlot(&show_StateX);
        }
        if (show_ClassProb) {
            dragClassProb(&show_ClassProb);
        }
        if (show_Bev) {
            dragBev(&show_Bev);
        }
        if (show_Test) {
            static AppLog log;
            // For the demo: add a debug button _BEFORE_ the normal log window contents
            // We take advantage of a rarely used feature: multiple calls to Begin()/End() are
            // appending to the _same_ window. Most of the contents of the window will be added by
            // the log.Draw() call.
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Log", &show_Test);
            // IMGUI_DEMO_MARKER("Examples/Log");
            if (ImGui::SmallButton("[Debug] Add 5 entries")) {
                static int counter = 0;
                const char* categories[3] = {"info", "warn", "error"};
                const char* words[] = {"Bumfuzzled",    "Cattywampus",  "Snickersnee",
                                       "Abibliophobia", "Absquatulate", "Nincompoop",
                                       "Pauciloquent"};
                for (int n = 0; n < 5; n++) {
                    const char* category = categories[counter % IM_ARRAYSIZE(categories)];
                    const char* word = words[counter % IM_ARRAYSIZE(words)];
                    log.AddLog(
                        "[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                        ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
                    counter++;
                }
            }
            ImGui::End();

            // Actually call in the regular Log helper (which will Begin() into the same window as
            // we just did)
            // static bool show_Test1 = true;
            log.Draw("Log", &show_Test);
        }
    }
    ImGui::End();
    // ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    // if (ImGui::TreeNode("StateX")) {
    //     dragAndDropPlot();
    //     ImGui::TreePop();
    // }
    // if (ImGui::TreeNode("ClassProb")) {
    //     dragClassProb();
    //     ImGui::TreePop();
    // }
    // if (ImGui::TreeNode("Bev")) {
    //     dragBev();
    //     ImGui::TreePop();
    // }
    if ("test") {
    }
}

void ObjectPlot::dragAndDropPlot(bool* open) {
    if (ImGui::Begin("dragAndDropPlot", open, 0)) {
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
        ImGui::End();
    } else {
        ImGui::End();
    }
}

void ObjectPlot::dragClassProb(bool* open) {
    if (ImGui::Begin("dragClassProb", open, 0)) {
        basePlot([&]() {
            // if (ImPlot::BeginPlot("##DND", ImVec2(-1, 200))) {
            if (ImPlot::BeginPlot("##DND")) {
                static double min{INT_MAX};
                min = std::min(min, time);
                for (auto& list : objs) {
                    for (auto& obj : list.second) {
                        if (obj.second.subPlotId != -1 &&
                            obj.second.data[ObjectData::X].size() > 0) {
                            ImPlot::SetNextLineStyle(obj.second.color, 4.0);
                            ImPlot::PlotLine(
                                obj.second.label.c_str(),
                                &obj.second.data[ObjectData::CLASSPROB][0].x,
                                &obj.second.data[ObjectData::CLASSPROB][0].y,
                                obj.second.data[ObjectData::CLASSPROB].size(), 0, 0,
                                2 * sizeof(float));
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
        ImGui::End();
    } else {
        ImGui::End();
    }
}

void ObjectPlot::dragBev(bool* open) {
    if (ImGui::Begin("dragBev", open, 0)) {  // ImPlot::SetNextAxisLimits(ImAxis_X1, -60, 200);
        // ImPlot::SetNextAxisLimits(ImAxis_Y1, -40, 40);
        if (ImPlot::BeginPlot("##CustomRend", ImVec2(-1, 900))) {
            plotSelf();
            ImPlot::EndPlot();
        }
        ImGui::End();
    } else {
        ImGui::End();
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
        ImVec2(x + r1x, y + r1y), ImVec2(x + r2x, y + r2y), ImVec2(x - r1x, y - r1y),
        ImVec2(x - r2x, y - r2y), ImVec2(x + r1x, y + r1y)};
}
