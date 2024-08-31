#include "Test_EzGL.h"
#include "Test_EzGL_Uniforms.h"

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

#define IfTestCollectionExist(v, str)        \
    if (vTest.find(#v) != std::string::npos) \
    return v(str)

bool Test_EzGL(const std::string& vTest) {
    IfTestCollectionExist(Test_EzGL_Uniforms, vTest);
    return true;
}
