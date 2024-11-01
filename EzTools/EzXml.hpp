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

#include "EzStr.hpp"

namespace ez {
namespace xml {

class Node {
public:
    enum class Type {
        None = 0,
        Token,
        Comment
    };

private:
    std::string m_Name;
    std::map<std::string, std::string> m_Attributes;
    std::string m_Content;
    std::vector<Node> m_Children;
    Type m_Type = Type::None;

public:
    Node(const std::string& vName = "") : m_Name(vName) {
    }

    void setType(Type vType) {
        m_Type = vType;
    }

    Type getType() const {
        return m_Type;
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

    bool isAttributeExist(const std::string& vKey) const {
        return (m_Attributes.find(vKey) != m_Attributes.end());
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

    std::string dump(const xml::Node& vNode, const uint32_t vLevel = 0) const {
        std::string indent(vLevel * 2, ' ');  // Indentation based on the depth level
        std::ostringstream oss;

        oss << indent;
        if (vNode.getType() != xml::Node::Type::Comment) {
            oss << "<" << vNode.getName();
            for (const auto& attr : vNode.m_Attributes) {
                oss << " " << attr.first << "=\"" << xml::Node::unEscapeXml(attr.second) << "\"";
            }
        }

        const auto& content = vNode.getContent();
        const auto& children = vNode.getChildren();

        if (content.empty() && children.empty()) {
            oss << "/>" << std::endl;
        } else {
            if (vNode.getType() != xml::Node::Type::Comment) {
                oss << ">";
            }
            if (!content.empty()) {
                oss << xml::Node::unEscapeXml(content);
            }
            if (vNode.getType() == xml::Node::Type::Comment) {
                oss << std::endl;
            }
            if (!children.empty()) {
                oss << std::endl;
                for (const auto& child : children) {
                    oss << dump(child, vLevel + 1);
                }
                oss << indent;
            }
            if (vNode.getType() != xml::Node::Type::Comment) {
                oss << "</" << vNode.getName() << ">" << std::endl;
            }
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
    static std::string unEscapeXml(const std::string& vDatas) {
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
    enum class TokenType {  //
        OPENED = 0,
        CLOSED,
        OPENED_CLOSED,
        COMMENT,
        CONTENT,
        Count
    };

public:
    Xml(const std::string& vRootName = "root") : m_Root(vRootName) {
        m_NodeStack.push(&m_Root);
    }

    xml::Node& getRoot() {
        return m_Root;
    }

    bool parse(const std::string& vDoc) {
        auto tokens = m_tokenize(vDoc);
        if (tokens.empty()) {
            return false;
        }
        for (const auto& token : tokens) {
            std::string tagName;
            std::map<std::string, std::string> attributes;
            if (token.first.empty()) {
                continue;
            }
            if (token.second == TokenType::CLOSED) {
                m_NodeStack.pop();
            } else if (token.second == TokenType::OPENED ||         //
                       token.second == TokenType::OPENED_CLOSED ||  //
                       token.second == TokenType::COMMENT) {
                if (token.second != TokenType::COMMENT) {
                    tagName = m_extractTagName(token.first);
                }
                xml::Node newNode(tagName);
                newNode.setType(xml::Node::Type::Token);
                if (token.second == TokenType::COMMENT) {
                    newNode.setType(xml::Node::Type::Comment);
                    newNode.setContent(token.first);
                } else {
                    if (!m_extractAttributes(token.first, attributes)) {
                        return false;
                    }
                    for (const auto& kv : attributes) {
                        newNode.setAttribute(kv.first, kv.second);
                    }
                }
                m_NodeStack.top()->addChild(newNode);
                if (token.second == TokenType::OPENED) {
                    m_NodeStack.push(const_cast<xml::Node*>(&m_NodeStack.top()->getChildren().back()));
                }
            } else if (token.second == TokenType::CONTENT) {
                if (!m_NodeStack.empty()) {
                    if (token.first[0] != '\n') {
                        m_NodeStack.top()->setContent(token.first);
                    }
                }
            }
        }
        return true;
    }

    std::string dump() const {
        return m_Root.dump();
    }

private:
    std::vector<std::pair<std::string, TokenType>> m_tokenize(const std::string& vDoc) {
        std::vector<std::pair<std::string, TokenType>> tokens;
        size_t pos = 0;
        size_t length = vDoc.length();
        TokenType type = TokenType::Count;
        while (pos < length) {
            if (vDoc[pos] == '<') {
                type = TokenType::OPENED;
                if (vDoc[pos + 1] == '/') {
                    type = TokenType::CLOSED;
                } else if (vDoc[pos + 1] == '!') {
                    type = TokenType::COMMENT;
                }
                size_t end = vDoc.find(">", pos);
                if (end == std::string::npos) {
                    break;
                }
                if (vDoc[end - 1] == '/') {
                    type = TokenType::OPENED_CLOSED;
                }
                const auto ss = vDoc.substr(pos, end + 1 - pos);
                tokens.push_back(std::make_pair(ss, type));
                pos = end + 1;
            } else {
                size_t end = vDoc.find('<', pos);
                const auto ss = vDoc.substr(pos, end - pos);
                tokens.push_back(std::make_pair(ss, TokenType::CONTENT));
                pos = end;
            }
        }
        return tokens;
    }

    std::string m_extractTagName(const std::string& vLine) {
        size_t startPos = vLine.find('<') + 1;
        size_t endPos = vLine.find_first_of(" \t>", startPos);
        return vLine.substr(startPos, endPos - startPos);
    }

    bool m_extractAttributes(const std::string& vLine, std::map<std::string, std::string>& attributes) {
        size_t startPos = vLine.find(' ');
        while (startPos != std::string::npos) {
            startPos = vLine.find_first_not_of(" \t", startPos);
            size_t equalsPos = vLine.find('=', startPos);
            if (equalsPos == std::string::npos) {
                return false; 
            }
            std::string key = vLine.substr(startPos, equalsPos - startPos);
            startPos = equalsPos + 1;
            char quoteChar = vLine[startPos];
            if (quoteChar == '"' || quoteChar == '\'') {
                startPos++;
                size_t endPos = vLine.find(quoteChar, startPos);
                if (endPos != std::string::npos) {
                    std::string value = vLine.substr(startPos, endPos - startPos);
                    attributes[key] = value;
                    startPos = vLine.find(' ', endPos);  // Passer ? l'attribut suivant
                } else {
#ifdef LogVarError
                    LogVarError("The attribut '%s' have invalid value", key.c_str());
#endif
                    return false;  // Erreur : guillemets/apostrophes non ferm?s
                }
            } else {
#ifdef LogVarError
                LogVarError("The attribut '%s' have invalid value", key.c_str());
#endif
                return false;  // Erreur : attribut sans guillemets ou apostrophes
            }
        }
        return true;  // Extraction r?ussie
    }
};

}  // namespace ez
