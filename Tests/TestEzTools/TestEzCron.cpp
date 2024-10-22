#include <TestEzTools/TestEzCron.h>
#include <EzTools/EzCron.hpp>
#include <EzTools/EzTime.hpp>

#include <iostream>
#include <string>

// D�sactivation des warnings de conversion
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

bool TestEzCron_0() {
    // Tous les 5 minutes
    if (ez::time::Cron("*/5 ** * *").isOk()) // 4 fiels
        return false;
    if (ez::time::Cron("*/5 * * * 1-8").isOk()) // 8 not exist
        return false;
    if (ez::time::Cron("*/5 * * * 1-8 *").isOk()) // 6 fields
        return false;
    if (ez::time::Cron("*/5 * * * gdfdfg").isOk()) // not valid char for days
        return false;
    return true;
}

bool TestEzCron_1() {
    // 18h from monday to friday
    auto cr = ez::time::Cron("0 18 * * 1-5");
    if (!cr.isOk())
        return false;
    struct tm time_info = {0};
    time_info.tm_year = 2024 - 1900;  // Année 2024
    time_info.tm_mon = 9;             // Mois d'octobre (tm_mon commence à 0)
    time_info.tm_mday = 22;           // 22 octobre
    time_info.tm_hour = 18;           // 18h
    time_info.tm_min = 0;             // 00 minutes
    time_info.tm_sec = 25;            // 00 secondes
    time_info.tm_isdst = 1;           // heure ete
    auto custom_epoch = std::mktime(&time_info);
    if (!cr.isTimeToAct(custom_epoch)) 
        return false;
    return true;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

bool TestEzCron(const std::string& vTest) {
    IfTestExist(TestEzCron_0);
    else IfTestExist(TestEzCron_1);
    return false;
}

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
