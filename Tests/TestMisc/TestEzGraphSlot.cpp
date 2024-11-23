#include "TestEzGraphSlot.h"
#include <EzLibs/EzGraph.hpp>

using namespace ez;

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

#define IfTestCollectionExist(v, str)        \
    if (vTest.find(#v) != std::string::npos) \
    return v(str)

bool TestEzGraphSlot(const std::string& /*vTest*/) {
    // IfTestExist(Test_EzGraph_Building_0);
    // else IfTestExist(Test_EzGraph_Evaluation_0);
    return true;
}
