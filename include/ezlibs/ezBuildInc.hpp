#pragma once

#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>

namespace ez {

/*  FORMAT 
#pragma once

#define Project_Prefix "project"
#define Project_BuildNumber 3629
#define Project_MinorNumber 3
#define Project_MajorNumber 0
#define Project_BuildId "0.3.3629"
*/

class BuildInc {
private:
    std::string m_buildFileHeader;
    std::string m_project;
    std::string m_prefix;
    int32_t m_majorNumber = 0;
    int32_t m_minorNumber = 0;
    int32_t m_buildNumber = 0;

public:
    BuildInc(const std::string& vBuildFileHeader) {
        m_buildFileHeader = vBuildFileHeader;
        read();
    }
    BuildInc& read() {
        std::string content;
        std::ifstream docFile(m_buildFileHeader, std::ios::in);
        if (docFile.is_open()) {
            std::stringstream strStream;
            strStream << docFile.rdbuf();
             content =  strStream.str();
            docFile.close();
        } 
        if (!content.empty()) {
            size_t startLine = 0;
            size_t endLine = content.find('\n', startLine);
            std::string line;
            std::string project, key, value;
            while (endLine != std::string::npos) {
                line = content.substr(startLine, endLine - startLine);
                if (m_parseDefine(line, project, key, value)) {
                    m_project = project;  // overwrote each time but its the same for each
                    if (key == "Prefix") {
                        m_prefix = value;
                    } else if (key == "MajorNumber") {
                        m_majorNumber = m_toNumber(value);
                    } else if (key == "MinorNumber") {
                        m_minorNumber = m_toNumber(value);
                    } else if (key == "BuildNumber") {
                        m_buildNumber = m_toNumber(value);
                    }
                }
                startLine = endLine+1;
                endLine = content.find('\n', startLine);
            }
        }
        return *this;
    }
    const std::string& getProject() { return m_project; }
    const std::string& getPrefix() { return m_prefix; }
    int32_t getMajor() { return m_majorNumber; }
    int32_t getMinor() { return m_minorNumber; }
    int32_t getBuildNumber() { return m_buildNumber; }
    BuildInc& setProject(const std::string& vProject) {
        m_project = vProject;
        return *this;
    }
    BuildInc& setPrefix(const std::string& vPrefix) {
        m_prefix = vPrefix;
        return *this;
    }
    BuildInc& setMajor(const int32_t vMajorNumber) {
        m_majorNumber = vMajorNumber;
        return *this;
    }
    BuildInc& setMinor(const int32_t vMinorNumber) {
        m_minorNumber = vMinorNumber;
        return *this;
    }
    BuildInc& setBuildNumber(const int32_t vBuildNumber) {
        m_buildNumber = vBuildNumber;
        return *this;
    }
    BuildInc& incBuildNumber() {
        ++m_buildNumber;
        return *this;
    }
    BuildInc& write() {
        std::stringstream content;
        content << "#pragma once" << std::endl;
        content << std::endl;
        content << "#define " << m_project << "_Prefix \"" << m_prefix << "\"" << std::endl;
        content << "#define " << m_project << "_BuildNumber " << m_buildNumber << std::endl;
        content << "#define " << m_project << "_MinorNumber " << m_minorNumber << std::endl;
        content << "#define " << m_project << "_MajorNumber " << m_majorNumber << std::endl;
        content << "#define " << m_project << "_BuildId \"" << m_majorNumber << "." << m_minorNumber << "." << m_buildNumber << "\"" << std::endl;
        std::ofstream configFileWriter(m_buildFileHeader, std::ios::out);
        if (!configFileWriter.bad()) {
            configFileWriter << content.str();
            configFileWriter.close();
        }
        return *this;
    }

private:
    // will parse a line '#define [PROJECT]_[KEY] [VALUE]'
    // return true is succeed, false if the format is not recognized
    bool m_parseDefine(const std::string& vRowContent, std::string& vOutProject, std::string& vOutKey, std::string& vOutValue) {
        if (!vRowContent.empty()) {
            size_t def_pos = vRowContent.find("#define ");
            if (def_pos != std::string::npos) {
                def_pos += 8;  // offset for '#define '
                size_t underScore_pos = vRowContent.find('_', def_pos);
                if (underScore_pos != std::string::npos) {
                    vOutProject = vRowContent.substr(def_pos, underScore_pos - def_pos);
                    ++underScore_pos;  // offset for '_'
                    size_t space_pos = vRowContent.find(' ', underScore_pos);
                    if (space_pos != std::string::npos) {
                        vOutKey = vRowContent.substr(underScore_pos, space_pos - underScore_pos);
                        ++space_pos;  // offset for ' '
                        vOutValue = m_trim(vRowContent.substr(space_pos));
                        return true;
                    }
                }
            }
        }
        return false;
    }
    int32_t m_toNumber(const std::string& vNum) {
        int32_t ret = 0; // 0 is the default value
        try {
            ret = std::stoi(vNum);
        } catch (...) {
        }
        return ret;
    }
    // will remove quotes
    std::string m_trim(const std::string& vValue) {
        std::string ret;
        for (auto& c : vValue) {
            if (c != '\"') {
                ret += c;
            }
        }
        return ret;
    }
};

}  // namespace ez
