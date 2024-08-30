#include <EzGraph/Test_EzGraph.h>

#include <EzGraph/core/Test_Slot.h>
#include <EzGraph/core/Test_Node.h>
#include <EzGraph/core/Test_Graph.h>

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v) \
    if (vTest == std::string(#v)) return v()

#define IfTestCollectionExist(v, str) \
    if (vTest.find(#v) != std::string::npos) return v(str)

bool Test_EzGraph(const std::string& vTest) {
    IfTestCollectionExist(Test_Slot, vTest);
    else IfTestCollectionExist(Test_Node, vTest);
    else IfTestCollectionExist(Test_Graph, vTest);
    return false;
}