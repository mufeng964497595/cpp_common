/*
 * File Created: Sunday, 7th February 2021 17:33:23
 * Author: huangwf (964497595@qq.com)
 * Desc: 测试
 * -----
 * Copyright (c) 2021 Inc.
 */

#include <iostream>
#include <memory>

#include "cpp_common/uniq_id/snow_flake.hpp"

int main(int argc, char* argv[]) {
    hwf::CSnowFlake& snow_flake = hwf::CSnowFlake::GetInstance();
    hwf::CSnowFlake& snow_flake2 = hwf::CSnowFlake::GetInstance();
    if (!snow_flake.IsInitSucc()) {
        std::cout << "snow_flake init fail, exit\n";
        return -1;
    }
    uint64_t uniq_id(0);
    int ret = snow_flake.GetUniqId(uniq_id);
    if (hwf::CSnowFlake::OK != ret) {
        std::cout << "snow_flake GetUniqId fail, ret: " << ret << "\n";
        return -1;
    }

    std::cout << "uniq_id: " << uniq_id << "\n";
    snow_flake2.GetUniqId(uniq_id);
    std::cout << "uniq_id2: " << uniq_id << "\n";
    return 0;
}
