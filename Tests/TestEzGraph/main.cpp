#include <TestEzGraph.h>

#include <TestEzGraphSlot.h>
#include <TestEzGraphNode.h>
#include <TestEzGraph.h>

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v) \
    if (vTest == std::string(#v)) return v()

#define IfTestCollectionExist(v, str) \
    if (vTest.find(#v) != std::string::npos) return v(str)

bool TestEzGraph(const std::string& vTest) {
    IfTestCollectionExist(TestEzGraphSlot, vTest);
    else IfTestCollectionExist(TestEzGraphNode, vTest);
    else IfTestCollectionExist(TestEzGraph, vTest);
    return false;
}

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc > 1) {
        printf("Exec test : %s\n", argv[1]);
        return TestEzGraph(argv[1]) ? 0 : 1;
    }
    // User testing
    return TestEzGraph("") ? 0 : 1;
}
