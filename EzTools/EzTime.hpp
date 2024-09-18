#pragma once

#include <ctime>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdint>

namespace ez {
namespace time {

// convert a string ISO8601 time to epoch time
inline bool iso8601ToEpoch(const std::string& vIsoDateTime, const std::string& vTimeFormat, std::time_t& vOutTime) {
    if (!vIsoDateTime.empty() && !vTimeFormat.empty()) {
        struct std::tm time = {};
        std::istringstream iss(vIsoDateTime);
        iss >> std::get_time(&time, vTimeFormat.c_str());
        if (!iss.fail()) {
            time.tm_hour = 0;
            time.tm_min = 0;
            time.tm_sec = 0;
#ifdef _WIN32
            vOutTime = _mkgmtime(&time);
#else
            vOutTime = timegm(&time);
#endif
            return true;
        }
    }
    return false;
}

// convert a epoch time to a string ISO8601 time
inline bool epochToISO8601(const std::time_t& vEpochTime, std::string& vOutTime) {
    auto tp = std::chrono::system_clock::from_time_t(vEpochTime);
    auto tt = std::chrono::system_clock::to_time_t(tp);
#ifdef _MSC_VER
    tm _timeinfo;
    tm* pTimeInfo = &_timeinfo;
    if (localtime_s(pTimeInfo, &tt) != 0) {
        return false;
    }
#else
    auto* pTimeInfo = std::localtime(&tt);
    #endif
    std::ostringstream oss;
    oss << std::put_time(pTimeInfo, "%Y-%m-%d");
    if (!oss.fail()) {
        vOutTime = oss.str();
        return true;
    }
    return false;
}

inline uint64_t getTicks() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

inline float getTimeInterval() {
    static auto lastTick = getTicks();
    const uint64_t ticks = getTicks();
    static float secMult = 1.0f / 1000.0f;
    const float interval = (ticks - lastTick) * secMult;
    lastTick = ticks;
    return interval;
}

class ActionTime {
private:
    uint64_t m_LastTick = 0U;
    uint64_t m_PauseTick = 0U;
    uint64_t m_ResumeTick = 0U;
    bool m_Play = true;

public:
    ActionTime() {
        m_LastTick = getTicks();
    }
    
    void fix() {  // fixe les marqueur de temps
        m_LastTick = getTicks();
        m_PauseTick = getTicks();
        m_ResumeTick = getTicks();
    }
    
    void fixTime(double vValue) {  // fixe les marqueur de temps
        fix();
        setTime(vValue);
    }
    
    void pause() {
        m_PauseTick = getTicks();
        m_Play = false;
    }
    
    void resume() {
        m_ResumeTick = getTicks();
        m_LastTick += m_ResumeTick - m_PauseTick;
        m_Play = true;
    }
    
    uint64_t get() const {
        return getTicks() - m_LastTick;
    }
    
    double getTime() const {
        static double secMult = 1e-3;
        return (getTicks() - m_LastTick) * secMult;
    }

    void setTime(double vValue){  // set le temps
        const auto v = (uint64_t)(vValue * 1000.0);
        m_LastTick = getTicks() - v;
    }

    // verifie si vMs millisecond depuis le dernier fix et donc si on peut agir
    // vFix permet de fixer le temps pour la prochaine action
    // on pourrait vouloir interroger sans vouloir permettre la prochaine action
    bool isTimeToAct(long vMs, bool vFix) {
        if (get() > (uint64_t)vMs) {
            if (vFix) {
                fix();
            }
            return true;
        }
        return false;
    }
};

}  // namespace time
}  // namespace ez