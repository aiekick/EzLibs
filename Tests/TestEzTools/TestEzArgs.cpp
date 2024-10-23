#include <TestEzTools/TestEzArgs.h>
#include <EzTools/EzArgs.hpp>

#include <iostream>
#include <string>
#include <array>

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

bool TestEzArgs_0() {
    std::vector<char*> arr{"-s", "toto.json", "--auto", "z"};
    ez::Args args;
    args.addStringOption("-s", "--source", "Source file");
    args.addBoolOption("-a", "--auto", "Auto mode");
    args.addBoolOption("-w", "--write", "Write");
    args.addBoolOption("z", "zozo", "zozo");
    args.addStringOption("-d", "--default", "Default option", "empty");
    if (!args.parseOptions(arr.size(), arr.data(), 0U))
        return false;
    if (args.getStringValue("s") != "toto.json")
        return false;
    if (args.getStringValue("source") != "toto.json") // the option "s" is active the related option "source" also
        return false;
    if (!args.getBoolValue("a"))
        return false;
    if (!args.getBoolValue("auto"))
        return false;
    if (args.getBoolValue("w"))
        return false;
    if (!args.getBoolValue("z"))
        return false;
    if (args.getStringValue("d") != "empty")
        return false;
    return true;
}

bool TestEzArgs_1() {
    return true;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

bool TestEzArgs(const std::string& vTest) {
    IfTestExist(TestEzArgs_0);
    else IfTestExist(TestEzArgs_1);
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
