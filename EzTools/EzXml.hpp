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

#include <map>
#include <string>
#include <vector>
#include <sstream>

namespace ez {
namespace xml {

class Node {
private:
    std::string m_Name;
    std::map<std::string, std::string> m_Attributes;
    std::string m_Content;
    std::vector<Node> m_Children;

public:
    Node(const std::string& vName = "") : m_Name(vName) {
    }

    void addChild(const Node& vChild) {
        m_Children.push_back(vChild);
    }

    Node& addChild(const std::string& vName) {
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

    const std::vector<Node>& getChildren() const {
        return m_Children;
    }

    const std::string& getName() const {
        return m_Name;
    }

public:
    static std::string escapeXml(const std::string& vDatas) {
        std::string escaped = vDatas;
        replaceAll(escaped, "&", "&amp;");
        replaceAll(escaped, "<", "&lt;");
        replaceAll(escaped, "\"", "&quot;");
        replaceAll(escaped, "'", "&apos;");
        replaceAll(escaped, ">", "&gt;");
        return escaped;
    }

    // replace xml excaped pattern by corresponding good pattern
    static std::string unEscapeXmlCode(const std::string& vDatas) {
        std::string unescaped = vDatas;
        replaceAll(unescaped, "&lt;", "<");
        replaceAll(unescaped, "&amp;", "&");
        replaceAll(unescaped, "&quot;", "\"");
        replaceAll(unescaped, "&apos;", "'");
        replaceAll(unescaped, "&gt;", ">");
        return unescaped;
    }

    static void replaceAll(std::string& vStr, const std::string& vFrom, const std::string& vTo) {
        if (vFrom.empty())
            return;
        size_t startPos = 0;
        while ((startPos = vStr.find(vFrom, startPos)) != std::string::npos) {
            vStr.replace(startPos, vFrom.length(), vTo);
            startPos += vTo.length();
        }
    }
};

}  // namespace xml

class Xml {
private:
    xml::Node m_Root;

public:
    Xml(const std::string& vRootName = "root") : m_Root(vRootName) {}

    xml::Node& getRoot() {
        return m_Root;
    }

    bool parse(const std::string& vDoc) {
        std::istringstream doc(vDoc);
        std::string line;
        while (std::getline(doc, line)) {
            m_parseLine(line);
        }
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
            xml::Node newNode(tagName);
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
            if (equalsPos == std::string::npos) {
                break;
            }
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
