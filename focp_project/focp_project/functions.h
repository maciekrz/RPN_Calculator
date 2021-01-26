#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <stack>
#include <sstream>
#include <cmath>
#include <map>
#include <set>
#include <iterator>

#define PI 3.14159265358979323846

using namespace std;

/// <summary>
/// Struct for storing operators and expressions.
/// </summary>
/// <param name="expr_">an expression</param>
/// <param name="op_">an operator</param>
struct Entry_
{
    string expr_;
    string op_;
};

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << "<option(s)> SOURCES"
        << "Options:\n"
        << "\t-h,--help\t\tShow this help message\n"
        << "\t-o,--output FILE_PATH\tSpecify the output path\n"
        << "\t-i,--input FILE_PATH\tSpecify the input path\n"
        << std::endl;
}

void printArgs(int argc, char** argv);

void showArgs(int argc, char* argv[], bool* o_flag, bool* i_flag, bool* r_flag, bool* c_flag, bool* h_flag, string* input, string* output);

/// <summary>
/// Function for interactive mode
/// </summary>
/// <param name="o_flag">output flag</param>
/// <param name="r_flag">notation type flag</param>
/// <param name="output">output directory</param>
void interactiveMode(bool o_flag, bool r_flag, string output);

/// <summary>
/// Function for input from file.
/// With c_flag being true, expressions with results are shown on screen.
/// </summary>
/// <param name="o_flag">output flag</param>
/// <param name="r_flag">notation flag</param>
/// <param name="c_flag">mode flag</param>
/// <param name="input">input directory</param>
/// <param name="output">output directory</param>
void inputMode(bool o_flag, bool r_flag, bool c_flag, string input, string output);

/// <summary>
/// Function for determining precedence of operations
/// </summary>
/// <param name="left">list of operators</param>
/// <param name="right">list of precedence</param>
/// <param name="assoc">a bool variable to determine if the operation is associative</param>
bool PrecedenceLess(const string& left, const string& right, bool assoc);

/// <summary>
/// Function to parenthesize low-precedence operations
/// </summary>
/// <param name="old">old expression without parenthesizes</param>
/// <param name="token">operation sign</param>
/// <param name="assoc">a bool variable to determine if the operation is associative</param>
void Parenthesize(Entry_* old, const string& token, bool assoc);

/// <summary>
/// Function to add operation sign to the expression
/// </summary>
/// <param name="stack">stack that holds items of Entry_ struct</param>
/// <param name="token">operation sign</param>
void AddToken(stack<Entry_>* stack, const string& token);

/// <summary>
/// Function to parse postfix notation into infix notation
/// </summary>
/// <param name="src">expression to parse</param>
/// <returns>parsed expression</returns>
string ToInfix(const string& src);

/// <summary>
/// Function to calculate postfix expression
/// </summary>
/// <typeparam name="ITERATOR"></typeparam>
/// <param name="iter">iterator to sweep through expression</param>
/// <param name="end">end of the expression</param>
/// <returns>result of the expression</returns>
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
            if (*iter == "sin" || *iter == "cos" || *iter == "sqrt") // sin, cos and sqrt take only one value and need separate code, so they won't pop two elements from the stack
            {
                auto right = pop_operands();
                auto left = 0;
                if (right < 0 && *iter == "sqrt")
                {
                    cout << "\nError: root of negative number!\n";
                    return 0;
                }                
                operands.push(operators.at(*iter)(left, right)); //pushes result to the stack making it available for next operations
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
                    operands.push(operators.at(*iter)(left, right)); //pushes result to the stack making it available for next operations
                }
            }
        }
        iter++;
    }
    return operands.top();
}

/////////////////////////////

/// <summary>
/// Function to verify whether a character/word is operator symbol or not.
/// </summary>
/// <param name="C">a character/word</param>
/// <returns>true if it's an operator</returns>
bool IsOperator(string C);

/// <summary>
/// Function to verify whether a character is an opening parenthesis
/// </summary>
/// <param name="C">a character</param>
/// <returns>true if it's an opening parenthesis</returns>
bool IsOpeningParenthesis(string C);

/// <summary>
/// Function to verify whether a character is a closing parenthesis
/// </summary>
/// <param name="C"></param>
/// <returns>true if it's a closing parenthesis</returns>
bool IsClosingParenthesis(string C);

/// <summary>
/// Function to verify whether an operator is right associative or not.
/// 
/// right-associative means that the operations are grouped from the right.
/// </summary>
/// <param name="op">operator</param>
bool IsRightAssociative(string op);

/// <summary>
/// Function to get weight of an operator. An operator with higher weight will have higher precedence.
/// </summary>
/// <param name="op">operator</param>
/// <returns>operator's weight</returns>
int GetOperatorWeight(string op);

/// <summary>
/// Function to determine precedence between two operators
/// </summary>
/// <param name="op1">first operator</param>
/// <param name="op2">second operator</param>
bool HasHigherPrecedence(string op1, string op2);

/// <summary>
/// Function to parse infix notation into postfix notation
/// </summary>
/// <typeparam name="ITERATOR"></typeparam>
/// <param name="iter">iterator to sweep through expression</param>
/// <param name="end">end of the expression</param>
/// <returns>parsed expression</returns>
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

            // If character is an operator, check for higher precedence operator in stack top
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

/// <summary>
/// Function to check notation. True if postfix, false if infix
/// </summary>
/// <typeparam name="ITERATOR"></typeparam>
/// <param name="iter">iterator to sweep through expression</param>
/// <param name="end">end of the expression</param>
/// <returns>true if expression is in postfix, false if it's in infix</returns>
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

/// <summary>
/// Function to check if the input is correct
/// </summary>
/// <typeparam name="ITERATOR"></typeparam>
/// <param name="iter">iterator to sweep through expression</param>
/// <param name="end">end of the expression</param>
/// <returns>true if expression is correct</returns>
template <typename ITERATOR>
bool isCorrect(ITERATOR iter, ITERATOR end)
{
    int operator_count = 0;
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



