/*
 * File Created: Sunday, 7th February 2021 14:22:20
 * Author: huangwf (964497595@qq.com)
 * Desc: 雪花算法，生成唯一id
 * -----
 * Copyright (c) 2021 Inc.
 */

#ifndef CPP_COMMON_UNIQ_ID_SNOW_FLAKE_HPP
#define CPP_COMMON_UNIQ_ID_SNOW_FLAKE_HPP

#include <stdint.h>

#include <memory>
#include <mutex>    // NOLINT

namespace hwf {
/*
    雪花算法id 64位二进制格式：
    位数：|   63  |   62 ~ 22   |    21 ~ 12   | 11 ~ 0 |
    含义：| 符号位 | 毫秒级时间戳 |  机器work id | 自增id |
 */
class CSnowFlake {
 public:
    enum ERROR_CODE_ENUM {
        OK  = 0,
        ERROR_INIT = -1,    // 初始化失败
        ERROR_TIMESTAMP_OVERFLOW = -2,    // 时间戳溢出范围
    };  // end enum ERROR_CODE_ENUM

    ~CSnowFlake() = default;

    // 构造单例
    static CSnowFlake& GetInstance() {
        static std::once_flag init_instance_flag_;
        std::call_once(init_instance_flag_, [&]() {
            instance_.reset(new CSnowFlake);
            instance_->Init();
        });
        return *instance_;
    }
    bool IsInitSucc();
    // 生成唯一id
    int GetUniqId(uint64_t&);

 private:
    CSnowFlake() : auto_incr_bit_(12), work_id_bit_(10), timestamp_ms_bit_(41), work_id_(0)
            , is_init_(false), timestamp_ms_offset_(1609430400000), last_timestamp_ms_(0)
            , last_auto_incr_id_(0) {
        max_timestamp_ms_ = 1ull << timestamp_ms_bit_;
        max_auto_incr_id_ = 1u << auto_incr_bit_;
    }

    CSnowFlake& operator = (const CSnowFlake&) = delete;
    CSnowFlake(const CSnowFlake&) = delete;

    void Init();

 private:
    const int auto_incr_bit_;  // 自增id二进制位数
    const int work_id_bit_;    // 机器word id二进制位数
    const int timestamp_ms_bit_;  // 毫秒级时间戳二进制位数
    int work_id_;   // 机器work id

    bool is_init_;    // 初始化成功标识

    uint64_t timestamp_ms_offset_;  // 时间戳偏移值，从较近的时间戳开始算，避免起始时时间戳二进制位数就偏大
    uint64_t last_timestamp_ms_;    // 上次处理的毫秒级时间戳
    uint64_t max_timestamp_ms_;     // 毫秒级时间戳上限
    int32_t last_auto_incr_id_;  // 上次自增id
    int32_t max_auto_incr_id_;   // 最大自增id

    static std::unique_ptr<CSnowFlake> instance_;  // 单例
    std::mutex mutex_;
};  // end class CSnowFlake
};  // end namespace hwf

#endif  // end #ifndef CPP_COMMON_UNIQ_ID_SNOW_FLAKE_HPP
