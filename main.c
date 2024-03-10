#include "main.h"

tree_t *help(tree_t *tree)
{
    static tree_t *tree_save = NULL;
    if(tree != NULL)
    {
        tree_save = tree;
    }
    return(tree_save);
}

void delete_all(void)
{
    tree_t *tree_save;
    tree_save = help(NULL);
    delete_tree(tree_save);
}

int main(int argc, char **argv)
{
    lexeme_t *lexemes;
    lexemes = lex_analyze();
    

    tree_t *tree = NULL;
    tree = tree_build(lexemes);
    help(tree);
    atexit(delete_all);
    free(lexemes);

    int exit_code;
    if(tree != NULL)
    {
        exit_code = run_tree(tree);
    }
    
    exit(exit_code);
}