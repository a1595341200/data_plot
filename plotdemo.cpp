/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-14 14:59:25
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-15 14:28:31
 * @FilePath: /cplusplus/submodule/data_plot/main.cpp
 * @Description: 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-14 14:59:25
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-03-15 13:01:56
 * @FilePath: /cplusplus/submodule/data_plot/main.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
// Demo:   demo.cpp
// Author: Evan Pezent (evanpezent.com)
// Date:   3/26/2021

#include "App.h"
struct ImPlotDemo : App {
    using App::App;
    void Update() override {
        ImPlot::ShowDemoWindow();
    }
};

ImPlotDemo app("ImPlot Demo", 1920, 1080);

int main(int argc, char const* argv[]) {
    app.Run();
    return 0;
}
