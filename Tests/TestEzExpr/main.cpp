
/*
MIT License

Copyright (c) 2024 Stephane Cuillerdier (aka aiekick)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <TestEzExprParsings.h>
#include <TestEzExprBuiltins.h>
#include <TestEzExprConstants.h>
#include <TestEzExprExceptions.h>
#include <TestEzExprPerfos.h>

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestCollectionExist(v, str) \
    if (vTest.find(str) != std::string::npos) return v(vTest)

bool TestEzExpr(const std::string& vTest) {
    IfTestCollectionExist(TestEzExpr_Parsings_run_test, "TestEzExpr_Parsing");
    else IfTestCollectionExist(TestEzExpr_Constants_run_test, "TestEzExpr_Constant");
    else IfTestCollectionExist(TestEzExpr_Builtins_run_test, "TestEzExpr_Builtin");
    else IfTestCollectionExist(TestEzExpr_Exceptions_run_test, "TestEzExpr_Exception");
    else IfTestCollectionExist(TestEzExpr_Perfos_run_test, "TestEzExpr_Perfo");
    // default
    return false;
}

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    if (argc > 1) {
        printf("Exec test : %s\n", argv[1]);
        return TestEzExpr(argv[1]) ? 0 : 1;
    }
    // User testing
    return TestEzExpr("") ? 0 : 1;
}
