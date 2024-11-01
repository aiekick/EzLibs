﻿
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
#include <EzTools/EzExpr.hpp>

////////////////////////////////////////////////////////////////////////////
//// PARSING ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool TestEzExpr_Parsing_OperatorWithoutOperands() {
    ez::Expr ev;
    try {
        if (!ev.parse("+").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_UnaryOperatorWithoutOperand() {
    ez::Expr ev;
    try {
        if (!ev.parse("!").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_UnmatchedOpeningParenthesis() {
    ez::Expr ev;
    try {
        if (!ev.parse("(5 + 3").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::UNMATCHED_PARENTHESIS) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_UnmatchedClosingParenthesis() {
    ez::Expr ev;
    try {
        if (!ev.parse("5 + 3)").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::UNMATCHED_PARENTHESIS) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_DoubleBinaryOperator() {
    ez::Expr ev;
    try {
        if (!ev.parse("5 ++ 3").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::OPERATOR_NOT_FOUND) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_DoubleUnaryOperator() {
    ez::Expr ev;
    try {
        if (!ev.parse("--5").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_UndefinedVariable() {
    ez::Expr ev;
    try {
        if (!ev.parse("x + 5").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::VARIABLE_NOT_FOUND) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_BinaryOperatorWithoutRightOperand() {
    ez::Expr ev;
    try {
        if (!ev.parse("5 + ").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_DoubleOperandWithoutOperator() {
    ez::Expr ev;
    try {
        if (!ev.parse("5 5").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_FunctionWithoutParentheses() {
    ez::Expr ev;
    try {
        if (!ev.parse("sin 5").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_FunctionWithoutArguments() {
    ez::Expr ev;
    try {
        if (!ev.parse("sin()").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::FUNCTION_WRONG_ARGUMENTS_COUNT) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_FunctionWithTooManyArguments() {
    ez::Expr ev;
    try {
        if (!ev.parse("atan2(1, 2, 3)").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::FUNCTION_WRONG_ARGUMENTS_COUNT) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_FunctionWithOneArgumentInsteadOfTwo() {
    ez::Expr ev;
    try {
        if (!ev.parse("atan2(1)").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::FUNCTION_WRONG_ARGUMENTS_COUNT) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_DivisionByZero() {
    ez::Expr ev;
    try {
        if (!ev.parse("1 / 0").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::DIVISION_BY_ZERO) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_MisplacedComma() {
    ez::Expr ev;
    try {
        if (!ev.parse("1,2 + 3").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_UndefinedOperator() {
    ez::Expr ev;
    try {
        if (!ev.parse("5 $ 3").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::OPERATOR_NOT_FOUND) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_MisuseOfFactorialOperator() {
    ez::Expr ev;
    try {
        if (!ev.parse("5 ! 3").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_UnaryOperatorBeforeParenthesis() {
    ez::Expr ev;
    try {
        if (!ev.parse("!(5)").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_EmptyParenthesis() {
    ez::Expr ev;
    try {
        if (!ev.parse("()").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::EMPTY_PARENTHESIS) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_DoubleComma() {
    ez::Expr ev;
    try {
        if (!ev.parse("atan2(1,,1)").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_DoubleBinaryOperatorAtStart() {
    ez::Expr ev;
    try {
        if (!ev.parse("++1").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_DoubleBinaryOperatorAtEnd() {
    ez::Expr ev;
    try {
        if (!ev.parse("1++").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::OPERATOR_NOT_FOUND) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_EmptyExpression() {
    ez::Expr ev;
    try {
        if (!ev.parse("").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_UnknownFunctionName() {
    ez::Expr ev;
    try {
        if (!ev.parse("unknownFunction(5)").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::FUNCTION_NOT_FOUND) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_MissingParenthesisInComplexExpression() {
    ez::Expr ev;
    try {
        if (!ev.parse("5 + 3 * 2 )").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::UNMATCHED_PARENTHESIS) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_OperatorWithoutOperandsInParenthesis() {
    ez::Expr ev;
    try {
        if (!ev.parse("( + )").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_NumberFollowedByVariable() {
    ez::Expr ev;
    try {
        if (!ev.parse("5x + 3").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_IncorrectUseOfComma() {
    ez::Expr ev;
    try {
        if (!ev.parse("5, 3").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_UnrecognizedCharacter() {
    ez::Expr ev;
    try {
        if (!ev.parse("5 + @").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_OperatorAtEndOfParenthesis() {
    ez::Expr ev;
    try {
        if (!ev.parse("(5 + 3 + )").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_NumberFollowedByParenthesisWithoutOperator() {
    ez::Expr ev;
    try {
        if (!ev.parse("5(3 + 2)").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_VariableFollowedByExpressionWithoutOperator() {
    ez::Expr ev;
    try {
        if (!ev.parse("x(3 + 2)").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::FUNCTION_NOT_FOUND) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_MisplacedCommaAfterFunction() {
    ez::Expr ev;
    try {
        if (!ev.parse("sin(,pi / 2)").set("pi", 3.14159).eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_BinaryOperatorMisplacedInFunction() {
    ez::Expr ev;
    try {
        if (!ev.parse("atan2(,1)").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_IncompleteExpression() {
    ez::Expr ev;
    try {
        if (!ev.parse("3 +").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_MisuseOfFactorialOperator_2() {
    ez::Expr ev;
    try {
        if (!ev.parse("5!3").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_NumberFollowedByBinaryOperator() {
    ez::Expr ev;
    try {
        if (!ev.parse("5*").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_IncorrectUseOfOpeningParenthesis() {
    ez::Expr ev;
    try {
        if (!ev.parse("((5)").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::UNMATCHED_PARENTHESIS) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_IncorrectVariableName() {
    ez::Expr ev;
    try {
        if (!ev.parse("5 + x$").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::OPERATOR_NOT_FOUND) return false;
    }
    return true;
}

bool TestEzExpr_Parsing_IncorrectUseOfMultipleOperators() {
    ez::Expr ev;
    try {
        if (!ev.parse("5 + - * 3").eval().check(0)) return false;
    } catch (const ez::ExprException& e) {
        if (e.getCode() != ez::ErrorCode::PARSE_ERROR) return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v) \
    if (vTest == std::string(#v)) return v()

bool TestEzExpr_Parsings_run_test(const std::string& vTest) {
    // Parsing
    IfTestExist(TestEzExpr_Parsing_OperatorWithoutOperands);
    else IfTestExist(TestEzExpr_Parsing_UnaryOperatorWithoutOperand);
    else IfTestExist(TestEzExpr_Parsing_UnmatchedOpeningParenthesis);
    else IfTestExist(TestEzExpr_Parsing_UnmatchedClosingParenthesis);
    else IfTestExist(TestEzExpr_Parsing_DoubleBinaryOperator);
    else IfTestExist(TestEzExpr_Parsing_DoubleUnaryOperator);
    else IfTestExist(TestEzExpr_Parsing_UndefinedVariable);
    else IfTestExist(TestEzExpr_Parsing_BinaryOperatorWithoutRightOperand);
    else IfTestExist(TestEzExpr_Parsing_DoubleOperandWithoutOperator);
    else IfTestExist(TestEzExpr_Parsing_FunctionWithoutParentheses);
    else IfTestExist(TestEzExpr_Parsing_FunctionWithoutArguments);
    else IfTestExist(TestEzExpr_Parsing_FunctionWithTooManyArguments);
    else IfTestExist(TestEzExpr_Parsing_FunctionWithOneArgumentInsteadOfTwo);
    else IfTestExist(TestEzExpr_Parsing_DivisionByZero);
    else IfTestExist(TestEzExpr_Parsing_MisplacedComma);
    else IfTestExist(TestEzExpr_Parsing_UndefinedOperator);
    else IfTestExist(TestEzExpr_Parsing_MisuseOfFactorialOperator);
    else IfTestExist(TestEzExpr_Parsing_UnaryOperatorBeforeParenthesis);
    else IfTestExist(TestEzExpr_Parsing_EmptyParenthesis);
    else IfTestExist(TestEzExpr_Parsing_DoubleComma);
    else IfTestExist(TestEzExpr_Parsing_DoubleBinaryOperatorAtStart);
    else IfTestExist(TestEzExpr_Parsing_DoubleBinaryOperatorAtEnd);
    else IfTestExist(TestEzExpr_Parsing_EmptyExpression);
    else IfTestExist(TestEzExpr_Parsing_UnknownFunctionName);
    else IfTestExist(TestEzExpr_Parsing_MissingParenthesisInComplexExpression);
    else IfTestExist(TestEzExpr_Parsing_OperatorWithoutOperandsInParenthesis);
    else IfTestExist(TestEzExpr_Parsing_NumberFollowedByVariable);
    else IfTestExist(TestEzExpr_Parsing_IncorrectUseOfComma);
    else IfTestExist(TestEzExpr_Parsing_UnrecognizedCharacter);
    else IfTestExist(TestEzExpr_Parsing_OperatorAtEndOfParenthesis);
    else IfTestExist(TestEzExpr_Parsing_NumberFollowedByParenthesisWithoutOperator);
    else IfTestExist(TestEzExpr_Parsing_VariableFollowedByExpressionWithoutOperator);
    else IfTestExist(TestEzExpr_Parsing_MisplacedCommaAfterFunction);
    else IfTestExist(TestEzExpr_Parsing_BinaryOperatorMisplacedInFunction);
    else IfTestExist(TestEzExpr_Parsing_IncompleteExpression);
    else IfTestExist(TestEzExpr_Parsing_MisuseOfFactorialOperator_2);
    else IfTestExist(TestEzExpr_Parsing_NumberFollowedByBinaryOperator);
    else IfTestExist(TestEzExpr_Parsing_IncorrectUseOfOpeningParenthesis);
    else IfTestExist(TestEzExpr_Parsing_IncorrectVariableName);
    else IfTestExist(TestEzExpr_Parsing_IncorrectUseOfMultipleOperators);
    // default
    return false;
}