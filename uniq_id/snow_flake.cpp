/*
 * File Created: Sunday, 7th February 2021 15:07:46
 * Author: huangwf (964497595@qq.com)
 * Desc: 雪花算法，生成唯一id
 * -----
 * Copyright (c) 2021 Inc.
 */

#include <iostream>
#include <mutex>    // NOLINT
#include <random>
#include <string>

#include "cpp_common/utils/facility.hpp"
#include "cpp_common/uniq_id/snow_flake.hpp"

using hwf::CSnowFlake;

std::unique_ptr<CSnowFlake> CSnowFlake::instance_;

void CSnowFlake::Init() {
    is_init_ = false;
    if (auto_incr_bit_ + work_id_bit_ + timestamp_ms_bit_ + 1 > 64) {
        // TODO(huangwf) 错误日志
        std::cout << "sum(bit) > 64: " << auto_incr_bit_ + work_id_bit_ + timestamp_ms_bit_ + 1 << "\n";
        return;
    }

    std::string local_ip(hwf::utils::GetLocalIp());
    // TODO(huangwf) 根据机器ip读取mysql获取work_id
    work_id_ = 1;
    if (work_id_ < 0 || work_id_ >= (1 << (work_id_bit_))) {
        // TODO(huangwf) 错误日志
        std::cout << "workid: " << work_id_ << " is bigger than " << (1<<work_id_bit_) << "\n";
        return;
    }
    std::cout << "Init succ\n";
    is_init_ = true;
}

bool CSnowFlake::IsInitSucc() {
    return is_init_;
}

int CSnowFlake::GetUniqId(uint64_t& uniq_id) {
    std::lock_guard<std::mutex> guard(mutex_);  // 加锁，避免多线程访问出现异常

    if (!is_init_) {
        return ERROR_INIT;
    }

    // 计算时间戳部分
    uint64_t cur_timestamp_ms = hwf::utils::GetTimeStampMs();
    // 计算自增id部分
    // TODO(huangwf) 优化时间回拨问题
    int auto_incr_id(-1);
    if (cur_timestamp_ms == last_timestamp_ms_ && last_auto_incr_id_ + 1 <= max_auto_incr_id_) {
        // 跟上次处理的时间是同一ms，且自增id未到上限，直接自增id + 1
        auto_incr_id = last_auto_incr_id_ + 1;
    } else {
        while (cur_timestamp_ms == last_timestamp_ms_) {  // 当前毫秒自增id已满，等待下一个毫秒
            cur_timestamp_ms = hwf::utils::GetTimeStampMs();
        }

        // 当前时间戳是第一次处理，随机生成0~9起始的自增id，避免当qps较小时自增id都是0
        // TODO(huangwf) add nodiff
        static std::default_random_engine engine;
        engine.seed(cur_timestamp_ms);
        static std::uniform_int_distribution<unsigned> rand(0, 9);
        auto_incr_id = rand(engine);
    }
    uint64_t timestamp_ms = cur_timestamp_ms - timestamp_ms_offset_;
    if (timestamp_ms > max_timestamp_ms_) {
        return ERROR_TIMESTAMP_OVERFLOW;
    }

    uniq_id = (timestamp_ms << (auto_incr_bit_ + work_id_bit_)) | (work_id_ << auto_incr_bit_) | auto_incr_id;
    // 更新维护当前信息
    last_timestamp_ms_ = cur_timestamp_ms;
    last_auto_incr_id_ = auto_incr_id;

    return OK;
}
