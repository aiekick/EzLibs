#include <TestEzMath.h>
#include <TestEzVec2.h>
#include <TestEzVec3.h>
#include <TestEzVec4.h>
#include <TestEzExpr.h>

#include <limits>
#include <cmath>

#define IfTestCollectionExist(v)             \
    if (vTest.find(#v) != std::string::npos) \
    return v(vTest)

bool TestMisc(const std::string& vTest) {
    IfTestCollectionExist(TestEzMath);
    else IfTestCollectionExist(TestEzVec2);
    else IfTestCollectionExist(TestEzVec3);
    else IfTestCollectionExist(TestEzVec4);
    else IfTestCollectionExist(TestEzExpr);
    return false;
}

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc > 1) {
        printf("Exec test : %s\n", argv[1]);
        return TestMisc(argv[1]) ? 0 : 1;
    }
    // User testing
    return TestMisc("TestEzExpr_Constants_Parsings_PI") ? 0 : 1;
}
