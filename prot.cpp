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

#include "prot.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int check_flags(int argc, char *argv[])
{
    for(int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "--help"))
        {
            return 1;
        }
    }
    if(!strcmp(argv[1], "c")) {return 2;}
    else if(!strcmp(argv[1], "p")) {return 3;}
    return 1;
}

int create_patch(char *argv[])
{
    ifstream old_file(argv[2], ios::binary);
    if(!old_file.is_open())
    {
        cout << "Can't open file: " << argv[2] << endl;
        return 1;
    }
    ifstream new_file(argv[3], ios::binary);
    if(!new_file.is_open())
    {
        old_file.close();
        cout << "Can't open file: " << argv[3] << endl;
        return 1;
    }
    ofstream patch_file(argv[4]);
    if(!patch_file.is_open())
    {
        old_file.close();
        new_file.close();
        cout << "Can't open file: " << argv[4] << endl;
        return 1;
    }

    char old_byte{}, new_byte{};
    long int old_offset{};
    long int new_offset{};

    while(true)
    {
        long int count = 0; // number of not equal bytes for write
        old_file.get(old_byte);
        new_file.get(new_byte);
        if(new_file.eof()) // check eof !delete!
        {
            old_file.close();
            new_file.close();
            patch_file.close();
            break;
        }
        if(old_byte != new_byte || old_file.eof()) // compare two byte
        {
            new_offset = old_offset; // offset in file
            vector<char> buff; // for bytes
            while(old_byte != new_byte || old_file.eof())
            {
                buff.push_back(new_byte);
                old_file.get(old_byte); // next byte
                new_file.get(new_byte); //
                count++; // inc noneb
                new_offset++; // for after use
                if(new_file.eof()) break;
            }
            patch_file << old_offset << '\x00'; // write offset
            patch_file << count << '\x00'; // write noneb
            for (char ch : buff) // write bytes
            {
                patch_file << ch;
            }
            buff.clear();
            old_offset = new_offset; // reset offset
        }
        old_offset++;
    }
    old_file.close();
    new_file.close();
    cout << "Patch file created!\n";
    return 0;
}

int use_patch(char *argv[])
{
    ofstream old_file(argv[2], ios::binary | ios::in | ios::out);
    if(!old_file.is_open())
    {
        cout << "Can't open file: " << argv[2] << endl;
        return 1;
    }
    ifstream patch_file(argv[3]);
    if(!patch_file.is_open())
    {
        old_file.close();
        cout << "Can't open file: " << argv[3] << endl;
        return 1;
    }
    while(true)
    {
        long int offset, size;
        patch_file >> offset;
        patch_file.get();
        if(patch_file.eof()) break;
        patch_file >> size;
        patch_file.get();
        char *buff = new char[size];
        patch_file.read(buff, size);
        old_file.seekp(offset, ios::beg);
        old_file.write(buff, size);
        delete [] buff;
    }
    old_file.close();
    patch_file.close();
    cout << "Patched - done!\n";
    return 0;
}
