#pragma once
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace ez {

class XmlNode {
private:
    std::string m_Name;
    std::map<std::string, std::string> m_Attributes;
    std::string m_Content;
    std::vector<XmlNode> m_Children;

public:
    XmlNode(const std::string& vName = "") : m_Name(vName) {}

    void addChild(const XmlNode& vChild) {
        m_Children.push_back(vChild);
    }

    XmlNode& addChild(const std::string& vName) {
        m_Children.emplace_back(vName);
        return m_Children.back();
    }

    void setAttribute(const std::string& vKey, const std::string& vValue) {
        m_Attributes[vKey] = vValue;
    }

    std::string getAttribute(const std::string& vKey) const {
        auto it = m_Attributes.find(vKey);
        if (it != m_Attributes.end()) {
            return it->second;
        }
        return "";
    }

    void setContent(const std::string& vContent) {
        m_Content = vContent;
    }

    const std::string& getContent() const {
        return m_Content;
    }

    const std::vector<XmlNode>& getChildren() const {
        return m_Children;
    }

    const std::string& getName() const {
        return m_Name;
    }

private:
    static std::string m_escapeXml(const std::string& vData) {
        std::string escaped = vData;
        m_replaceAll(escaped, "&", "&amp;");
        m_replaceAll(escaped, "<", "&lt;");
        m_replaceAll(escaped, ">", "&gt;");
        m_replaceAll(escaped, "\"", "&quot;");
        m_replaceAll(escaped, "'", "&apos;");
        return escaped;
    }

    static void m_replaceAll(std::string& vStr, const std::string& vFrom, const std::string& vTo) {
        if (vFrom.empty()) return;
        size_t startPos = 0;
        while ((startPos = vStr.find(vFrom, startPos)) != std::string::npos) {
            vStr.replace(startPos, vFrom.length(), vTo);
            startPos += vTo.length();
        }
    }
};

class Xml {
private:
    XmlNode m_Root;

public:
    Xml(const std::string& vRootName = "root") : m_Root(vRootName) {}

    XmlNode& getRoot() {
        return m_Root;
    }

    bool loadFromFile(const std::string& vFilename) {
        std::ifstream file(vFilename);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            m_parseLine(line);
        }

        file.close();
        return true;
    }

private:
    void m_parseLine(const std::string& vLine) {
        std::string trimmedLine = m_trim(vLine);

        if (trimmedLine.empty()) {
            return;
        }

        if (trimmedLine[0] == '<' && trimmedLine[1] != '/') {
            std::string tagName = m_extractTagName(trimmedLine);
            XmlNode newNode(tagName);

            auto attributes = m_extractAttributes(trimmedLine);
            for (const auto& [key, value] : attributes) {
                newNode.setAttribute(key, value);
            }

            m_Root.addChild(newNode);
        }
    }

    std::string m_extractTagName(const std::string& vLine) {
        size_t startPos = vLine.find('<') + 1;
        size_t endPos = vLine.find_first_of(" \t>", startPos);
        return vLine.substr(startPos, endPos - startPos);
    }

    std::map<std::string, std::string> m_extractAttributes(const std::string& vLine) {
        std::map<std::string, std::string> attributes;
        size_t startPos = vLine.find(' ');

        while (startPos != std::string::npos) {
            startPos = vLine.find_first_not_of(" \t", startPos);
            size_t equalsPos = vLine.find('=', startPos);
            if (equalsPos == std::string::npos) break;

            std::string key = vLine.substr(startPos, equalsPos - startPos);
            startPos = vLine.find('"', equalsPos) + 1;
            size_t endPos = vLine.find('"', startPos);

            std::string value = vLine.substr(startPos, endPos - startPos);
            attributes[key] = value;

            startPos = vLine.find(' ', endPos);
        }

        return attributes;
    }

    std::string m_trim(const std::string& vStr) {
        const std::string whitespace = " \t\n\r";
        size_t start = vStr.find_first_not_of(whitespace);
        size_t end = vStr.find_last_not_of(whitespace);
        if (start == std::string::npos || end == std::string::npos) {
            return "";
        }
        return vStr.substr(start, end - start + 1);
    }
};

} // namespace ez
