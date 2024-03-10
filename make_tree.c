#include "make_tree.h"

struct node *expr(lexeme_t *lexemes, int *poz, int *err);
struct node *command(lexeme_t *lexemes, int *poz, int *err);
struct node *redirection(lexeme_t *lexemes, int *poz, int *err);
struct node *conveyor(lexeme_t *lexemes, int *poz, int *err);
struct node *separation(lexeme_t *lexemes, int *poz, int *err);

struct node *tree_build(lexeme_t *lexemes)
{
    int pozition = 0;
    int err = 0;
    tree_t *tree = NULL;
    tree = expr(lexemes, &pozition, &err);
    if(err)
    {
        delete_lexemes(lexemes);
        delete_tree_build(tree);
        exit(1);
    }
    return(tree);
}

struct node *expr(lexeme_t *lexemes, int *poz, int *err)
{
    if(lexemes[*poz].token == EOI)
    {
        return NULL;
    }
    tree_t *tree_part_left = separation(lexemes, poz, err);
    if(lexemes[*poz].token != EOI)
    {
        *err = 1;
        fprintf(stderr, "incorrect expression\n");
    }
    /*if(tree_part_left == NULL)
    {
        //ошибка
    }*/
    struct node *Node = malloc(sizeof(struct node));
    Node -> token = EOI;
    Node -> left = tree_part_left;
    Node -> right = NULL;
    return Node;
}

struct node *separation(lexeme_t *lexemes, int *poz, int *err)
{
    tree_t *tree_part_left = conveyor(lexemes, poz, err);
    while((lexemes[*poz].token == SEMICOLON) ||(lexemes[*poz].token == NEW_LINE) 
    || (lexemes[*poz].token == AMPERSANT))
    {
        (*poz)++;
        struct node *Node = malloc(sizeof(struct node));
        Node -> token = lexemes[*poz - 1].token;

        while(lexemes[*poz].token == NEW_LINE)
        {
            (*poz)++;
        }

        if(lexemes[*poz].token != RIGHT_BRACKET)
        {
            tree_t *tree_part_right = separation(lexemes, poz, err);
            Node -> left = tree_part_left;
            Node -> right = tree_part_right;
            tree_part_left = Node;
            Node = NULL;
        }
        else
        {
            Node -> left = tree_part_left;
            Node -> right = NULL;
            tree_part_left = Node;
            Node = NULL;
        }
    }
    return tree_part_left;
}

struct node *conveyor(lexeme_t *lexemes, int *poz, int *err)
{
    tree_t *tree_part_left = redirection(lexemes, poz, err);
    while(lexemes[*poz].token == PIPE)
    {
        (*poz)++;
        tree_t *tree_part_right = redirection(lexemes, poz, err);
        if(tree_part_right == NULL)
        {
            *err = 1;
            fprintf(stderr, "not operand\n");
            delete_tree_build(tree_part_left);
            return NULL;
        }

        struct node *Node = malloc(sizeof(struct node));
        Node -> left = tree_part_left;
        Node -> right = tree_part_right;
        Node -> token = PIPE;
        tree_part_left = Node;
        Node = NULL;
    }
    return tree_part_left;
}

struct node *redirection(lexeme_t *lexemes, int *poz, int *err)
{
    tree_t *tree_part_left = command(lexemes, poz, err);
    if((lexemes[*poz].token == REDIRECTION_I) || (lexemes[*poz].token == REDIRECTION_O) ||
        (lexemes[*poz].token == REDIRECTION_OO))
    {
        while(lexemes[*poz].token == REDIRECTION_I || lexemes[*poz].token == REDIRECTION_O ||
            lexemes[*poz].token == REDIRECTION_OO)
        {
            (*poz)++;
        }
        if(lexemes[*poz].token == WORDS)
        {
            struct node *Node = malloc(sizeof(struct node));
            Node -> left = tree_part_left;
            Node -> right = NULL;
            if(lexemes[*poz - 1].token == REDIRECTION_I)
            {
                Node -> redirection = lexemes[*poz].name;
                Node -> token = REDIRECTION_I;
            }
            else if(lexemes[*poz - 1].token == REDIRECTION_O)
            {
                Node -> redirection = lexemes[*poz].name;
                Node -> token = REDIRECTION_O;
            }
            else if(lexemes[*poz - 1].token == REDIRECTION_OO)
            {
                Node -> redirection = lexemes[*poz].name;
                Node -> token = REDIRECTION_OO;
            }
            (*poz)++;
            return Node;
        }
        else
        {
            *err = 1;
            fprintf(stderr, "not file name\n");
            delete_tree_build(tree_part_left);
            return NULL;
        }
    }
    return tree_part_left;   
}

struct node *command(lexeme_t *lexemes, int *poz, int *err)
{ 
    if(lexemes[*poz].token == WORDS)
    {
        struct node *Node = malloc(sizeof(struct node));
        Node -> left = NULL;
        Node -> right = NULL;
        char **args = NULL;
        int i = 0;
        do
        {
            args = realloc(args, (i + 1) * sizeof(char*));
            args[i] = lexemes[*poz].name;
            i++;
            (*poz)++;
        }while(lexemes[*poz].token == WORDS);
        args = realloc(args, (i + 1) * sizeof(char*));
        args[i] = NULL;

        Node -> token = PROG;
        Node -> command = args;
        return Node;
    }
    else if(lexemes[*poz].token == LEFT_BRACKET)
    {
        (*poz)++;
        tree_t *tree_part = separation(lexemes, poz, err);
        if(lexemes[*poz].token != RIGHT_BRACKET)
        {
            *err = 1;
            fprintf(stderr, "not closing bracket\n");
            delete_tree_build(tree_part);
            return NULL;
        }
        (*poz)++;
        return tree_part;
    }

    else if(lexemes[*poz].token == EOI)
    {
        return NULL;
    }

    else
    {
        *err = 1;
        fprintf(stderr, "incorrect expression\n");
        return NULL;
    }
}