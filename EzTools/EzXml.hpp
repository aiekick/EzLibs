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
#include <stack>
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

    std::string dump(const xml::Node& node, const uint32_t level = 0) const {
        std::string indent(level * 2, ' ');  // Indentation based on the depth level
        std::ostringstream oss;

        oss << indent << "<" << node.getName();

        for (const auto& attr : node.m_Attributes) {
            oss << " " << attr.first << "=\"" << xml::Node::escapeXml(attr.second) << "\"";
        }

        const auto& content = node.getContent();
        const auto& children = node.getChildren();

        if (content.empty() && children.empty()) {
            oss << "/>\n";
        } else {
            oss << ">";
            if (!content.empty()) {
                oss << xml::Node::escapeXml(content);
            }
            if (!children.empty()) {
                oss << "\n";
                for (const auto& child : children) {
                    oss << dump(child, level + 1);
                }
                oss << indent;
            }
            oss << "</" << node.getName() << ">\n";
        }

        return oss.str();
    }

    std::string dump() const {
        return dump(*this);
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
    // just during parsing,
    // for know what is the current node
    std::stack<xml::Node*> m_NodeStack;

public:
    Xml(const std::string& vRootName = "root") : m_Root(vRootName) {
        m_NodeStack.push(&m_Root);
    }

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

    std::string dump() const {
        return m_Root.dump();
    }

private:
    bool m_isOpeningTag(const std::string& vLine) {
        // Vérifie si la ligne commence par '<' et n'est ni une balise fermante ni une auto-fermante
        return vLine[0] == '<' && vLine[1] != '/' && vLine.back() != '/';
    }
    bool m_isClosingTag(const std::string& vLine) {
        // Vérifie si la ligne commence par '</'
        return vLine[0] == '<' && vLine[1] == '/';
    }
    bool m_isSelfClosingTag(const std::string& vLine) {
        // Vérifie si la ligne commence par '<' et se termine par '/>'
        return vLine[0] == '<' &&                         //
            (vLine.find("</", 1) != std::string::npos ||  //
             vLine.find("/>", 1) != std::string::npos);
    }

    void m_parseLine(const std::string& vLine) {
        std::string line = m_trim(vLine);
        if (line.empty()) {
            return;
        }
        if (m_isClosingTag(line)) {
            m_NodeStack.pop();
        } else if (m_isSelfClosingTag(line)) {
            std::string tagName = m_extractTagName(line);
            xml::Node newNode(tagName);
            auto attributes = m_extractAttributes(line);
            for (const auto& [key, value] : attributes) {
                newNode.setAttribute(key, value);
            }
            m_NodeStack.top()->addChild(newNode);
        } else if (m_isOpeningTag(line)) {
            std::string tagName = m_extractTagName(line);
            xml::Node newNode(tagName);
            auto attributes = m_extractAttributes(line);
            for (const auto& [key, value] : attributes) {
                newNode.setAttribute(key, value);
            }
            m_NodeStack.top()->addChild(newNode);
            m_NodeStack.push(const_cast<xml::Node*>(&m_NodeStack.top()->getChildren().back()));
        } else {
            if (!m_NodeStack.empty()) {
                m_NodeStack.top()->setContent(line);
            }
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

}  // namespace ez
