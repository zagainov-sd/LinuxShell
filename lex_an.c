#include "lex_an.h"

lexeme_t *lex_analyze(void)
{
    lexeme_t *arr = NULL;
    int arr_len = 0;
    int brackets_col = 0;
    int c;
    while((c = getchar()) != EOF)
    {
        switch(c)
        {
            case '(':
                brackets_col++;
                arr_len++;
                arr = realloc(arr, arr_len * sizeof(lexeme_t));
                arr[arr_len - 1].name = NULL;
                arr[arr_len - 1].token = LEFT_BRACKET;
                continue;

            case ')':
                brackets_col--;
                if(brackets_col < 0)
                {
                    delete_lexemes(arr);
                    fprintf(stderr, "missing opening bracket\n");
                    exit(1);
                }
                (arr_len)++;
                arr = realloc(arr, arr_len * sizeof(lexeme_t));
                arr[arr_len - 1].name = NULL;
                arr[arr_len - 1].token = RIGHT_BRACKET;
                continue;

            case ';':
                (arr_len)++;
                arr = realloc(arr, arr_len * sizeof(lexeme_t));
                arr[arr_len - 1].name = NULL;
                arr[arr_len - 1].token = SEMICOLON;
                continue;

            case '&':
                arr_len++;
                arr = realloc(arr, arr_len * sizeof(lexeme_t));
                arr[arr_len - 1].name = NULL;
                arr[arr_len - 1].token = AMPERSANT;
                continue;

            case '\n':
                arr_len++;
                arr = realloc(arr, arr_len * sizeof(lexeme_t));
                arr[arr_len - 1].name = NULL;
                arr[arr_len - 1].token = NEW_LINE;
                continue;

            case '|':
                arr_len++;
                arr = realloc(arr, arr_len * sizeof(lexeme_t));
                arr[arr_len - 1].name = NULL;
                arr[arr_len - 1].token = PIPE;
                continue;

            case '>':

                if((c = getchar()) != '>')
                {
                    ungetc(c, stdin);
                    arr_len++;
                    arr = realloc(arr, arr_len * sizeof(lexeme_t));
                    arr[arr_len - 1].name = NULL;
                    arr[arr_len - 1].token = REDIRECTION_O;
                    continue;
                }
                else
                {
                    arr_len++;
                    arr = realloc(arr, arr_len * sizeof(lexeme_t));
                    arr[arr_len - 1].name = NULL;
                    arr[arr_len - 1].token = REDIRECTION_OO;
                    continue;
                }

            case '<':
                arr_len++;
                arr = realloc(arr, arr_len * sizeof(lexeme_t));
                arr[arr_len - 1].name = NULL;
                arr[arr_len - 1].token = REDIRECTION_I;
                continue;

            default:

                if(((c != ';') && (c != '&')) && ((c != '|') && (c != '>'))
                && (c != '<') && (c != '(') && (c != ')') && (!isspace(c)))
                {
                    char *word = NULL;
                    int word_len = 0;
                    do
                    {
                        word_len++;
                        word = realloc(word, word_len * sizeof(char));
                        word[word_len - 1] = c;
                        c = getchar();
                    } while(((c != ';') && (c != '&')) && ((c != '|') && (c != '>'))
                        && (c != '<') && (c != '(') && (c != ')') && (!isspace(c)) && (c != EOF));

                    ungetc(c, stdin);

                    word_len++;
                    word = realloc(word, word_len * sizeof(char));
                    word[word_len - 1] = '\0';

                    arr_len++;
                    arr = realloc(arr, arr_len * sizeof(lexeme_t));
                    arr[arr_len - 1].name = word;
                    arr[arr_len - 1].token = WORDS;
                }
        }
    }

    arr_len++;
    arr = realloc(arr, arr_len * sizeof(lexeme_t));
    arr[arr_len - 1].name = NULL;
    arr[arr_len - 1].token = EOI;

    if(brackets_col > 0)
    {
        delete_lexemes(arr);
        fprintf(stderr, "missing closing bracket\n");
        exit(1);
    }

    return arr;
}
