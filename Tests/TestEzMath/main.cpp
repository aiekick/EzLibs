#include <TestEzMath.h>
#include <TestEzVec2.h>
#include <TestEzVec3.h>
#include <TestEzVec4.h>

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

#define IfTestCollectionExist(v, str)        \
    if (vTest.find(#v) != std::string::npos) \
    return v(str)

bool TestMain(const std::string& vTest) {
    IfTestCollectionExist(TestEzMath, vTest);
    else IfTestCollectionExist(TestEzVec2, vTest);
    else IfTestCollectionExist(TestEzVec3, vTest);
    else IfTestCollectionExist(TestEzVec4, vTest);
    return false;
}

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc > 1) {
        printf("Exec test : %s\n", argv[1]);
        return TestMain(argv[1]) ? 0 : 1;
    }
    // User testing
    return TestMain("TestEzMathRound<int32_t>") ? 0 : 1;
}
