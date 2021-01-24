
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stack>
#include <sstream>
#include <cmath>
#include <map>
#include <set>
#include <iterator>
#include "functions.h"
#define PI 3.14159265


using namespace std;

struct Entry_
{
    string expr_;
    string op_;
};

bool PrecedenceLess(const string& left, const string& right, bool assoc)
{
    static const map<string, int> KNOWN({ {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"^", 3}, {"sqrt", 3}, {"sin", 4}, {"cos", 4} });
    static const set<string> ASSOCIATIVE({ "+", "*" });
    return (KNOWN.count(left) ? KNOWN.find(left)->second : 0) < (KNOWN.count(right) ? KNOWN.find(right)->second : 0) + (assoc && !ASSOCIATIVE.count(right) ? 1 : 0);
}

void Parenthesize(Entry_* old, const string& token, bool assoc)
{
    if (!old->op_.empty() && PrecedenceLess(old->op_, token, assoc))
        old->expr_ = '(' + old->expr_ + ')';
}

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
        return "";
    }
    return stack.top().expr_;
}

template <typename ITERATOR>
double calcRPN(ITERATOR iter, ITERATOR end) // function to read RPN expression and calculate it
{
    stack<double> operands; // stack of operands
    auto pop_operands([&operands]() // lambda function to get operands from the stack
        {
            if (operands.empty())
            {
                //cout << "\nError: too little operands!\n";
            }
            else
            {
                auto top(operands.top());
                operands.pop(); // deletes the operand from stack
                return top; // returns the operand
            }
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
                //cout << "\nError: unknown operator: " << *iter << "\n";
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
                    //cout << "\nError: wrong input!\n";
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
                        //cout << "\nError: wrong input!\n";
                    }
                }
            }
        }
        iter++;
    }
    if (operands.size() > 1)
    {
        //cout << "\nError: too many operands!\n";
        return 0;
    }
    return operands.top();
}

/////////////////////////////

bool IsOperator(string C)
{
    if (C == "+" || C == "-" || C == "*" || C == "/" || C == "^" || C == "sqrt" || C == "sin" || C == "cos")
        return true;

    return false;
}

bool IsOpeningParenthesis(string C)
{
    if (C == "(")
        return true;
    return false;
}

bool IsClosingParenthesis(string C)
{
    if (C == ")")
        return true;
    return false;
}

bool IsRightAssociative(string op)
{
    if (op == "^" || op == "sqrt") return true;
    return false;
}

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
string ToPostfix(ITERATOR iter, ITERATOR end)
{
    // Declaring a Stack from Standard template library in C++.
    stack<string> S;
    string postfix = ""; // Initialize postfix as an empty string.
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
            postfix += new_value + " ";

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
                    postfix += S.top() + " ";
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
                    postfix += S.top() + " ";
                    S.pop();
                }
                S.pop();
            }
        }
        iter++;
    }

    // pop and append all operators until stack is not empty
    while (!S.empty()) {
        postfix += S.top() + " ";
        S.pop();
    }

    return postfix;
}

template <typename ITERATOR>
bool isRPN(ITERATOR iter, ITERATOR end)
{
    int current_pos = 0;
    int last_num_pos = -2;

    while (iter != end)
    {
        stringstream ss{ *iter };
        double value;

        if (current_pos == 1)
            if (*iter == "sin" || *iter == "cos" || *iter == "sqrt")
                return true;

        if (current_pos == 0)
        {
            if (*iter == "sin" || *iter == "cos" || *iter == "sqrt")
                return false;
        }
        if (ss >> value)
        {
            if (last_num_pos == current_pos - 1)
            {
                return true;
            }
            last_num_pos = current_pos;
        }
        current_pos++;
        iter++;
    }

    return false;
}

template <typename ITERATOR>
bool isCorrect(ITERATOR iter, ITERATOR end)
{
    int operator_count = 0;
    int s_operator_count = 0;
    int operand_count = 0;

    while (iter != end)
    {
        stringstream ss{ *iter };
        double value;

        if (ss >> value)
        {
            operand_count++;
        }
        else if (*iter == "+" || *iter == "-" || *iter == "*" || *iter == "/" || *iter == "^")
        {
            operator_count++;
        }
        else if (*iter != "(" && *iter != ")" && *iter != "sin" && *iter != "cos" && *iter != "sqrt")
        {
            cout << "\nUnknown operator (" << *iter << ")!\n";
            return 0;
        }
        iter++;
    }
    if (operand_count == operator_count + 1)
        return true;
    else if (operand_count > operator_count + 1)
    {
        cout << "\nToo many operands!\n";
        return false;
    }
    else if (operand_count < operator_count + 1)
    {
        cout << "\nToo many operators!\n";
        return false;
    }
}



