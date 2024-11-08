/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-11-08 13:46:49
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-11-08 13:51:57
 * @FilePath: /cplusplus/submodule/data_plot/common/SensorType.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef SENSORTYPE_H
#define SENSORTYPE_H
#include <cstdint>

enum SensorType : uint16_t {
    FRONT_CAMERA = 1u,
    FRONT_RADAR = 2u,
    FUSION = 4u,
    EGOMOTION = 8u,
    FL_RADAR = 16u,
    FR_RADAR = 32u,
    RL_RADAR = 64u,
    RR_RADAR = 128u,
    SIDE_CAMERA = 256u,
    MAX
};

#endif /* SENSORTYPE_H */
