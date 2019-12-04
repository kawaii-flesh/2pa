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

#ifndef PROT_H
#define PROT_H

struct cmd_flags
{
    long int dual_offset   = 0;
    long int first_offset  = 0;
    long int second_offset = 0;
    long int portion       = 0;
};

void set_flags_value(int argc, char *argv[], cmd_flags &flags);
void check_flags(int argc, char *argv[], cmd_flags &flags);
int check_action(int argc, char *argv[]);
int create_patch(char *argv[], cmd_flags &flags);
int use_patch(char *argv[], cmd_flags &flags);
int visual_diff(char *argv[], cmd_flags &flags);

#endif
