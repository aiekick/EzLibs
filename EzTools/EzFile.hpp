#pragma once

#include <fstream>
#include <string>
#include <ctime>

#include "EzStr.hpp"

namespace ez {
namespace file {

inline bool saveStringToFile(const std::string& vFilePathname, const std::string& vDatas, bool vAddTimeStamp = false) {
    std::string fpn = vFilePathname;
    if (!fpn.empty()) {
        if (vAddTimeStamp) {
            auto dot_p = fpn.find_last_of('.');
            time_t epoch = std::time(nullptr);
            if (dot_p != std::string::npos) {
                fpn = fpn.substr(0, dot_p) + ez::str::toStr("_%llu", epoch) + fpn.substr(dot_p);
            } else {
                fpn += ez::str::toStr("_%llu", epoch);
            }
        }
        std::ofstream configFileWriter(fpn, std::ios::out);
        if (!configFileWriter.bad()) {
            configFileWriter << vDatas;
            configFileWriter.close();
            return true;
        }
    }
    return false;
}

}  // namespace file
}  // namespace ez