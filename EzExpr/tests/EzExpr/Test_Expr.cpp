
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

#include <EzExpr/Test_Expr.h>
#include <EzExpr/parsings/Test_Expr_Parsings.h>
#include <EzExpr/builtins/Test_Expr_Builtins.h>
#include <EzExpr/constants/Test_Expr_Constants.h>
#include <EzExpr/exceptions/Test_Expr_Exceptions.h>
#include <EzExpr/perfos/Test_Expr_Perfos.h>

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestCollectionExist(v, str) \
    if (vTest.find(str) != std::string::npos) return v(vTest)

bool Test_Expr(const std::string& vTest) {
    IfTestCollectionExist(Test_Expr_Parsings_run_test, "Test_Expr_Parsing");
    else IfTestCollectionExist(Test_Expr_Constants_run_test, "Test_Expr_Constant");
    else IfTestCollectionExist(Test_Expr_Builtins_run_test, "Test_Expr_Builtin");
    else IfTestCollectionExist(Test_Expr_Exceptions_run_test, "Test_Expr_Exception");
    else IfTestCollectionExist(Test_Expr_Perfos_run_test, "Test_Expr_Perfo");
    // default
    return false;
}