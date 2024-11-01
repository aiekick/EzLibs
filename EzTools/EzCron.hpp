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

#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>

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
namespace time {

class Cron {
private:
    std::string m_CronExpr;
    std::vector<std::string> m_Field;

public:
    Cron(const std::string& vCronExpr) : m_CronExpr(vCronExpr) {
        m_Field = m_split(m_CronExpr);
#ifdef LogVarInfo
        LogVarInfo("Cron(%s)", vCronExpr.c_str());
#endif
    }
    bool isOk() const {
        if (m_Field.size() != 5) {
            std::cerr << "Invalid cron expression" << std::endl;
            return false;
        }
        return true;
    }
    bool isTimeToAct() const {
        return isTimeToAct(std::time(nullptr));
    }
    bool isTimeToAct(time_t vEpochTime) const {
#ifdef _MSC_VER
        struct tm _tm;
        localtime_s(&_tm, &vEpochTime);
        auto* time_info = &_tm;
#else
        struct tm* time_info = std::localtime(&vEpochTime);
#endif
#ifdef LogVarInfo
        LogVarInfo(                  //
            "%i-%i-%i %ih:%im:%is",  //
            time_info->tm_year + 1900,
            time_info->tm_mon + 1,
            time_info->tm_mday,
            time_info->tm_hour,
            time_info->tm_min,
            time_info->tm_sec);
#endif
        bool minute_match = m_matchField(m_Field[0], time_info->tm_min, 0, 59);
        if (minute_match) {
#ifdef LogVarInfo
            LogVarInfo("minute_match => true");
#endif
        }
        bool hour_match = m_matchField(m_Field[1], time_info->tm_hour, 0, 23);
        if (hour_match) {
#ifdef LogVarInfo
            LogVarInfo("hour_match => true");
#endif
        }
        bool day_match = m_matchField(m_Field[2], time_info->tm_mday, 1, 31);
        if (day_match) {
#ifdef LogVarInfo
            LogVarInfo("day_match => true");
#endif
        }
        bool month_match = m_matchField(m_Field[3], time_info->tm_mon + 1, 1, 12);  // tm_mon est de 0 à 11
        if (month_match) {
#ifdef LogVarInfo
            LogVarInfo("month_match => true");
#endif
        }
        bool weekday_match = m_matchField(m_Field[4], time_info->tm_wday, 0, 6);  // tm_wday est de 0 (dimanche) à 6
        if (weekday_match) {
#ifdef LogVarInfo
            LogVarInfo("weekday_match => true");
#endif
        }
        if (minute_match && hour_match && day_match && month_match && weekday_match) {
#ifdef LogVarInfo
            LogVarInfo("isTimeToAct() => true");
#endif
            return true;
        }
        return false;
    }

private:
    // Fonction pour séparer une chaîne de caractères par des espaces
    std::vector<std::string> m_split(const std::string& str) const {
        std::istringstream iss(str);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Fonction qui vérifie si un champ (min, heure, jour, mois, jour de la semaine) correspond au moment actuel
    bool m_matchField(const std::string& field, int current_value, int min_value, int max_value) const {
        if (field == "*") {
            return true;
        }
        std::istringstream iss(field);
        std::string token;
        while (std::getline(iss, token, ',')) {
            if (token.find('-') != std::string::npos) {
                int start, end;
#ifdef _MSC_VER
                sscanf_s(token.c_str(), "%d-%d", &start, &end);
#else
                sscanf(token.c_str(), "%d-%d", &start, &end);
#endif
                if (current_value >= start && current_value <= end) {
                    return true;
                }
            } else if (token.find('/') != std::string::npos) {
                int start, step;
#ifdef _MSC_VER
                sscanf_s(token.c_str(), "%d/%d", &start, &step);
#else
                sscanf(token.c_str(), "%d/%d", &start, &step);
#endif
                if ((current_value - start) % step == 0) {
                    return true;
                }
            } else {
                auto n = std::stoi(token);
                if (n > min_value && n < max_value) {
                    if (n == current_value) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

}  // namespace time
}  // namespace ez

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
