#include "functions.h"
using namespace std;

void printArgs(int argc, char** argv)
{
    std::cout << "You have entered " << argc
        << " arguments:" << "\n";

    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << "\n";

    std::cout << "\n\n\n";
}

void showArgs(int argc, char* argv[], bool* o_flag, bool* i_flag, bool* r_flag, bool* c_flag, bool* h_flag, string* input, string* output)
{
    for (int i = 1; i < argc; i++)
    {
        auto argument = std::string(argv[i]);
        if (argument == "-i" || argument == "--input")
        {
            *input = argv[i + 1];
            *i_flag = true;
            std::cout << "Input: " << *input << "\n\n";
        }
        else if (argument == "-o" || argument == "--output")
        {
            *output = argv[i + 1];
            *o_flag = true;
            std::cout << "Output: " << *output << "\n\n";
        }
        else if (argument == "-r")
        {
            *r_flag = true;
            cout << "Output notation: POSTFIX  (default is INFIX)\n\n";
        }
        else if (argument == "-c")
        {
            *c_flag = true;
            cout << "Interactive mode\n\n";
        }
        else if (argument == "-h" || argument == "--help")
        {
            *h_flag = true;
            cout << "\nAvailable flags:\n -i\tinput file path\n -o\toutput file path\n -c\tinteractive mode\n -r\tif present, expressions are written to output file in POSTFIX notation\n\n";
        }
    }
    cout << "\nDone\n";
}

void interactiveMode(bool o_flag, bool r_flag, string output)
{
    ofstream outputfile(output);
    if (outputfile.fail())
    {
        o_flag = false;
        cout << "\nError: couldn't open output file!\n";
    }

    string expr = "";
    system("pause");
    cout << "\n";

    cout << "Example of postfix notation:\t2 1 - 3 + \nExample of infix notation:\t2 - 1 + 3\n\nAvailable operations:\n+\t-\t*\t/\n^\tsqrt\tsin\tcos\n\nUse of round brackets is also available\n\nType '-q' to exit the program\n\n";
    while (expr != "-q")
    {
        getline(cin, expr);
        stringstream sstream_to_check_amount1{ expr };
        if (expr != "-q" && !expr.empty())
        {
            if (isCorrect(istream_iterator<string>{sstream_to_check_amount1}, {}))
            {

                stringstream sstream_to_check1{ expr };
                stringstream sstream_to_screen1{ expr };
                stringstream sstream_to_file1{ expr };
                stringstream sstream_to_string{ expr };
                string sstream_to_postfix = ToPostfix(istream_iterator<string>{sstream_to_string}, {});
                stringstream sstream_to_postfix2{ sstream_to_postfix };
                stringstream sstream_to_postfix3{ sstream_to_postfix };
                if (isRPN(istream_iterator<string>{sstream_to_check1}, {}))
                {
                    cout << "POSTFIX  ->  INFIX\n";
                    // parsing postfix -> infix, calculating the result from postfix notation
                    cout << ToInfix(expr) << "  =  " << calcRPN(istream_iterator<string>{sstream_to_screen1}, {}) << "\n\n";
                    if (o_flag == true)
                    {
                        if (r_flag == true)
                            outputfile << expr << "  =  " << calcRPN(istream_iterator<string>{sstream_to_file1}, {}) << "\n";
                        else
                            outputfile << ToInfix(expr) << "  =  " << calcRPN(istream_iterator<string>{sstream_to_file1}, {}) << "\n";
                    }
                }
                else
                {
                    cout << "INFIX  ->  POSTFIX\n";
                    // parsing infix -> postfix, calculating the result from infix notation
                    cout << sstream_to_postfix << " =  " << calcRPN(istream_iterator<string>{sstream_to_postfix2}, {}) << "\n\n";
                    if (o_flag == true)
                    {
                        if (r_flag == true)
                            outputfile << sstream_to_postfix << " =  " << calcRPN(istream_iterator<string>{sstream_to_postfix3}, {}) << "\n";
                        else
                            outputfile << expr << " =  " << calcRPN(istream_iterator<string>{sstream_to_postfix3}, {}) << "\n";
                    }
                }
            }
            else
            {
                cout << "***Error: incorrect input! (" << expr << ") Check used operators or the amount of operands and operators!***\n\n";
                if (o_flag == true)
                    outputfile << expr << "\t\t~~\tincorrect input!\n";
            }
        }
    }
    outputfile.close();
}

void inputMode(bool o_flag, bool r_flag, bool c_flag, string input, string output)
{
    ofstream outputfile(output);
    if (outputfile.fail())
    {
        o_flag = false;
        cout << "\nError: couldn't open output file!\n";
    }
    string expr = "";
    ifstream inputfile(input);
    if (inputfile.fail()) {
        cout << "\nError: Input file doesnt exist!\n\n";
        system("pause");
        return;
    }

    int line_count = 1;
    while (getline(inputfile, expr))
    {
        stringstream sstream_to_check_amount1{ expr };

        if (isCorrect(istream_iterator<string>{sstream_to_check_amount1}, {}))
        {
            stringstream sstream_to_check1{ expr };
            stringstream sstream_to_screen1{ expr };
            stringstream sstream_to_file1{ expr };
            stringstream sstream_to_string{ expr };
            string sstream_to_postfix = ToPostfix(istream_iterator<string>{sstream_to_string}, {});
            stringstream sstream_to_postfix2{ sstream_to_postfix };
            stringstream sstream_to_postfix3{ sstream_to_postfix };
            if (isRPN(istream_iterator<string>{sstream_to_check1}, {}))
            {
                if (c_flag == true)
                {
                    cout << "POSTFIX  ->  INFIX\n" << expr << " = ";
                    // parsing postfix -> infix, calculating the result from postfix notation
                    cout << ToInfix(expr) << "  =  " << calcRPN(istream_iterator<string>{sstream_to_screen1}, {}) << "\n\n";
                }
                if (o_flag == true)
                {
                    if (r_flag == true)
                        outputfile << expr << "  =  " << calcRPN(istream_iterator<string>{sstream_to_file1}, {}) << "\n";
                    else
                        outputfile << ToInfix(expr) << "  =  " << calcRPN(istream_iterator<string>{sstream_to_file1}, {}) << "\n";
                }
            }
            else
            {
                if (c_flag == true)
                {
                    cout << "INFIX  ->  POSTFIX\n" << expr << " = ";
                    // parsing infix -> postfix, calculating the result from infix notation
                    cout << sstream_to_postfix << " =  " << calcRPN(istream_iterator<string>{sstream_to_postfix2}, {}) << "\n\n";
                }
                if (o_flag == true)
                {
                    if (r_flag == true)
                        outputfile << sstream_to_postfix << " =  " << calcRPN(istream_iterator<string>{sstream_to_postfix3}, {}) << "\n";
                    else
                        outputfile << expr << " =  " << calcRPN(istream_iterator<string>{sstream_to_postfix3}, {}) << "\n";
                }
            }
        }
        else
        {
            cout << "***Error: incorrect input at line " << line_count << "! (" << expr << ") Check used operators or the amount of operands and operators!***\n\n";
            if (o_flag == true)
                outputfile << expr << "\t\t~~\tincorrect input!\n";
        }
        line_count++;
        system("pause");
        cout << "\n";
    }
    outputfile.close();
}

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









