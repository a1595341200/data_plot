/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-15 16:08:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-11-08 14:11:45
 * @FilePath: /cplusplus/submodule/data_plot/src/ObjectPlot.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "ObjectPlot.h"

#include <Image.h>

#include <cmath>
#include <thread>

#include "Convert.h"
#include "PlotIndex.h"
#include "Utils.h"

// stb_image.h: Download from https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "AppLog.h"

ObjectPlot::ObjectPlot(std::string title, int w, int h) : App(title, w, h) {
    basePlot = [&](std::function<void()> func) {
        if (ImGui::Button("Reset Data")) {
            objs->clear();
        }
        ImGui::NewLine();
        for (auto& sceneName : mDndNameMap) {
            ImGui::SameLine();
            ImGui::Checkbox(sceneName.second.realName.c_str(), &sceneName.second.show);
        }
        ImGui::Separator();
        for (auto& objPlot : mObjPlotMap) {
            ImGui::SameLine();
            ImGui::Checkbox(objPlot.second.yLabel.c_str(), &objPlot.second.show);
        }
        ImGui::Separator();
        for (auto& sceneName : mDndNameMap) {
            std::string& name = sceneName.second.dndName;
            if (!sceneName.second.show) {
                continue;
            }
            ImGui::BeginChild(name.c_str(), ImVec2(200, 1080));
            for (auto& obj : (*objs)[sceneName.first]) {
                if (obj.second.subPlotId != -1) {
                    continue;
                }
                ImPlot::ItemIcon(obj.second.color);
                ImGui::SameLine();
                ImGui::Selectable(obj.second.label.c_str(), false, 0, ImVec2(180, 0));
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                    PlotIndex index{sceneName.first, obj.first};
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
                    (*objs)[tmp.name][tmp.id].subPlotId = -1;
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::SameLine();
        }
        if (ImGui::BeginChild("DND_RIGHT", ImVec2(-1, 2400))) {
            func();
        }
        ImGui::EndChild();
    };
}

std::unique_ptr<ObjectPlot> ObjectPlot::objPlotPtr = nullptr;

void ObjectPlot::startPLot() {
    std::thread t([&] {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        objPlotPtr = std::make_unique<ObjectPlot>("Object Plot", 1920, 1080);
        objPlotPtr->Init();
        objPlotPtr->Run();
    });
    t.detach();
}

GLuint loadImageToTexture(const char* filename, int* image_width, int* image_height) {
    int channels;
    unsigned char* image_data = stbi_load(filename, image_width, image_height, &channels, 4);
    if (image_data == nullptr) {
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, *image_width, *image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        image_data);

    stbi_image_free(image_data);

    return texture;
}

void showImage(const std::string& filename) {
    int img_width, img_height;
    Image image;
    GLuint img_texture = loadImageToTexture(
        "/home/user/图片/2024-09-25 10-53-54 的屏幕截图.png", &img_width, &img_height);
    ImGui::Image((ImTextureID)(intptr_t)img_texture, ImVec2(img_width, img_height));
}

void ObjectPlot::Update() {
    static bool show_StateX{};
    static bool show_ClassProb{};
    static bool show_Bev{};
    static bool show_Test{};
    static bool show_MenuBar = true;
    ImGui::SetNextWindowSize(ImVec2(1763, 900), ImGuiCond_FirstUseEver);
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
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("Log", &show_Test);
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

            log.Draw("Log", &show_Test);
        }
    }
    ImGui::End();
}

void ObjectPlot::dragAndDropPlot(bool* open) {
    ImGui::SetNextWindowSize(ImVec2(1531, 889), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("dragAndDropPlot", open, 0)) {
        basePlot([&]() {
            for (auto& objPlot : mObjPlotMap) {
                if (!objPlot.second.show) {
                    continue;
                }
                ImGui::NewLine();
                if (ImPlot::BeginPlot(("##DND" + std::to_string(objPlot.first)).c_str())) {
                    static double min{INT_MAX};
                    min = std::min(min, time);
                    ImPlot::SetupAxes("time", objPlot.second.yLabel.c_str());

                    for (auto& list : (*objs)) {
                        for (auto& obj : list.second) {
                            if (obj.second.subPlotId != -1 &&
                                obj.second.data[ObjectData::X].size() > 0) {
                                ImPlot::SetNextLineStyle(obj.second.color, 4.0);
                                std::vector<ImVec2>& data = obj.second.data[objPlot.first];

                                ImPlot::PlotLine(
                                    obj.second.label.c_str(), &data[0].x, &data[0].y, data.size(),
                                    0, 0, 2 * sizeof(float));

                                if (ImPlot::BeginDragDropSourceItem(obj.second.label.c_str())) {
                                    PlotIndex index{list.first, obj.first};
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
                            (*objs)[tmp.name][tmp.id].subPlotId = 1;
                        }
                        ImPlot::EndDragDropTarget();
                    }

                    if (ImPlot::BeginDragDropTargetLegend()) {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
                            auto tmp = *(PlotIndex*)payload->Data;
                            (*objs)[tmp.name][tmp.id].subPlotId = 1;
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
                for (auto& list : *objs) {
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
                                PlotIndex index{list.first, obj.first};
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
                        (*objs)[tmp.name][tmp.id].subPlotId = 1;
                    }
                    ImPlot::EndDragDropTarget();
                }

                if (ImPlot::BeginDragDropTargetLegend()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MY_DND")) {
                        auto tmp = *(PlotIndex*)payload->Data;
                        (*objs)[tmp.name][tmp.id].subPlotId = 1;
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
    Convert::convert(front_cam_obj, mDataCenter);
    Convert::convert(front_radar_obj, mDataCenter);
    Convert::convert(output, mDataCenter);
    Convert::convert(side_cam_obj, mDataCenter);
    Convert::convert(fl_radar_obj, mDataCenter);
    Convert::convert(fr_rdara_obj, mDataCenter);
    Convert::convert(rl_radar_obj, mDataCenter);
    Convert::convert(rr_rdara_obj, mDataCenter);
    (*frameCount)++;
}

void ObjectPlot::plotSelf() {
    auto points = Utils::rotatedRect(-1.15, 0, 2.425, 0.96, 1, 1);
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

void ObjectPlot::Init() {
    objs = mDataCenter->objs;
    frameCount = mDataCenter->frameCount;
    for (size_t i = 0; i < ObjectData::MAX; ++i) {
        mObjPlotMap.emplace(i, ObjPlotSelect{ObjectData::id2yLabel[i], false});
    }
}
