#include "delet_func.h"

void delete_lexemes(lexeme_t *lexemes)
{
    int i = 0;
    while(lexemes[i].token != EOI)
    {
        if(lexemes[i].token == WORDS)
        {
            free(lexemes[i].name);
        }
        i++;
    }
    free(lexemes);
}

void delete_tree_build(tree_t *tree)
{
    if(tree != NULL)
    {
        delete_tree_build(tree -> left);
        delete_tree_build(tree -> right);
        if(tree -> token == PROG)
        {
            free(tree -> command);
        }
        free(tree);
    }
}

void delete_tree(tree_t *tree)
{
    if(tree != NULL)
    {
        delete_tree(tree -> left);
        delete_tree(tree -> right);
        if(tree -> token == PROG)
        {
            int i = 0;
            while((tree -> command)[i] != NULL)
            {
                free((tree -> command)[i]);
                i++;
            }
            free(tree -> command);
        }
        else if(tree -> token == REDIRECTION_I)
        {
            free(tree -> redirection);
        }

        else if(tree -> token == REDIRECTION_O)
        {
            free(tree -> redirection);
        }

        else if(tree -> token == REDIRECTION_OO)
        {
            free(tree -> redirection);
        }
        free(tree);
    }
}