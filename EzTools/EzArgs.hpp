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

#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32) || defined(__WIN64__) || defined(WIN64) || defined(_WIN64) || defined(_MSC_VER)
#define WINDOWS_OS
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__APPLE__) || defined(__EMSCRIPTEN__)
#define LINUX_OS
#else
#define MAC_OS
#endif

#include <vector>
#include <string>
#include <cstdio>   // FILENAME_MAX
#include <cstdint>  // int32_t
#include <iostream>

namespace ez {

class Args {
private:
    enum class ValueType {
        None = 0,
        Bool,    // option is present ? value = true : value = false
        String,  // option value
        Int      // option value
    };

private:
    struct Option {
        std::string longOpt;
        std::string shortOpt;
        std::string helpText;
        bool required = true;
        ValueType type = ValueType::None;
        std::string value_string;
        bool value_bool = false;
        int32_t value_int = 0;
    };
    Option m_HelpOption;
    std::vector<std::string> m_Args;
    std::vector<Option> m_Options;

public:
    void addBoolOption(const std::string& vShortOpt, const std::string& vLongOpt, const std::string& vHelpText, const bool vDefaultValue = false) {
        auto opt = m_addOption(vShortOpt, vLongOpt, vHelpText, ValueType::Bool);
        opt.value_bool = vDefaultValue;
        m_Options.push_back(opt);
    }

    void addIntOption(const std::string& vShortOpt, const std::string& vLongOpt, const std::string& vHelpText, const int32_t vDefaultValue = false) {
        auto opt = m_addOption(vShortOpt, vLongOpt, vHelpText, ValueType::Int);
        opt.value_int = vDefaultValue;
        m_Options.push_back(opt);
    }

    void addStringOption(const std::string& vShortOpt, const std::string& vLongOpt, const std::string& vHelpText, const std::string& vDefaultValue = {}) {
        auto opt = m_addOption(vShortOpt, vLongOpt, vHelpText, ValueType::String);
        opt.value_string = vDefaultValue;
        m_Options.push_back(opt);
    }

    void addHelpOption(const std::string& vHelpText) {
        Option opt;
        opt.shortOpt = "-h";
        opt.longOpt = "--help";
        opt.helpText = vHelpText;
        m_HelpOption = opt;
    }

    bool parseOptions(int32_t vArgc, char** vArgv, int32_t vStartIdx = 1U) {
        for (int32_t idx = vStartIdx; idx < vArgc; ++idx) {
            std::string arg = vArgv[idx];
            auto last_minus = arg.find_last_of("-");
            if (last_minus != std::string::npos) {
                arg = arg.substr(last_minus + 1);
            }

            // print help
            if (arg == m_HelpOption.shortOpt ||  //
                arg == m_HelpOption.longOpt) {
                m_printHelp();
                return true;
            }

            // get args values
            for (auto& opt : m_Options) {
                if (arg == opt.shortOpt || //
                    arg == opt.longOpt) {
                    if (opt.type == ValueType::String) {
                        if (idx < (vArgc + 1)) {
                            ++idx;
                            opt.value_string = vArgv[idx];
                            break;
                        }
                    } else if (opt.type == ValueType::Bool) {
                        opt.value_bool = true;
                        break;
                    } else if (opt.type == ValueType::Int) {
                        opt.value_int = true;
                        break;
                    }
                }
            }
        }
        return true;
    }

    bool getBoolValue(const std::string& vKey) {
        auto* p_opt = m_getOptionPtr(vKey);
        if (p_opt != nullptr) {
            if (p_opt->type == ValueType::Bool) {
                return p_opt->value_bool;
            }
        }
        return {};
    }

    int32_t getIntValue(const std::string& vKey) {
        auto* p_opt = m_getOptionPtr(vKey);
        if (p_opt != nullptr) {
            if (p_opt->type == ValueType::Int) {
                return p_opt->value_int;
            }
        }
        return {};
    }

    const std::string& getStringValue(const std::string& vKey) {
        auto* p_opt = m_getOptionPtr(vKey);
        if (p_opt != nullptr) {
            if (p_opt->type == ValueType::String) {
                return p_opt->value_string;
            }
        }
        return {};
    }

private:
    Option m_addOption(const std::string& vShortOpt, const std::string& vLongOpt, const std::string& vHelpText, const ValueType vType) {
        Option opt;
        opt.shortOpt = vShortOpt;
        opt.longOpt = vLongOpt;
        opt.helpText = vHelpText;
        opt.type = vType;
        auto short_last_minus = vShortOpt.find_last_of("-");
        if (short_last_minus != std::string::npos) {
            opt.shortOpt = vShortOpt.substr(short_last_minus + 1);
        } else {
            opt.shortOpt = vShortOpt;
        }
        auto long_last_minus = vLongOpt.find_last_of("-");
        if (long_last_minus != std::string::npos) {
            opt.longOpt = vLongOpt.substr(long_last_minus + 1);
        } else {
            opt.longOpt = vLongOpt;
        }
        return opt;
    }

    Option* m_getOptionPtr(const std::string& vKey) {
        for (auto& opt : m_Options) {
            if (vKey == opt.shortOpt ||  //
                vKey == opt.longOpt) {
                return &opt;
            }
        }
        return nullptr;
    }

    void m_printHelp() const {
        std::cout << m_HelpOption.helpText << std::endl;
        std::cout << "Usage: \n";
        for (const auto& opt : m_Options) {
            std::cout << "  " << opt.shortOpt << " (" << opt.longOpt << "): " << opt.helpText << "\n";
        }
    }
};

}  // namespace ez
