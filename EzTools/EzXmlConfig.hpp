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

#include <string>
#include <vector>
#include <fstream>


#include "EzXml.hpp"
#include "EzLog.hpp"

namespace ez {
namespace xml {

class Config {
public:
    virtual std::string getXml(const std::string& vOffset, const std::string& vUserDatas) = 0;
    // return true for continue xml parsing of childs in this node or false for interrupt the child exploration (if we want explore child ourselves)
    virtual bool setFromXml(const Node& vNode, const Node& vParent, const std::string& vUserDatas) = 0;

public:
    bool LoadConfigString(const std::string& vConfigString, const std::string& vUserDatas) {
        return parseConfigDatas(vConfigString, vUserDatas);
    }

    std::string SaveConfigString(const std::string& vUserDatas, const std::string& vFirstElement) {
        return "<" + vFirstElement + ">\n" + getXml("\t", vUserDatas) + "</" + vFirstElement + ">\n";
    }

    bool LoadConfigFile(const std::string& vFilePathName, const std::string& vUserDatas) {
        bool res = true; // if not file found its ok
        std::ifstream docFile(vFilePathName, std::ios::in);
        if (docFile.is_open()) {
            std::stringstream strStream;
            strStream << docFile.rdbuf();
            res = LoadConfigString(strStream.str(), vUserDatas);
            if (!res) {
                LogVarError("Config.xml file parsing failed");
            }
            docFile.close();
        }
        return res;
    }

    bool SaveConfigFile(const std::string& vFilePathName, const std::string& vUserDatas, const std::string& vFirstElement) {
        bool res = false;
        std::string data = SaveConfigString(vUserDatas, vFirstElement);
        std::ofstream configFileWriter(vFilePathName, std::ios::out);
        if (!configFileWriter.bad()) {
            configFileWriter << data;
            configFileWriter.close();
            res = true;
        }
        return res;
    }

    bool parseConfigDatas(const std::string& vDatas, const std::string& vUserDatas) {
        bool res = false;
        std::string datas = vDatas;
        Node::replaceAll(datas, "\r\n", "\n");
        ez::Xml doc;
        res = doc.parse(datas);
        if (res) {
            Node parent;
            RecursParsingConfig(doc.getRoot(), parent, vUserDatas);
        }
        return res;
    }

    void RecursParsingConfig(const Node& vNode, const Node& vParent, const std::string& vUserDatas) {
        if (setFromXml(vNode, vParent, vUserDatas)) {
            RecursParsingConfigChilds(vNode, vUserDatas);
        }
    }

    void RecursParsingConfigChilds(const Node& vNode, const std::string& vUserDatas) {
        // CHILDS
        // parse through all childs elements
        for (const auto& child : vNode.getChildren()) {
            RecursParsingConfig(child, vNode, vUserDatas);
        }
    }
};

}  // namespace xml
}  // namespace ez
