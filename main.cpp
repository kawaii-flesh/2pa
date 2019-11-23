/*
Author:	kawaii-flesh
License: GPL 3.0
    2pa - Tool for patch files
    Copyright (C) 2019  kawaii-flesh
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    int flag = check_flags(argc, argv);
    if(flag == 1)
    {
        cout << "2pa [action] [old file] <...>\n";
        cout << "=========================\n";
        cout << "c - create patch file\n";
        cout << "p - patch file\n";
        cout << "v - visualizing difference\n";
        cout << "2pa c [old file] [new file] [output patch file]\n";
        cout << "2pa p [old file] [patch file]\n";
        cout << "2pa v [old file] [new file] [type(0-2)]\n";
        cout << "=========================\n";
        return 0;
    }
    if(flag == 2)
    {
        if(argc != 5)
        {
            cout << "2pa c [old file] [new file] [output patch file]\n";
            return 1;
        }
        create_patch(argv);
    }
    else if(flag == 3)
    {
        if(argc != 4)
        {
            cout << "2pa p [old file] [patch file]\n";
            return 1;
        }
        use_patch(argv);
    }
    else if(flag == 4)
    {
        if(argc != 5)
        {
            cout << "2pa v [old file] [new file] [type(0-2)]\n";
            return 1;
        }
        visual_diff(argv);
    }
    return 0;
}
