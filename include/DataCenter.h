/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-11-08 13:45:06
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-11-08 13:58:28
 * @FilePath: /cplusplus/submodule/data_plot/include/DataCenter.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef DATACENTER_H
#define DATACENTER_H
#include <limits>
#include <map>
#include <memory>
#include <unordered_map>

#include "DndName.h"
#include "ObjectData.h"

class DataCenter : std::enable_shared_from_this<DataCenter> {
public:
    void Reset() {}

    void Init() {}

    std::shared_ptr<uint64_t> frameCount{std::make_shared<uint64_t>()};

    std::shared_ptr<std::unordered_map<uint64_t, std::map<int, ObjectData>>> objs{
        std::make_shared<std::unordered_map<uint64_t, std::map<int, ObjectData>>>()};

    double time{INT_MAX};
};

#endif /* DATACENTER_H */
