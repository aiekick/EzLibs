#include <TestEzTools.h>

#include <TestEzMath.h>
#include <TestEzVec2.h>
#include <TestEzVec3.h>
#include <TestEzVec4.h>
#include <TestEzStr.h>
#include <TestEzXml.h>
#include <TestEzCron.h>
#include <TestEzArgs.h>
#include <TestEzGraph.h>

#include <limits>
#include <cmath>

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

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
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

#define IfTestCollectionExist(v)        \
    if (vTest.find(#v) != std::string::npos) \
    return v(vTest)

bool TestEzTools(const std::string& vTest) {
    IfTestCollectionExist(TestEzMath);
    else IfTestCollectionExist(TestEzVec2);
    else IfTestCollectionExist(TestEzVec3);
    else IfTestCollectionExist(TestEzVec4);
    else IfTestCollectionExist(TestEzStr);
    else IfTestCollectionExist(TestEzXml);
    else IfTestCollectionExist(TestEzCron);
    else IfTestCollectionExist(TestEzArgs);
    else IfTestCollectionExist(TestEzGraph);
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
