#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

enum TOKENS {EOI, SEMICOLON, NEW_LINE, AMPERSANT, REDIRECTION_I, 
    REDIRECTION_O,REDIRECTION_OO, PIPE, WORDS, LEFT_BRACKET, RIGHT_BRACKET, PROG };

typedef struct node
{
    enum TOKENS token;
    char *redirection;
    char **command;
    struct node *left;
    struct node *right;
}tree_t;

typedef struct
{
    enum TOKENS token;
    char *name;
}lexeme_t;
