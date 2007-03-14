#ifndef RSH_H
#define RSH_H


typedef struct
{
    char * command;
    char * aliasfor;
    int hidden;
}* command_t;


typedef struct s_node
{
    struct s_node * next;
    command_t cmd;
}* node_t;


typedef struct
{
    node_t head;
    size_t size;
}* command_list_t;


command_t new_command(char * cmd, char * aliasfor, int hidden);
void free_command(command_t c);



command_list_t new_command_list();
void free_command_list(command_list_t c);
command_list_t add_head_command_list(command_list_t c, char * cmd, char * aliasfor, int hidden);



node_t new_node(node_t next, command_t cmd);
void free_node(node_t n);


int exec_command(char * str, command_list_t cmds);


#endif
