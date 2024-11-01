#pragma once

/*
MIT License

Copyright (c) 2024 Stephane Cuillerdier (aka aiekick)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#include "EzStr.hpp"

#ifndef EZ_FILE_SLASH_TYPE
#ifdef WIN32
#define EZ_FILE_SLASH_TYPE "\\"
#else // UNIX
#define EZ_FILE_SLASH_TYPE "/"
#endif
#endif // EZ_FILE_SLASH_TYPE

namespace ez {
namespace file {

inline std::string loadFileToString(const std::string& vFilePathName) {
    std::string ret;
    std::ifstream docFile(vFilePathName, std::ios::in);
    if (docFile.is_open()) {
        std::stringstream strStream;
        strStream << docFile.rdbuf();  // read the file
        ret = strStream.str();
        ez::str::replaceString(ret, "\r\n", "\n");
        ez::str::replaceString(ret, "\r", "\n");
        docFile.close();
    } else  {
#ifdef EZ_TOOLS_LOG
        LogVarError("File \"%s\" Not Found !\n", vFilePathName.c_str());
#endif
    }
    return ret;
}

inline bool saveStringToFile(const std::string& vDatas, const std::string& vFilePathname, bool vAddTimeStamp = false) {
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

/* correct file path between os and different slash type between window and unix */
inline std::string correctSlashTypeForFilePathName(const std::string& vFilePathName) {
    std::string res = vFilePathName;
    ez::str::replaceString(res, "\\", EZ_FILE_SLASH_TYPE);
    ez::str::replaceString(res, "/", EZ_FILE_SLASH_TYPE);
    return res;
}

struct PathInfos {
    std::string path;
    std::string name;
    std::string ext;
    bool isOk = false;

    PathInfos() {
        isOk = false;
    }

    PathInfos(const std::string& vPath, const std::string& vName, const std::string& vExt) {
        isOk = true;
        path = vPath;
        name = vName;
        ext = vExt;

        if (ext.empty()) {
            const size_t lastPoint = name.find_last_of('.');
            if (lastPoint != std::string::npos) {
                ext = name.substr(lastPoint + 1);
                name = name.substr(0, lastPoint);
            }
        }
    }

    std::string GetFPNE() {
        return GetFPNE_WithPathNameExt(path, name, ext);
    }

    std::string GetFPNE_WithPathNameExt(std::string vPath, const std::string& vName, const std::string& vExt) {
        if (vPath[0] == EZ_FILE_SLASH_TYPE[0]) {
#ifdef WIN32
            // if it happening on window this seem that this path msut be a relative path but with an error
            vPath = vPath.substr(1);  // bad formated path go relative
#endif
        } else {
#ifdef UNIX
            vPath = "/" + vPath;  // make it absolute
#endif
        }

        if (vPath.empty())
            return vName + "." + vExt;

        return vPath + EZ_FILE_SLASH_TYPE + vName + "." + vExt;
    }

    std::string GetFPNE_WithPath(const std::string& vPath) {
        return GetFPNE_WithPathNameExt(vPath, name, ext);
    }

    std::string GetFPNE_WithPathName(const std::string& vPath, const std::string& vName) {
        return GetFPNE_WithPathNameExt(vPath, vName, ext);
    }

    std::string GetFPNE_WithPathExt(const std::string& vPath, const std::string& vExt) {
        return GetFPNE_WithPathNameExt(vPath, name, vExt);
    }

    std::string GetFPNE_WithName(const std::string& vName) {
        return GetFPNE_WithPathNameExt(path, vName, ext);
    }

    std::string GetFPNE_WithNameExt(const std::string& vName, const std::string& vExt) {
        return GetFPNE_WithPathNameExt(path, vName, vExt);
    }

    std::string GetFPNE_WithExt(const std::string& vExt) {
        return GetFPNE_WithPathNameExt(path, name, vExt);
    }
};

inline PathInfos parsePathFileName(const std::string& vPathFileName) {
    PathInfos res;
    if (!vPathFileName.empty()) {
        const std::string pfn = correctSlashTypeForFilePathName(vPathFileName);
        if (!pfn.empty()) {
            const size_t lastSlash = pfn.find_last_of(EZ_FILE_SLASH_TYPE);
            if (lastSlash != std::string::npos) {
                res.name = pfn.substr(lastSlash + 1);
                res.path = pfn.substr(0, lastSlash);
                res.isOk = true;
            }
            const size_t lastPoint = pfn.find_last_of('.');
            if (lastPoint != std::string::npos) {
                if (!res.isOk) {
                    res.name = pfn;
                    res.isOk = true;
                }
                res.ext = pfn.substr(lastPoint + 1);
                ez::str::replaceString(res.name, "." + res.ext, "");
            }
            if (!res.isOk) {
                res.name = pfn;
                res.isOk = true;
            }
        }
    }
    return res;
}

inline std::string simplifyFilePath(const std::string& vFilePath) {
    std::string newPath = correctSlashTypeForFilePathName(vFilePath);
    // the idea is to simplify a path where there is some ..
    // by ex : script\\kifs\\../space3d.glsl => can be simplified in /script/space3d.glsl
    size_t dpt = 0;
    while ((dpt = newPath.find("..")) != std::string::npos) {
        ez::str::replaceString(newPath, "\\", EZ_FILE_SLASH_TYPE);
        ez::str::replaceString(newPath, "/", EZ_FILE_SLASH_TYPE);
        size_t sl = newPath.rfind(EZ_FILE_SLASH_TYPE, dpt);
        if (sl != std::string::npos) {
            if (sl > 0) {
                sl = newPath.rfind(EZ_FILE_SLASH_TYPE, sl - 1);
                if (sl != std::string::npos) {
                    std::string str = newPath.substr(sl, dpt + 2 - sl);
                    ez::str::replaceString(newPath, str, "");
                }
            }
        } else {
            break;
        }
    }
    return newPath;
}

}  // namespace file
}  // namespace ez