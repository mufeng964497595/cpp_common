/*
 * File Created: Sunday, 7th February 2021 15:37:33
 * Author: huangwf (964497595@qq.com)
 * Desc: 工具类
 * -----
 * Copyright (c) 2021 Inc.
 */

#ifndef __CPP_COMMON_UTILS_FACILITY_HPP__
#define __CPP_COMMON_UTILS_FACILITY_HPP__

#include <stdint.h>

#include <string>

namespace hwf {
namespace utils {
    std::string GetLocalIp();   // 获取本机IP
    uint64_t GetTimeStampMs();  // 获取毫秒级时间戳
};  // end namespace utils
};  // end namespace hwf

#endif  // end #ifndef __CPP_COMMON_UTILS_FACILITY_HPP__
