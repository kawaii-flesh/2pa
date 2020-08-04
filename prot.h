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
