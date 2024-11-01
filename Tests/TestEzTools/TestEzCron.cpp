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

    return (isValid == expectedValidity && errorFlags == expectedErrors);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Wildcard dans tous les champs
bool TestEzCron_Format_Valid_Wildcard() {
    return runTest("* * * * *", true, ez::time::Cron::NONE);
}

// Expression cron exacte pour un jour particulier
bool TestEzCron_Format_Valid_Nums() {
    return runTest("0 0 1 1 0", true, ez::time::Cron::NONE);
}

// Intervalles valides
bool TestEzCron_Format_Valid_Intervals() {
    return runTest("*/5 * * * *", true, ez::time::Cron::NONE);
}

// Minute invalide (60 n'est pas une minute valide)
bool TestEzCron_Format_Invalid_Mins() {
    return runTest("60 * * * *", false, ez::time::Cron::INVALID_MINUTE);
}

// Heure invalide (24 n'est pas une heure valide)
bool TestEzCron_Format_Invalid_Hours() {
    return runTest("0 24 * * *", false, ez::time::Cron::INVALID_HOUR);
}

// Jour du mois invalide (32 n'est pas un jour valide)
bool TestEzCron_Format_Invalid_MonthDay() {
    return runTest("0 * 32 * *", false, ez::time::Cron::INVALID_MONTH_DAY);
}

// Mois invalide (13 n'est pas un mois valide)
bool TestEzCron_Format_Invalid_Month() {
    return runTest("0 * * 13 *", false, ez::time::Cron::INVALID_MONTH);
}

// Jour de la semaine invalide (7 n'est pas valide)
bool TestEzCron_Format_Invalid_WeekDay() {
    return runTest("0 * * * 7", false, ez::time::Cron::INVALID_WEEK_DAY);
}

// Cas de tests avec plusieurs erreurs
bool TestEzCron_Format_Invalid_ManyErrors_0() {
    return runTest("60 24 32 13 7",
                   false,
                   ez::time::Cron::INVALID_MINUTE |         //
                       ez::time::Cron::INVALID_HOUR |       //
                       ez::time::Cron::INVALID_MONTH_DAY |  //
                       ez::time::Cron::INVALID_MONTH |      //
                       ez::time::Cron::INVALID_WEEK_DAY);
}
bool TestEzCron_Format_Invalid_ManyErrors_1() {
    return runTest("03 17 22 32 68 12",
                   false,
                   ez::time::Cron::INVALID_MINUTE |         //
                       ez::time::Cron::INVALID_HOUR |       //
                       ez::time::Cron::INVALID_MONTH_DAY |  //
                       ez::time::Cron::INVALID_MONTH |      //
                       ez::time::Cron::INVALID_WEEK_DAY |   //
                       ez::time::Cron::WRONG_FIELDS_COUNT);
}

// Cas avec un nombre de champs incorrect
bool TestEzCron_Format_Invalid_FieldCount_0() {
    return runTest("0 0 1", false, ez::time::Cron::WRONG_FIELDS_COUNT);  // Nombre incorrect de champs
}
bool TestEzCron_Format_Invalid_FieldCount_1() {
    return runTest("* * * * * *", true, ez::time::Cron::WRONG_FIELDS_COUNT);  // Nombre incorrect de champs
}

bool TestEzCron_TimeCheck_Valid() {
    // 18h from monday to friday
    auto cr = ez::time::Cron("18 0 * * 1-5");
    if (!cr.isOk())
        return false;
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
    //cr.isTimeToAct(custom_epoch);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

bool TestEzCron(const std::string &vTest) {
    IfTestExist(TestEzCron_Format_Valid_Wildcard);
    else IfTestExist(TestEzCron_Format_Valid_Nums);
    else IfTestExist(TestEzCron_Format_Valid_Intervals);
    else IfTestExist(TestEzCron_Format_Invalid_Mins);
    else IfTestExist(TestEzCron_Format_Invalid_Hours);
    else IfTestExist(TestEzCron_Format_Invalid_MonthDay);
    else IfTestExist(TestEzCron_Format_Invalid_Month);
    else IfTestExist(TestEzCron_Format_Invalid_WeekDay);
    else IfTestExist(TestEzCron_Format_Invalid_ManyErrors_0);
    else IfTestExist(TestEzCron_Format_Invalid_ManyErrors_1);
    else IfTestExist(TestEzCron_Format_Invalid_FieldCount_0);
    else IfTestExist(TestEzCron_Format_Invalid_FieldCount_1);
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
