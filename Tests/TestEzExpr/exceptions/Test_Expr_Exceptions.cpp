
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

#include <EzExpr/exceptions/Test_Expr_Exceptions.h>
#include <EzExpr.hpp>

////////////////////////////////////////////////////////////////////////////
//// ERROR CODES ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Test for DIVISION_BY_ZERO exception
bool Test_Expr_Exception_DivisionByZero() {
    ez::Expr ev;
    try {
        ev.parse("1 / 0").eval();  // Division by integer zero
        return false;              // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::DIVISION_BY_ZERO; }
}

bool Test_Expr_Exception_DivisionByZero_Float() {
    ez::Expr ev;
    try {
        ev.parse("5.5 / 0.0").eval();  // Division by floating-point zero
        return false;                  // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::DIVISION_BY_ZERO; }
}

bool Test_Expr_Exception_ModuloByZero() {
    ez::Expr ev;
    try {
        ev.parse("10 % 0").eval();  // Modulo by zero
        return false;               // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::DIVISION_BY_ZERO; }
}

// Test for UNMATCHED_PARENTHESIS exception
bool Test_Expr_Exception_UnmatchedParenthesis_Open() {
    ez::Expr ev;
    try {
        ev.parse("1 + (2 * 3").eval();  // Unmatched opening parenthesis
        return false;                   // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::UNMATCHED_PARENTHESIS; }
}

bool Test_Expr_Exception_UnmatchedParenthesis_Close() {
    ez::Expr ev;
    try {
        ev.parse("1 + 2) * 3").eval();  // Unmatched closing parenthesis
        return false;                   // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::UNMATCHED_PARENTHESIS; }
}

// Test for VARIABLE_NOT_FOUND exception
bool Test_Expr_Exception_VariableNotFound() {
    ez::Expr ev;
    try {
        ev.parse("x + 1").eval();  // 'x' is not defined
        return false;              // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::VARIABLE_NOT_FOUND; }
}

bool Test_Expr_Exception_VariableNotFound_Multiple() {
    ez::Expr ev;
    try {
        ev.parse("y + x + 1").eval();  // 'x' and 'y' are not defined
        return false;                  // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::VARIABLE_NOT_FOUND; }
}

// Test for FUNCTION_NOT_FOUND exception
bool Test_Expr_Exception_FunctionNotFound() {
    ez::Expr ev;
    try {
        ev.parse("unknownFunc(1)").eval();  // 'unknownFunc' is not defined
        return false;                       // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::FUNCTION_NOT_FOUND; }
}

bool Test_Expr_Exception_FunctionWrongArgs() {
    ez::Expr ev;
    try {
        ev.parse("sin(1, 2)").eval();  // 'sin' is defined but with wrong number of arguments
        return false;                  // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::FUNCTION_WRONG_ARGUMENTS_COUNT; }
}

// Test for EVALUATION_NAN exception
bool Test_Expr_Exception_EvaluationNaN_1() {
    ez::Expr ev;
    try {
        ev.parse("sqrt(-1)").eval();  // sqrt of negative number leads to NaN
        return false;                 // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::EVALUATION_NAN; }
}
bool Test_Expr_Exception_EvaluationNaN_2() {
    ez::Expr ev;
    try {
        ev.parse("(-1)^0.5").eval();  // sqrt of negative number leads to NaN
        return false;                 // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::EVALUATION_NAN; }
}

bool Test_Expr_Exception_EvaluationNaN_Log() {
    ez::Expr ev;
    try {
        ev.parse("log(-5)").eval();  // Logarithm of negative number leads to NaN
        return false;                // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::EVALUATION_NAN; }
}

bool Test_Expr_Exception_EvaluationNaN_DivZeroZero() {
    ez::Expr ev;
    try {
        ev.parse("0 / 0.0").eval();  // 0 divided by 0.0 leads to NaN
        return false;                // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::DIVISION_BY_ZERO; }
}

// Test for EVALUATION_INF exception
bool Test_Expr_Exception_EvaluationInf() {
    ez::Expr ev;
    try {
        ev.parse("1 / 0.0").eval();  // Division by zero should lead to Inf in floating point
        return false;                // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::DIVISION_BY_ZERO; }
}

bool Test_Expr_Exception_EvaluationInf_Exp() {
    ez::Expr ev;
    try {
        ev.parse("exp(1000)").eval();  // Exponential of a large number may lead to Inf
        return false;                  // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::EVALUATION_INF; }
}

bool Test_Expr_Exception_EvaluationInf_Power() {
    ez::Expr ev;
    try {
        ev.parse("10^1000").eval();  // Large power leading to Inf
        return false;                // Expected an exception
    } catch (const ez::ExprException& e) { return e.getCode() == ez::ErrorCode::EVALUATION_INF; }
}

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v) \
    if (vTest == std::string(#v)) return v()

bool Test_Expr_Exceptions_run_test(const std::string& vTest) {
    // Errors Codes
    IfTestExist(Test_Expr_Exception_DivisionByZero);
    else IfTestExist(Test_Expr_Exception_DivisionByZero_Float);
    else IfTestExist(Test_Expr_Exception_ModuloByZero);
    else IfTestExist(Test_Expr_Exception_UnmatchedParenthesis_Open);
    else IfTestExist(Test_Expr_Exception_UnmatchedParenthesis_Close);
    else IfTestExist(Test_Expr_Exception_VariableNotFound);
    else IfTestExist(Test_Expr_Exception_VariableNotFound_Multiple);
    else IfTestExist(Test_Expr_Exception_FunctionNotFound);
    else IfTestExist(Test_Expr_Exception_FunctionWrongArgs);
    else IfTestExist(Test_Expr_Exception_EvaluationNaN_1);
    else IfTestExist(Test_Expr_Exception_EvaluationNaN_2);
    else IfTestExist(Test_Expr_Exception_EvaluationNaN_Log);
    else IfTestExist(Test_Expr_Exception_EvaluationNaN_DivZeroZero);
    else IfTestExist(Test_Expr_Exception_EvaluationInf);
    else IfTestExist(Test_Expr_Exception_EvaluationInf_Exp);
    else IfTestExist(Test_Expr_Exception_EvaluationInf_Power);
    // default
    return false;
}