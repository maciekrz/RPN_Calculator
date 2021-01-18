#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stack>
#include <sstream>
#include <cmath>
#include <map>
#include <set>
#include <iterator>
#define PI 3.14159265

using namespace std;

/// <summary>
/// Struct for storing operators and expressions.
/// </summary>
struct Entry_
{
    string expr_;
    string op_;
};

/// <summary>
/// Function for determining precedence of operations
/// </summary>
/// <param name="left">list of operators</param>
/// <param name="right">list of precedence</param>
/// <param name="assoc">a bool variable to determine if the operation is associative</param>
/// <returns></returns>
bool PrecedenceLess(const string& left, const string& right, bool assoc)
{
    static const map<string, int> KNOWN({ {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"^", 3}, {"sqrt", 3}, {"sin", 4}, {"cos", 4} });
    static const set<string> ASSOCIATIVE({ "+", "*" });
    return (KNOWN.count(left) ? KNOWN.find(left)->second : 0) < (KNOWN.count(right) ? KNOWN.find(right)->second : 0) + (assoc && !ASSOCIATIVE.count(right) ? 1 : 0);
}

/// <summary>
/// Function to parenthesize low-precedence operations
/// </summary>
/// <param name="old">old expression without parenthesizes</param>
/// <param name="token">operation sign</param>
/// <param name="assoc">a bool variable to determine if the operation is associative</param>
void Parenthesize(Entry_* old, const string& token, bool assoc)
{
    if (!old->op_.empty() && PrecedenceLess(old->op_, token, assoc))
        old->expr_ = '(' + old->expr_ + ')';
}

/// <summary>
/// Function to add operation sign to the expression
/// </summary>
/// <param name="stack">stack that holds items of Entry_ struct</param>
/// <param name="token">operation sign</param>
void AddToken(stack<Entry_>* stack, const string& token)
{
    if (token.find_first_of("0123456789") != string::npos)
    {
        stack->push(Entry_({ token, string() })); // it's a number, not an operator
    }
    else if (token == "sin" || token == "cos" || token == "sqrt")
    {
        auto rhs = stack->top();
        Parenthesize(&rhs, token, false);
        stack->top().expr_ = token + '(' + rhs.expr_ + ')';
        stack->top().op_ = token;
    }
    else // it's an operator
    { 

        if (stack->size() < 2)
        {
            cout << "Stack underflow";
            return;
        }
        auto rhs = stack->top();
        Parenthesize(&rhs, token, false);
        stack->pop();
        auto lhs = stack->top();
        Parenthesize(&lhs, token, true);
        stack->top().expr_ = lhs.expr_ + ' ' + token + ' ' + rhs.expr_;
        stack->top().op_ = token;
    }
}

/// <summary>
/// Function to parse postfix notation into infix notation
/// </summary>
/// <param name="src">expression to parse</param>
/// <returns>parsed expression</returns>
string ToInfix(const string& src)
{
    stack<Entry_> stack;
    for (auto start = src.begin(), p = src.begin();; ++p)
    {
        if (p == src.end() || *p == ' ')
        {
            if (p > start)
                AddToken(&stack, string(start, p));
            if (p == src.end())
                break;
            start = p + 1;
        }
    }
    if (stack.size() != 1) {
        cout << "Incomplete expression";
        return "";
    }
    return stack.top().expr_;
}

template <typename ITERATOR>
/// <summary>
/// Function to calculate postfix expression
/// </summary>
/// <typeparam name="ITERATOR"></typeparam>
/// <param name="iter">iterator to sweep through expression</param>
/// <param name="end">end of the expression</param>
/// <returns></returns>
double calcRPN(ITERATOR iter, ITERATOR end) // function to read RPN expression and calculate it
{
    stack<double> operands; // stack of operands

    auto pop_operands([&operands]() // lambda function to get operands from the stack
        {
            if (operands.empty()) cout << "\nError: too little operands!\n";
            auto top(operands.top());

            operands.pop(); // deletes the operand from stack
            return top; // returns the operand
        });


    map<string, double(*)(double, double)> operators // map of available operations
    {
            {"+", [](double a, double b) {return a + b;}},
            {"-", [](double a, double b) {return a - b;}},
            {"*", [](double a, double b) {return a * b;}},
            {"/", [](double a, double b) {return a / b;}},
            {"sin", [](double a, double b) {return sin(b * PI / 180);}},
            {"cos", [](double a, double b) {return cos(b * PI / 180);}},
            {"^", [](double a, double b) {return pow(a,b);}},
            {"sqrt", [](double a, double b) {return sqrt(b);}},
    };

    while (iter != end) // iterator sweeps through stringstream and points either to operands (then if is true) or to operator (then if is false)
    {

        stringstream ss{ *iter };
        double value;
        if (ss >> value) // iterator points to a number - placing doubles into the stack
            operands.push(value);

        else // iterator points to an operator - calculating the result
        {
            try // checks if there is such uperator
            {
                operators.at(*iter);
            }
            catch (...)
            {
                cout << "\nError: unknown operator: " << *iter << "\n";
            }

            if (*iter == "sin" || *iter == "cos" || *iter == "sqrt") // sin, cos and sqrt take only one value and need separate code, so they won't pop two elements from the stack
            {
                auto right = pop_operands();
                auto left = 0;
                if (right < 0 && *iter == "sqrt")
                {
                    cout << "\nError: root of negative number!\n";
                    return 0;
                }
                try
                {
                    operands.push(operators.at(*iter)(left, right)); //pushes result to the stack making it available for next operations
                }
                catch (...)
                {
                    cout << "\nError: wrong input!\n";
                }
            }

            else
            {
                auto right = pop_operands();
                auto left = pop_operands();

                if (right == 0 && *iter == "/") // division by 0
                {
                    cout << "\nError: division by 0!\n";
                    return 0;
                }
                else
                {
                    try
                    {
                        operands.push(operators.at(*iter)(left, right)); //pushes result to the stack making it available for next operations
                    }
                    catch (...)
                    {
                        cout << "\nError: wrong input!\n";
                    }
                }
            }
        }
        iter++;
    }
    if (operands.size() > 1) 
    {
        cout << "\nError: too many operands!\n";
        return 0;
    }
    return operands.top();
}

/////////////////////////////

/// <summary>
/// Function to verify whether a character/word is operator symbol or not.
/// </summary>
/// <param name="C">a character/word</param>
/// <returns></returns>
bool IsOperator(string C)
{
    if (C == "+" || C == "-" || C == "*" || C == "/" || C == "^" || C == "sqrt" || C == "sin" || C == "cos")
        return true;

    return false;
}

/// <summary>
/// Function to verify whether a character is an opening parenthesis
/// </summary>
/// <param name="C">a character</param>
/// <returns></returns>
bool IsOpeningParenthesis(string C)
{
    if (C == "(")
        return true;
    return false;
}

/// <summary>
/// Function to verify whether a character is a closing parenthesis
/// </summary>
/// <param name="C"></param>
/// <returns></returns>
bool IsClosingParenthesis(string C)
{
    if (C == ")")
        return true;
    return false;
}
/// <summary>
/// Function to verify whether an operator is right associative or not.
/// </summary>
/// <param name="op">operator</param>
/// <returns></returns>
bool IsRightAssociative(string op)
{
    if (op == "^" || op == "sqrt") return true;
    return false;
}

/// <summary>
/// Function to get weight of an operator. An operator with higher weight will have higher precedence.
/// </summary>
/// <param name="op">operator</param>
/// <returns></returns>
int GetOperatorWeight(string op)
{
    if (op == "sin" || op == "cos")
        return 4;
    if (op == "^" || op == "sqrt")
        return 3;
    if (op == "*" || op == "/")
        return 2;
    if (op == "+" || op == "-")
        return 1;
    else return -1;
}

/// <summary>
/// Function to determine precedence between two operators
/// </summary>
/// <param name="op1">first operator</param>
/// <param name="op2">secound operator</param>
/// <returns></returns>
bool HasHigherPrecedence(string op1, string op2)
{
    int op1Weight = GetOperatorWeight(op1);
    int op2Weight = GetOperatorWeight(op2);

    // If operators have equal precedence, return true if they are left associative.
    // return false, if right associative.
    // if operator is left-associative, left one should be given priority.
    if (op1Weight == op2Weight)
    {
        if (IsRightAssociative(op1)) return false;
        else return true;
    }
    return op1Weight > op2Weight ? true : false;
}


template <typename ITERATOR>
/// <summary>
/// Function to parse infix notation into postfix notation
/// </summary>
/// <typeparam name="ITERATOR"></typeparam>
/// <param name="iter">iterator to sweep through expression</param>
/// <param name="end">end of the expression</param>
/// <returns></returns>
string ToPostfix(ITERATOR iter, ITERATOR end)
{
    // Declaring a Stack from Standard template library in C++.
    stack<string> S;
    string postfix = ""; // Initialize postfix as empty string.
    while (iter != end)
    {
        stringstream ss{ *iter };
        double value;
        string sgn;
        string new_value;

        // Else if character is an operand then just append it in res string
        if (ss >> value)
        {
            new_value = to_string(value);
            new_value.erase(new_value.find_last_not_of('0') + 2, string::npos);
            postfix += " " + new_value;

        }
        // Scanning each character from left.
        // If character is a delimiter, move on.
        else
        {

            sgn = *iter;

            if (sgn == " ") continue;

            // If character is operator, check for Higher precedence operator in Stack top
            // until first opening bracket and pop all such operators
            // Finally push the current operator on the Stack
            else if (IsOperator(sgn))
            {
                while (!S.empty() && !IsOpeningParenthesis(S.top()) && HasHigherPrecedence(S.top(), sgn))
                {
                    postfix += " " + S.top();
                    S.pop();
                }
                S.push(sgn);
            }
            // If opening Parentheses simply push it on the Stack
            else if (IsOpeningParenthesis(sgn))
            {
                S.push(sgn);
            }
            // If Closing Parenthesis then pop all the operators and append to result string
            else if (IsClosingParenthesis(sgn))
            {
                while (!S.empty() && !IsOpeningParenthesis(S.top())) {
                    postfix += " " + S.top();
                    S.pop();
                }
                S.pop();
            }
        }
        iter++;
    }

    // pop and append all operators until stack is not empty
    while (!S.empty()) {
        postfix += " " + S.top();
        S.pop();
    }

    return postfix;
}



