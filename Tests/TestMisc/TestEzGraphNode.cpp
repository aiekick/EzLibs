#include "TestEzGraphNode.h"
#include <ezlibs/ezGraph.hpp>

using namespace ez;

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v) \
    if (vTest == std::string(#v)) return v()

#define IfTestCollectionExist(v, str) \
    if (vTest.find(#v) != std::string::npos) return v(str)

bool TestEzGraphNode(const std::string& /*vTest*/) {
    //IfTestExist(Test_EzGraph_Building_0);
    //else IfTestExist(Test_EzGraph_Evaluation_0);
    return true;
}