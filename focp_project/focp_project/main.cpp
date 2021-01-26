#include "functions.h"
using namespace std;

int main(int argc, char* argv[])
{
    bool o_flag = false;
    bool i_flag = false;
    bool r_flag = false;
    bool c_flag = false;
    bool h_flag = false;
    string input, output;

    printArgs(argc, argv);

    if (argc < 5) {
        show_usage(argv[0]);
        return 1;
    }

    showArgs(argc, argv, &o_flag, &i_flag, &r_flag, &c_flag, &h_flag, &input, &output);

    system("pause");
    cout << "\n";

    if (c_flag == true && i_flag == false)
    {
        interactiveMode(o_flag, r_flag, output);
    }

    if (i_flag == true)
    {
        inputMode(o_flag, r_flag, c_flag, input, output);
    }
    
    if (i_flag == false && c_flag == false)
    {
        cout << "\nYou haven't chosen input type (interactive or from file).\nRestart the program with one of those.\n";
    }
    
    return 0;
}