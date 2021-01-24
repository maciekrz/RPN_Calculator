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
#include "functions.h"

#define PI 3.14159265

using namespace std;

//string ToInfix(const string& src);
//template <typename ITERATOR>
//double calcRPN(ITERATOR iter, ITERATOR end);
//template <typename ITERATOR>
//string ToPostfix(ITERATOR iter, ITERATOR end);


static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << "<option(s)> SOURCES"
        << "Options:\n"
        << "\t-h,--help\t\tShow this help message\n"
        << "\t-o,--output FILE_PATH\tSpecify the output path\n"
        << "\t-i,--input FILE_PATH\tSpecify the input path\n"
        << std::endl;
}

void printArgs(int argc, char** argv)
{
    std::cout << "You have entered " << argc
        << " arguments:" << "\n";

    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << "\n";

    std::cout << "\n\n\n";
}

int main(int argc, char* argv[])
{
    bool o_flag = false;
    bool i_flag = false;
    bool r_flag = false;
    bool c_flag = false;
    bool h_flag = true;


    printArgs(argc, argv);

    if (argc < 5) {
        show_usage(argv[0]);
        return 1;
    }

    string input, output;
    for (int i = 1; i < argc; i++)
    {
        auto argument = std::string(argv[i]);
        if (argument == "-i" || argument == "--input")
        {
            input = argv[i + 1];
            //i_flag = true;
            std::cout << "Input: " << input << "\n\n";
        }
        else if (argument == "-o" || argument == "--output")
        {
            output = argv[i + 1];
            o_flag = true;
            std::cout << "Output: " << output << "\n\n";
        }
        else if (argument == "-r")
        {
            r_flag = true;
            cout << "Output notation: POSTFIX  (default is INFIX)\n\n";
        }
        else if (argument == "-c")
        {
            c_flag = true;
            cout << "Interactive mode\n\n";
        }
        else if (argument == "-h")
        {
            cout << "\nAvailable flags:\n -i\tinput file path\n -o\toutput file path\n -c\tinteractive mode\n -r\tif present, expressions are written to output file in POSTFIX notation\n\n";
        }
    }

    std::cout << "\nDone\n";

    ofstream outputfile(output);
    string expr = "";
    system("pause");
    cout << "\n";

    if (c_flag == true && i_flag == false)
    {
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
                            if (r_flag == true)
                                outputfile << expr << "  =  " << calcRPN(istream_iterator<string>{sstream_to_file1}, {}) << "\n";
                            else
                                outputfile << ToInfix(expr) << "  =  " << calcRPN(istream_iterator<string>{sstream_to_file1}, {}) << "\n";
                    }
                    else
                    {

                        cout << "INFIX  ->  POSTFIX\n";
                        // parsing infix -> postfix, calculating the result from infix notation
                        cout << sstream_to_postfix << " =  " << calcRPN(istream_iterator<string>{sstream_to_postfix2}, {}) << "\n\n";
                        if (o_flag == true)
                            if (r_flag == true)
                                outputfile << sstream_to_postfix << " =  " << calcRPN(istream_iterator<string>{sstream_to_postfix3}, {}) << "\n";
                            else
                                outputfile << expr << " =  " << calcRPN(istream_iterator<string>{sstream_to_postfix3}, {}) << "\n";
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
    }

    if (i_flag == true)
    {
        ifstream inputfile(input);
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
    }
    

    if (i_flag == false && c_flag == false)
    {
        cout << "\nYou haven't chosen input type (interactive or from file).\nRestart the program with one of those.\n";
    }
    
    outputfile.close();
    return 0;
}