#include "Test_glApi_Uniforms.h"
#include "glApi.hpp"

////////////////////////////////////////////////////////////////////////////
//// GOOD SYNTAX ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool Test_glApi_Uniforms_Parsing_Good_Syntax_0() {
    return glApi::UniformParsingDatas("/*ds\\nddfs*/uniform/*ds\\nddfs*/float/*ds\\nddfs*/toto/*dsd\\ndfs*/;/*dsd\\ndfs*/").isValid();
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_1() {
    return glApi::UniformParsingDatas("uniform float toto;").isValid();
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_2() {
    return glApi::UniformParsingDatas("uniform float toto ;").isValid();
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_3() {
    return glApi::UniformParsingDatas("uniform float toto; // toto").isValid();
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_4() {
    return glApi::UniformParsingDatas("uniform float toto; // toto \\n tata;").isValid();
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_5() {
    return glApi::UniformParsingDatas("uniform float toto; /* toto \\n tata; */").isValid();
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_6() {
    return glApi::UniformParsingDatas("/* lkj\\ndfghxkl */ uniform float toto ; /* sdfsd\\nfsdfsdf */").isValid();
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_7() {
    return glApi::UniformParsingDatas(
               "/* lkjdf \\nghxkl */ uniform /* d \\nsdfdf */ float /* qs \\ndsddf */ toto /* lkjdf \\nghxkl */ ; /* lkjdf \\nghxkl */ ")
        .isValid();
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_8() {
    return glApi::UniformParsingDatas(" uniform float toto;").isValid();
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_9() {
    return glApi::UniformParsingDatas(" uniform float(titi) toto;").isValid();
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_10() {
    auto upd = glApi::UniformParsingDatas(" uniform float(titi:one:two:three) toto; /* kjhdfgkshk */");
    if (!upd.isValid()) {
        return false;
    }
    if (upd.uniform_type != "float") {
        return false;
    }
    if (upd.widget_type != "titi") {
        return false;
    }
    if (upd.uniform_name != "toto") {
        return false;
    }
    if (upd.params.size() != 3U) {
        return false;
    }
    if (upd.params.size() > 0 && upd.params[0] != "one") {
        return false;
    }
    if (upd.params.size() > 1 && upd.params[1] != "two") {
        return false;
    }
    if (upd.params.size() > 2 && upd.params[2] != "three") {
        return false;
    }
    if (upd.uniform_comment_original != " kjhdfgkshk ") {
        return false;
    }
}

bool Test_glApi_Uniforms_Parsing_Good_Syntax_11() {
    auto upd = glApi::UniformParsingDatas(" uniform float(titi) toto; /* kjhdfgkshk */");
    if (!upd.isValid()) {
        return false;
    }
    if (upd.uniform_type != "float") {
        return false;
    }
    if (upd.widget_type != "titi") {
        return false;
    }
    if (upd.uniform_name != "toto") {
        return false;
    }
    if (upd.params.size() != 0U) {
        return false;
    }
    if (upd.uniform_comment_original != " kjhdfgkshk ") {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////
//// BAD SYNTAX ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool Test_glApi_Uniforms_Parsing_Bad_Syntax_0() {
    return !glApi::UniformParsingDatas("").isValid();
}

bool Test_glApi_Uniforms_Parsing_Bad_Syntax_1() {
    return !glApi::UniformParsingDatas("//uniform float toto;").isValid();
}

bool Test_glApi_Uniforms_Parsing_Bad_Syntax_2() {
    return !glApi::UniformParsingDatas("// uniform float toto;").isValid();
}

bool Test_glApi_Uniforms_Parsing_Bad_Syntax_3() {
    return !glApi::UniformParsingDatas("uniform /* toto;").isValid();
}

bool Test_glApi_Uniforms_Parsing_Bad_Syntax_4() {
    return !glApi::UniformParsingDatas("uniform float ;").isValid();
}

bool Test_glApi_Uniforms_Parsing_Bad_Syntax_5() {
    return !glApi::UniformParsingDatas("uniform float toto").isValid();
}

bool Test_glApi_Uniforms_Parsing_Bad_Syntax_6() {
    return !glApi::UniformParsingDatas(" float toto;").isValid();
}

bool Test_glApi_Uniforms_Parsing_Bad_Syntax_7() {
    return !glApi::UniformParsingDatas("unifrm float toto;").isValid();
}

////////////////////////////////////////////////////////////////////////////
//// TIME WIDGET ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool Test_glApi_Uniforms_Parsing_Time_Widget_0() {
    auto upd = glApi::UniformParsingDatas("uniform float(time:0.0) uTime;");
    if (!upd.isValid()) {
        return false;
    }
    if (upd.uniform_type != "float") {
        return false;
    }
    if (upd.widget_type != "time") {
        return false;
    }
    if (upd.uniform_name != "uTime") {
        return false;
    }
    if (upd.params.size() != 1U) {
        return false;
    }
    if (upd.params.size() > 0 && upd.params[0] != "0.0") {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////
//// BUFFER WIDGET /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool Test_glApi_Uniforms_Parsing_Buffer_Widget_0() {
    auto upd = glApi::UniformParsingDatas("uniform vec2(buffer) uResolution;");
    if (!upd.isValid()) {
        return false;
    }
    if (upd.uniform_type != "vec2") {
        return false;
    }
    if (upd.widget_type != "buffer") {
        return false;
    }
    if (upd.uniform_name != "uResolution") {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////
//// SLIADER WIDGET ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool Test_glApi_Uniforms_Parsing_Slider_Widget_0() {
    auto upd = glApi::UniformParsingDatas("uniform float(0.0:1.0:0.5:0.1) uSlider;");
    if (!upd.isValid()) {
        return false;
    }
    if (upd.uniform_type != "float") {
        return false;
    }
    if (upd.widget_type != "slider") {
        return false;
    }
    if (upd.params.size() != 4U) {
        return false;
    }
    if (upd.params.size() > 0 && upd.params[0] != "0.0") {
        return false;
    }
    if (upd.params.size() > 1 && upd.params[1] != "1.0") {
        return false;
    }
    if (upd.params.size() > 2 && upd.params[2] != "0.5") {
        return false;
    }
    if (upd.params.size() > 3 && upd.params[3] != "0.1") {
        return false;
    }
    if (upd.uniform_name != "uSlider") {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

bool Test_glApi_Uniforms(const std::string& vTest) {
    IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_0);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_1);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_2);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_3);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_4);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_5);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_6);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_7);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_8);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_9);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_10);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Good_Syntax_11);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Bad_Syntax_0);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Bad_Syntax_1);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Bad_Syntax_2);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Bad_Syntax_3);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Bad_Syntax_4);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Bad_Syntax_5);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Bad_Syntax_6);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Bad_Syntax_7);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Time_Widget_0);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Buffer_Widget_0);
    else IfTestExist(Test_glApi_Uniforms_Parsing_Slider_Widget_0);

    return false;
}
