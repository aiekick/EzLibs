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

bool runTest(const std::string &cronExpr, bool expectedValidity, int expectedErrors) {
    ez::time::Cron cron(cronExpr);
    bool isValid = cron.isOk();
    int errorFlags = cron.getErrorFlags();

    std::cout << "Testing expression: \"" << cronExpr << "\"" << std::endl;
    std::cout << "Expected validity: " << (expectedValidity ? "Valid" : "Invalid") << std::endl;
    std::cout << "Actual validity: " << (isValid ? "Valid" : "Invalid") << std::endl;
    std::cout << "Expected error flags: " << expectedErrors << std::endl;
    std::cout << "Actual error flags: " << errorFlags << std::endl;

    if (!isValid) {
        std::cout << cron.getErrorMessage() << std::endl;
    }

    return ((isValid == expectedValidity) && (errorFlags == expectedErrors));
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool TestEzCron_Format_Invalid_Mins() {
    return runTest("60 * * * *", false, ez::time::Cron::INVALID_MINUTE);
}

bool TestEzCron_Format_Invalid_Hours() {
    return runTest("0 24 * * *", false, ez::time::Cron::INVALID_HOUR);
}

bool TestEzCron_Format_Invalid_MonthDay() {
    return runTest("0 * 32 * *", false, ez::time::Cron::INVALID_MONTH_DAY);
}

bool TestEzCron_Format_Invalid_Month() {
    return runTest("0 * * 13 *", false, ez::time::Cron::INVALID_MONTH);
}

bool TestEzCron_Format_Invalid_WeekDay() {
    return runTest("0 * * * 8", false, ez::time::Cron::INVALID_WEEK_DAY);
}

bool TestEzCron_Format_Invalid_ManyErrors_0() {
    return runTest("60 24 32 13 8",
                   false,
                   ez::time::Cron::INVALID_MINUTE |         //
                       ez::time::Cron::INVALID_HOUR |       //
                       ez::time::Cron::INVALID_MONTH_DAY |  //
                       ez::time::Cron::INVALID_MONTH |      //
                       ez::time::Cron::INVALID_WEEK_DAY);
}

bool TestEzCron_Format_Invalid_ManyErrors_1() {
    return runTest("62 28 35 32 68 12",
                   false,
                   ez::time::Cron::INVALID_MINUTE |         //
                       ez::time::Cron::INVALID_HOUR |       //
                       ez::time::Cron::INVALID_MONTH_DAY |  //
                       ez::time::Cron::INVALID_MONTH |      //
                       ez::time::Cron::INVALID_WEEK_DAY |   //
                       ez::time::Cron::WRONG_FIELDS_COUNT);
}

bool TestEzCron_Format_Invalid_FieldCount_0() {
    return runTest("0 0 1", false, ez::time::Cron::WRONG_FIELDS_COUNT);
}

bool TestEzCron_Format_Invalid_FieldCount_1() {
    return runTest("* * * * * *", false, ez::time::Cron::WRONG_FIELDS_COUNT);
}

bool TestEzCron_Format_Valid_Type_Value() {
    auto pat = "05 003 0009 005 0004";
    ez::time::Cron cr(pat);
    if (!cr.isOk()) return false;
    auto fields = cr.getFields();
    if (fields.size() != 5) return false;
    if (fields.at(0).value != 5) return false;
    if (fields.at(1).value != 3) return false;
    if (fields.at(2).value != 9) return false;
    if (fields.at(3).value != 5) return false;
    if (fields.at(4).value != 4) return false;
    return runTest(pat, true, ez::time::Cron::NONE);
}

bool TestEzCron_Format_inValid_Type_Value() {
    return runTest("@ _5 $9 ù9 %5", false, ez::time::Cron::NONE);
}

bool TestEzCron_Format_Valid_Type_Interval() {
    auto cr = ez::time::Cron("*5 * * * *");
    if (cr.isOk()) return false;
    if (cr.getErrorFlags() != (ez::time::Cron::INVALID_MINUTE | ez::time::Cron::INVALID_INTERVAL)) return false;
    cr = ez::time::Cron("*//5 * * * *");
    if (cr.isOk()) return false;
    if (cr.getErrorFlags() != (ez::time::Cron::INVALID_MINUTE | ez::time::Cron::INVALID_INTERVAL)) return false;
    cr = ez::time::Cron("*/ * * * *");
    if (cr.isOk()) return false;
    if (cr.getErrorFlags() != (ez::time::Cron::INVALID_MINUTE | ez::time::Cron::INVALID_INTERVAL)) return false;
    cr = ez::time::Cron("*/* * * * *");
    if (cr.isOk()) return false;
    if (cr.getErrorFlags() != (ez::time::Cron::INVALID_MINUTE | ez::time::Cron::INVALID_INTERVAL)) return false;
    return true;
}

bool TestEzCron_Format_InValid_Type_Interval() {
    return runTest("* * * * *", true, ez::time::Cron::NONE);
}

bool TestEzCron_Format_Valid_Type_Range() {
    return runTest("0 0 1 1 0", true, ez::time::Cron::NONE);
}

bool TestEzCron_Format_InValid_Type_Range() {
    return runTest("0 0 1 1 0", true, ez::time::Cron::NONE);
}

bool TestEzCron_Format_Valid_Type_Values() {
    return runTest("*/5 * * * *", true, ez::time::Cron::NONE);
}

bool TestEzCron_Format_InValid_Type_Values() {
    return runTest("*/5 * * * *", true, ez::time::Cron::NONE);
}

bool TestEzCron_TimeCheck_Valid() {
    // 18h from monday to friday
    auto cr = ez::time::Cron("18 0 * * 1-5");
    if (!cr.isOk()) return false;
    struct tm time_info {};
    time_info.tm_year = 2024 - 1900;  // Année 2024
    time_info.tm_mon = 9;             // Mois d'octobre (tm_mon commence à 0)
    time_info.tm_mday = 22;           // 22 octobre
    time_info.tm_hour = 18;           // 18h
    time_info.tm_min = 0;             // 00 minutes
    time_info.tm_sec = 25;            // 00 secondes
    time_info.tm_isdst = 1;           // heure ete
    auto custom_epoch = std::mktime(&time_info);
    return false;
    // cr.isTimeToAct(custom_epoch);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v) \
    if (vTest == std::string(#v)) return v()

bool TestEzCron(const std::string &vTest) {
    IfTestExist(TestEzCron_Format_Invalid_Mins);
    else IfTestExist(TestEzCron_Format_Invalid_Hours);
    else IfTestExist(TestEzCron_Format_Invalid_MonthDay);
    else IfTestExist(TestEzCron_Format_Invalid_Month);
    else IfTestExist(TestEzCron_Format_Invalid_WeekDay);
    else IfTestExist(TestEzCron_Format_Invalid_ManyErrors_0);
    else IfTestExist(TestEzCron_Format_Invalid_ManyErrors_1);
    else IfTestExist(TestEzCron_Format_Invalid_FieldCount_0);
    else IfTestExist(TestEzCron_Format_Invalid_FieldCount_1);
    else IfTestExist(TestEzCron_Format_Valid_Type_Value);
    else IfTestExist(TestEzCron_Format_inValid_Type_Value);
    else IfTestExist(TestEzCron_Format_Valid_Type_Interval);
    else IfTestExist(TestEzCron_Format_InValid_Type_Interval);
    else IfTestExist(TestEzCron_Format_Valid_Type_Range);
    else IfTestExist(TestEzCron_Format_InValid_Type_Range);
    else IfTestExist(TestEzCron_Format_Valid_Type_Values);
    else IfTestExist(TestEzCron_Format_InValid_Type_Values);
    else IfTestExist(TestEzCron_TimeCheck_Valid);
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
