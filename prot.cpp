#include "prot.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdio>

using namespace std;

void set_flags_value(int argc, char *argv[], cmd_flags &flags)
{
    long int *offset = (long int *)&flags;
    for(int i = 0; i < sizeof(cmd_flags)/sizeof(long int); ++i)
    {
        if(*offset != 0 && (*offset + 1) < argc)
        {
            *offset = strtol(argv[*offset+1], NULL, 0);
        }
        ++offset;
    }
}

void check_flags(int argc, char *argv[], cmd_flags &flags)
{
    for(int i = 4; i < argc; ++i)
    {
        if(!strcmp(argv[i], "-d")) flags.dual_offset = i;
        else if(!strcmp(argv[i], "-f")) flags.first_offset = i;
        else if(!strcmp(argv[i], "-s")) flags.second_offset = i;
        else if(!strcmp(argv[i], "-p")) flags.portion = i;
    }
    set_flags_value(argc, argv, flags);
}

int check_action(int argc, char *argv[])
{
    if(!strcmp(argv[1], "c")) {return 2;}
    else if(!strcmp(argv[1], "p")) {return 3;}
    else if(!strcmp(argv[1], "v")) {return 4;}
    return 1;
}

int create_patch(char *argv[], cmd_flags &flags)
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

    if(flags.dual_offset != 0)
    {
        old_file.seekg(flags.dual_offset);
        new_file.seekg(flags.dual_offset);
    }
    if(flags.first_offset != 0)
    {
        old_file.seekg(flags.first_offset);
    }
    if(flags.second_offset != 0)
    {
        new_file.seekg(flags.second_offset);
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
                ++count; // inc noneb
                ++new_offset; // for after use
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
        ++old_offset;
    }
    if(nodb)
    {
        cout << "Patch file created!\n";
        cout << "Number of different bytes: " << nodb << endl;
    }
    else
    {
        remove(argv[4]);
        cout << "Files are equal!\n";
    }
    return 0;
}

int use_patch(char *argv[], cmd_flags &flags)
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

int visual_diff(char *argv[], cmd_flags &flags)
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

    if(flags.dual_offset != 0)
    {
        old_file.seekg(flags.dual_offset);
        new_file.seekg(flags.dual_offset);
    }
    if(flags.first_offset != 0)
    {
        old_file.seekg(flags.first_offset);
    }
    if(flags.second_offset != 0)
    {
        new_file.seekg(flags.second_offset);
    }

    char old_byte{}, new_byte{};
    char space{};
    long int offset = 0;
    string offset_str;

    string green_color  = "\x1b[32m";
    string yellow_color = "\x1b[33m";
    string red_color    = "\x1b[31m";
    string drop_color   = "\x1b[0m";
    string purp_color   = "\x1b[35m";
    string red_pulse    = "\x1b[5;31m";

    long int port = 0;

    cout << " -offset-  |F 0 S|F 1 S|F 2 S|F 3 S|F 4 S|F 5 S|F 6 S|F 7 S|F 8 S|F 9 S|F A S|F B S|F C S|F D S|F E S|F F S|\n";
    if(argv[4][0] == '0' || argv[4][0] == '1')
    {        
        char buff[3];

        if(argv[4][0] == '1')
        {
            green_color  = "";
            yellow_color = "";
            red_color    = "";
            drop_color   = "";
            purp_color   = "";
            red_pulse    = "";
        }

        while(!old_file.eof() || !new_file.eof())
        {
            stringstream sstr;
            sstr << hex << offset;
            offset_str = sstr.str();
            for(int i = offset_str.length(); i < 8; ++i) offset_str = '0' + offset_str;
            offset_str = "0x" + offset_str;            
            bool diff = false;
            for(int i = 0; i < 16; ++i)
            {                
                if(i == 0 && (!new_file.eof() || !old_file.eof())) cout << purp_color << offset_str << drop_color << ' ';                                
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
                        cout << yellow_color << "-- " << drop_color;
                        byte_to_string(new_byte, buff);
                        cout << red_color << buff << drop_color << space;
                    }
                    else
                    {
                        byte_to_string(old_byte, buff);
                        cout << red_color << buff << drop_color;
                        cout << yellow_color << " --" << drop_color << space;
                    }
                }
                else if(old_byte != new_byte)
                {
                    byte_to_string(old_byte, buff);
                    cout << red_color << buff << '~' << drop_color;
                    byte_to_string(new_byte, buff);
                    cout << red_color << buff << drop_color << space;
                    diff = true;
                }
                else
                {
                    byte_to_string(old_byte, buff);
                    cout << green_color << buff << ' ' << buff << drop_color << space;
                }
            }
            if(diff) cout << red_pulse << " !" << drop_color;
            ++port;
            if(flags.portion != 0 && port == flags.portion)
            {
                char sym = cin.get();
                if(sym == 'q') return 0;
                else if(sym == 'c') flags.portion = 0;
                port = 0;
            }
            cout << endl;
            offset += 0x10;
        }
    }
    else if(argv[4][0] == '2' || argv[4][0] == '3')
    {                
        if(argv[4][0] == '3')
        {
            green_color  = "";
            yellow_color = "";
            red_color    = "";
            drop_color   = "";
            purp_color   = "";
            red_pulse    = "";
        }

        while(!old_file.eof() || !new_file.eof())
        {
            stringstream sstr;
            sstr << hex << offset;
            offset_str = sstr.str();
            for(int i = offset_str.length(); i < 8; ++i) offset_str = '0' + offset_str;
            offset_str = "0x" + offset_str;            
            bool diff = false;
            for(int i = 0; i < 16; ++i)
            {
                if(i == 0 && (!new_file.eof() || !old_file.eof())) cout << purp_color << offset_str << drop_color << ' ';
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
                        cout << yellow_color << "--  " << drop_color;
                        if(!(new_byte >= 0x20 && new_byte <= 126)) new_byte = '.';
                        cout << red_color << new_byte << drop_color << space;
                    }
                    else
                    {
                        if(!(old_byte >= 0x20 && old_byte <= 126)) old_byte = '.';
                        cout << red_color << old_byte << drop_color;
                        cout << yellow_color << "  --" << drop_color << space;
                    }
                }
                else if(old_byte != new_byte)
                {
                    if(!(old_byte >= 0x20 && old_byte <= 126)) old_byte = '.';
                    cout << red_color << old_byte << " ~ " << drop_color;
                    if(!(new_byte >= 0x20 && new_byte <= 126)) new_byte = '.';
                    cout << red_color << new_byte << drop_color << space;
                    diff = true;
                }
                else
                {
                    if(!(old_byte >= 0x20 && old_byte <= 126)) old_byte = '.';
                    cout << green_color << old_byte << "   " << old_byte << drop_color << space;
                }
            }
            if(diff) cout << red_pulse << " !" << drop_color;
            ++port;
            if(flags.portion != 0 && port == flags.portion)
            {
                cin.tie(0);
                char sym = getc(stdin);
                if(sym == 'q') return 0;
                else if(sym == 'c') flags.portion = 0;
                port = 0;
            }
            cout << endl;
            offset += 0x10;
        }
    }
    return 0;
}
