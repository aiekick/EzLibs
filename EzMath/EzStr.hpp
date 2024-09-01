#pragma once

#include <set>
#include <list>
#include <cmath>
#include <vector>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <cstdarg>

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)  // Conversion from 'double' to 'float', possible loss of data
#pragma warning(disable : 4305)  // Truncation from 'double' to 'float'
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

namespace ez {

inline ::std::string toStr(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char TempBuffer[3072 + 1];  // 3072 = 1024 * 3
    const int w = vsnprintf(TempBuffer, 3072, fmt, args);
    va_end(args);
    if (w) {
        return std::string(TempBuffer, (size_t)w);
    }
    return std::string();
}

template <typename T>
inline ::std::string toStr(T t) {
    std::ostringstream os;
    os << t;
    return os.str();
}

inline ::std::list<::std::string> SplitStringToList(const ::std::string& text, const std::string& delimiters, bool pushEmpty = false, bool vInversion = false) {
    ::std::list<::std::string> arr;
    if (!text.empty()) {
        ::std::string::size_type start = 0;
        ::std::string::size_type end = text.find_first_of(delimiters, start);
        while (end != ::std::string::npos) {
            ::std::string token = text.substr(start, end - start);
            if (!token.empty() || (token.empty() && pushEmpty)) {
                if (vInversion)
                    arr.emplace_front(token);
                else
                    arr.emplace_back(token);
            }
            start = end + 1;
            end = text.find_first_of(delimiters, start);
        }
        ::std::string token = text.substr(start);
        if (!token.empty() || (token.empty() && pushEmpty)) {
            if (vInversion)
                arr.emplace_front(token);
            else
                arr.emplace_back(token);
        }
    }
    return arr;
}

inline ::std::vector<::std::string> SplitStringToVector(const ::std::string& text, const std::string& delimiters, bool pushEmpty = false) {
    ::std::vector<::std::string> arr;
    if (!text.empty()) {
        ::std::string::size_type start = 0;
        ::std::string::size_type end = text.find_first_of(delimiters, start);
        while (end != ::std::string::npos) {
            ::std::string token = text.substr(start, end - start);
            if (!token.empty() || (token.empty() && pushEmpty))
                arr.emplace_back(token);
            start = end + 1;
            end = text.find_first_of(delimiters, start);
        }
        ::std::string token = text.substr(start);
        if (!token.empty() || (token.empty() && pushEmpty))
            arr.emplace_back(token);
    }
    return arr;
}

inline ::std::set<::std::string> SplitStringToSet(const ::std::string& text, const std::string& delimiters, bool pushEmpty = false) {
    ::std::set<::std::string> arr;
    if (!text.empty()) {
        ::std::string::size_type start = 0;
        ::std::string::size_type end = text.find_first_of(delimiters, start);
        while (end != ::std::string::npos) {
            ::std::string token = text.substr(start, end - start);
            if (!token.empty() || (token.empty() && pushEmpty))
                arr.emplace(token);
            start = end + 1;
            end = text.find_first_of(delimiters, start);
        }
        ::std::string token = text.substr(start);
        if (!token.empty() || (token.empty() && pushEmpty))
            arr.emplace(token);
    }
    return arr;
}

inline ::std::list<::std::string> SplitStringToList(const ::std::string& text, char delimiter, bool pushEmpty = false, bool vInversion = false) {
    ::std::list<::std::string> arr;
    if (!text.empty()) {
        ::std::string::size_type start = 0;
        ::std::string::size_type end = text.find(delimiter, start);
        while (end != ::std::string::npos) {
            ::std::string token = text.substr(start, end - start);
            if (!token.empty() || (token.empty() && pushEmpty)) {
                if (vInversion)
                    arr.emplace_front(token);
                else
                    arr.emplace_back(token);
            }
            start = end + 1;
            end = text.find(delimiter, start);
        }
        ::std::string token = text.substr(start);
        if (!token.empty() || (token.empty() && pushEmpty)) {
            if (vInversion)
                arr.emplace_front(token);
            else
                arr.emplace_back(token);
        }
    }
    return arr;
}

inline ::std::vector<::std::string> SplitStringToVector(const ::std::string& text, char delimiter, bool pushEmpty = false) {
    ::std::vector<::std::string> arr;
    if (!text.empty()) {
        ::std::string::size_type start = 0;
        ::std::string::size_type end = text.find(delimiter, start);
        while (end != ::std::string::npos) {
            ::std::string token = text.substr(start, end - start);
            if (!token.empty() || (token.empty() && pushEmpty))
                arr.emplace_back(token);
            start = end + 1;
            end = text.find(delimiter, start);
        }
        ::std::string token = text.substr(start);
        if (!token.empty() || (token.empty() && pushEmpty))
            arr.emplace_back(token);
    }
    return arr;
}

inline ::std::set<::std::string> SplitStringToSet(const ::std::string& text, char delimiter, bool pushEmpty = false) {
    ::std::set<::std::string> arr;
    if (!text.empty()) {
        ::std::string::size_type start = 0;
        ::std::string::size_type end = text.find(delimiter, start);
        while (end != ::std::string::npos) {
            ::std::string token = text.substr(start, end - start);
            if (!token.empty() || (token.empty() && pushEmpty))
                arr.emplace(token);
            start = end + 1;
            end = text.find(delimiter, start);
        }
        ::std::string token = text.substr(start);
        if (!token.empty() || (token.empty() && pushEmpty))
            arr.emplace(token);
    }
    return arr;
}

template <typename T>
inline bool StringToNumber(const std::string& vText, T& vNumber) {
    try {
        std::stringstream ss(vText);
        ss >> vNumber;
    } catch (std::exception&) {
        return false;
    }
    return true;
}

template <typename T>
inline std::vector<T> StringToNumberVector(const std::string& text, char delimiter) {
    std::vector<T> arr;
    std::string::size_type start = 0;
    std::string::size_type end = text.find(delimiter, start);
    T value;
    while (end != std::string::npos) {
        std::string token = text.substr(start, end - start);
        if (StringToNumber<T>(token, value)) {
            arr.emplace_back(value);
        }
        start = end + 1;
        end = text.find(delimiter, start);
    }
    arr.emplace_back(StringToNumber<T>(text.substr(start).c_str()));
    return arr;
}

}  // namespace ez

#include "EzVec2.hpp"
#include "EzVec3.hpp"
#include "EzVec4.hpp"
#include "EzAABB.hpp"
#include "EzQuat.hpp"
#include "EzPlane.hpp"
#include "EzAABBCC.hpp"
#include "EzVariant.hpp"

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(pop)
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
