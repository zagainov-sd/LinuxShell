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

lexeme_t *lex_analyze(void);

struct node *tree_build(lexeme_t *lexemes);

//void delete_lexemes(lexeme_t *lexemes);
void delete_tree(tree_t *tree);

int run_tree(tree_t *tree);