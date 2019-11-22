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
#include <sstream>

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
    else if(!strcmp(argv[1], "v")) {return 4;}
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
    long int nodb{};

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
            nodb += count;
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
    cout << "Number of different bytes: " << nodb << endl;
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
    long int nodb{};
    while(true)
    {
        long int offset, size;
        patch_file >> offset;
        patch_file.get();
        if(patch_file.eof()) break;
        patch_file >> size;
        nodb += size;
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
    cout << "Bytes patched: " << nodb << endl;
    return 0;
}

void byte_to_string(unsigned char byte, char *buff)
{
    for(int i = 1; i >= 0; i--)
    {
    unsigned char first = byte % 0x10;
    if(first == 0)
    {
        buff[i] = '0';
    }
    else if(first == 1)
    {
        buff[i] = '1';
    }
    else if(first == 2)
    {
        buff[i] = '2';
    }
    else if(first == 3)
    {
        buff[i] = '3';
    }
    else if(first == 4)
    {
        buff[i] = '4';
    }
    else if(first == 5)
    {
        buff[i] = '5';
    }
    else if(first == 6)
    {
        buff[i] = '6';
    }
    else if(first == 7)
    {
        buff[i] = '7';
    }
    else if(first == 8)
    {
        buff[i] = '8';
    }
    else if(first == 9)
    {
        buff[i] = '9';
    }
    else if(first == 0xa)
    {
        buff[i] = 'a';
    }
    else if(first == 0xb)
    {
        buff[i] = 'b';
    }
    else if(first == 0xc)
    {
        buff[i] = 'c';
    }
    else if(first == 0xd)
    {
        buff[i] = 'd';
    }
    else if(first == 0xe)
    {
        buff[i] = 'e';
    }
    else if(first == 0xf)
    {
        buff[i] = 'f';
    }
    byte /= 0x10;
    }
}

int visual_diff(char *argv[])
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
    if(argv[4][0] == '0')
    {
        cout << " -offset-  0 1 2 3 4 5 6 7 8 9 A B C D E F\n";
        char old_byte{}, new_byte{};
        long int offset = 0;
        string offset_str;
        while(!old_file.eof() || !new_file.eof())
        {            
            stringstream sstr;
            sstr << hex << offset;
            offset_str = sstr.str();
            for(int i = offset_str.length(); i < 8; i++) offset_str = '0' + offset_str;
            offset_str = "0x" + offset_str;
            cout << offset_str << ' ';
            for(int i = 0; i < 16; i++)
            {
                old_byte = 0;
                new_byte = 0;
                old_file.get(old_byte);
                new_file.get(new_byte);
                if(old_file.eof() && new_file.eof()) break;
                if(old_file.eof() || new_file.eof())
                    cout << "\x1b[33m-\x1b[0m ";
                else if(old_byte != new_byte)
                {
                    cout << "\x1b[31m#\x1b[0m ";
                }
                else if(old_byte == new_byte)
                    cout << "\x1b[32m#\x1b[0m ";
            }
            cout << endl;
            offset += 0x10;
        }
    }
    else if(argv[4][0] == '1')
    {
        cout << " -offset-  |O 0 N|O 1 N|O 2 N|O 3 N|O 4 N|O 5 N|O 6 N|O 7 N|O 8 N|O 9 N|O A N|O B N|O C N|O D N|O E N|O F N|\n";
        char old_byte{}, new_byte{};
        char buff[3];
        char space{};
        long int offset = 0;
        string offset_str;
        while(!old_file.eof() || !new_file.eof())
        {
            stringstream sstr;
            sstr << hex << offset;
            offset_str = sstr.str();
            for(int i = offset_str.length(); i < 8; i++) offset_str = '0' + offset_str;
            offset_str = "0x" + offset_str;
            cout << offset_str << ' ';
            bool diff = false;
            for(int i = 0; i < 16; i++)
            {                
                old_byte = 0;
                new_byte = 0;
                space = '|';
                if(i == 0) cout << space;
                old_file.get(old_byte);
                new_file.get(new_byte);
                if(old_file.eof() && new_file.eof()) break;
                if(old_file.eof() || new_file.eof())
                {
                    diff = true;
                    if(old_file.eof())
                    {
                        cout << "\x1b[33m--\x1b[0m ";
                        byte_to_string(new_byte, buff);
                        cout << "\x1b[31m" << buff << "\x1b[0m" << space;
                    }
                    else
                    {
                        byte_to_string(old_byte, buff);
                        cout << "\x1b[31m" << buff << "\x1b[0m ";
                        cout << "\x1b[33m--\x1b[0m" << space;
                    }
                }
                else if(old_byte != new_byte)
                {
                    byte_to_string(old_byte, buff);
                    cout << "\x1b[31m" << buff << ' ' << "\x1b[0m";
                    byte_to_string(new_byte, buff);
                    cout << "\x1b[31m" << buff << "\x1b[0m" << space;
                    diff = true;
                }
                else if(old_byte == new_byte)
                {
                    byte_to_string(old_byte, buff);
                    cout << "\x1b[32m" << buff << ' ' << buff << "\x1b[0m" << space;
                }
            }
            if(diff) cout << " !";
            cout << endl;            
            offset += 0x10;
        }
    }

    return 0;
}
