/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-14 14:59:25
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-04-02 15:01:28
 * @FilePath: /cplusplus/submodule/data_plot/plotdemo1.cpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

#include "ObjectPlot.h"

int main(int argc, char const* argv[]) {
    ObjectPlot app("ImPlot Demo", 1920, 1080);
    app.init();
    app.Run();
    return 0;
}
