#include "Test_glApi.h"
#include "Test_glApi_Uniforms.h"

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

#define IfTestCollectionExist(v, str)        \
    if (vTest.find(#v) != std::string::npos) \
    return v(str)

bool Test_glApi(const std::string& vTest) {
    IfTestCollectionExist(Test_glApi_Uniforms, vTest);
    return true;
}
