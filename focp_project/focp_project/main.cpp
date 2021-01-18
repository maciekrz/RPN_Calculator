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

string ToInfix(const string& src);
template <typename ITERATOR>
double calcRPN(ITERATOR iter, ITERATOR end);
template <typename ITERATOR>
string ToPostfix(ITERATOR iter, ITERATOR end);


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

            std::cout << "Input: " << input << "\n";
        }
        else if (argument == "-o" || argument == "--output")
        {
            output = argv[i + 1];
            std::cout << "Output: " << output << "\n";
        }
    }

    std::cout << "Done\n\n\n";

    ofstream outputfile;
    outputfile.open(output);
    
    char notation;
    string expr;

    

    cout << "Example of postfix notation:\t2 1 - 3 + \nExample of infix notation:\t2 - 1 + 3 \n\nType '-q' to exit the program\n\n";

    while (expr != "-q")
    {
            cout << "Postfix -> infix\n> ";
            getline(cin, expr);
            
            if (expr != "-q")
            {
                stringstream s1{ expr };
                // parsing postfix -> infix, calculating the result from postfix notation
                cout << " " << ToInfix(expr) << "  =  " << calcRPN(istream_iterator<string>{s1}, {}) << "\n\n";

                // outputfile << ToInfix(expr) << "  =  " << calcRPN(istream_iterator<string>{s1}, {}) << "\n";

                cout << "Infix -> postfix\n> ";
                getline(cin, expr);

                if (expr != "-q")
                {
                    stringstream s2{ expr };
                    string s3 = ToPostfix(istream_iterator<string>{s2}, {});
                    stringstream s4{ s3 };

                    // parsing infix -> postfix, calculating the result from infix notation
                    cout << s3 << "  =  " << calcRPN(istream_iterator<string>{s4}, {}) << "\n\n";

                    // outputfile << s3 << "  =  " << calcRPN(istream_iterator<string>{s4}, {}) << "\n\n";
                }
            }
    }
    
    outputfile.close();
    return 0;
}