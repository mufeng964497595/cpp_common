/*
 * File Created: Sunday, 7th February 2021 15:41:01
 * Author: huangwf (964497595@qq.com)
 * Desc: 工具类
 * -----
 * Copyright (c) 2021 Inc.
 */

#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <chrono>   // NOLINT
#include <string>

#include "facility.hpp"

// 获取当前机器IP
std::string hwf::utils::GetLocalIp() {
    if (socket(AF_INET, SOCK_DGRAM, 0) < 0) {
        return "";
    }
    char buf[512] = {0};
    struct ifconf ifconf;
    ifconf.ifc_len = sizeof(buf);
    ifconf.ifc_buf = buf;
    int sockfd = -1;
    if (ioctl(sockfd, SIOCGIFCONF, &ifconf)) {
        return "";
    }

    struct ifreq* ifreq_ptr = (struct ifreq*)buf;
    std::string local_addr("127.0.0.1");
    char ip[20] = {0};
    std::string local_ip("");
    for (int i = (ifconf.ifc_len / sizeof(struct ifreq)); i > 0; --i, ++ifreq_ptr) {
        if (AF_INET != ifreq_ptr->ifr_flags) {
            continue;
        }
        struct sockaddr_in* sin = (struct sockaddr_in*)&ifreq_ptr->ifr_addr;
        if (!inet_ntop(AF_INET, &(sin->sin_addr), ip, 16)) {
            continue;
        }
        if (local_addr != (std::string)(ip)) {
            local_ip = ip;
            break;
        }
    }

    return local_ip;
}

// 获取当前毫秒级时间戳
uint64_t hwf::utils::GetTimeStampMs() {
    uint64_t timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    return timestamp_ms;
}
