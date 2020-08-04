#include <iostream>
#include "prot.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        cout << "2pa [action] [old file] <...>\n";
        cout << "=========================\n";
        cout << "c - create patch file\n";
        cout << "p - patch file\n";
        cout << "v - visualizing difference\n";
        cout << "2pa c [old file] [new file] [output patch file]\n";
        cout << "2pa p [old file] [patch file]\n";
        cout << "2pa v [old file] [new file] [type(0-3)]\n";
        cout << "=========================\n";
        return 1;
    }
    int flag = check_action(argc, argv);
    if(flag == 1)
    {
        cout << "2pa [action] [old file] <...>\n";
        cout << "=========================\n";
        cout << "c - create patch file\n";
        cout << "p - patch file\n";
        cout << "v - visualizing difference\n";
        cout << "2pa c [old file] [new file] [output patch file]\n";
        cout << "2pa p [old file] [patch file]\n";
        cout << "2pa v [old file] [new file] [type(0-3)]\n";
        cout << "=========================\n";
        return 0;
    }
    cmd_flags flags;
    check_flags(argc, argv, flags);
    if(flag == 2)
    {
        if(argc < 5)
        {
            cout << "2pa c [old file] [new file] [output patch file]\n";
            return 1;
        }
        create_patch(argv, flags);
    }
    else if(flag == 3)
    {
        if(argc < 4)
        {
            cout << "2pa p [old file] [patch file]\n";
            return 1;
        }
        use_patch(argv, flags);
    }
    else if(flag == 4)
    {
        if(argc < 5)
        {
            cout << "2pa v [old file] [new file] [type(0-2)]\n";
            return 1;
        }
        visual_diff(argv, flags);
    }
    return 0;
}
