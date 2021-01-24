#pragma once

#include <stack>
#include <sstream>
#include <map>
#include <iterator>

using namespace std;

/// <summary>
/// Struct for storing operators and expressions.
/// </summary>
/// <param name="expr_">an expression</param>
/// <param name="op_:>an operator</param>
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
double calcRPN(ITERATOR iter, ITERATOR end);

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
string ToPostfix(ITERATOR iter, ITERATOR end);

/// <summary>
/// Function to check notation. True if postfix, false if infix
/// </summary>
/// <typeparam name="ITERATOR"></typeparam>
/// <param name="iter">iterator to sweep through expression</param>
/// <param name="end">end of the expression</param>
/// <returns>true if expression is in postfix, false if it's in infix</returns>
template <typename ITERATOR>
bool isRPN(ITERATOR iter, ITERATOR end);

/// <summary>
/// Function to check if the input is correct
/// </summary>
/// <typeparam name="ITERATOR"></typeparam>
/// <param name="iter">iterator to sweep through expression</param>
/// <param name="end">end of the expression</param>
/// <returns>true if expression is correct</returns>
template <typename ITERATOR>
bool isCorrect(ITERATOR iter, ITERATOR end);



