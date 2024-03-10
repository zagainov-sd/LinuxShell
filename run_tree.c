#include "run_tree.h"

int run_tree(tree_t *tree)
{
    if((tree -> token == EOI) || (tree -> token == SEMICOLON) || (tree -> token == NEW_LINE))
    {
        int exit_code = run_tree(tree -> left);
        if(tree -> right != NULL)
        {
            exit_code = run_tree(tree -> right);
        }
        return exit_code;
    }
    else if(tree -> token == PIPE)
    {
        int exit_code;
        pid_t pid1, pid2;
        int fd[2];
        if(pipe(fd) == -1)
        {
            return 1;
        }
        if((pid1 = fork()) == 0)
        {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            exit_code = run_tree(tree -> left);
            exit(exit_code);
        }
        if((pid2 = fork()) == 0)
        {
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            exit_code = run_tree(tree -> right);
            exit(exit_code);
        }
        close(fd[0]);
        close(fd[1]);
        if((pid1 < 0) || (pid2 < 0))
        {
            fprintf(stderr, "error son generation");
            return 1;
        }   
        
        int status;
        waitpid(pid1, &status, 0);
        waitpid(pid2, &status, 0);

        if(WIFEXITED(status))
        {
            exit_code = (WEXITSTATUS(status));
        }
        return(exit_code);

    }

    else if(tree -> token == AMPERSANT)
    {
        pid_t pid;
        int exit_code = 0;
        if((pid = fork()) == 0)
        {
            run_tree(tree -> left);
            exit(0);
        }
        if(pid < 0)
        {
            fprintf(stderr, "error son generation");
            return 1;
        }
        if(tree -> right != NULL)
        {
            exit_code = run_tree(tree -> right);
        }
        waitpid(pid, NULL, 0);
        return(exit_code);
    }

    else if(tree -> token == REDIRECTION_I)
    {
        int exit_code;
        pid_t pid;
        int fd = open(tree -> redirection, O_RDONLY);
        if(fd == -1)
        {
            fprintf(stderr, "error open file");
            return 1;
        }
        if((pid = fork()) == 0)
        {
            dup2(fd, 0);
            exit_code = run_tree(tree -> left);
            exit(exit_code);
        }
        if(pid < 0)
        {
            fprintf(stderr, "error son generation");
            return 1;
        }
        int status;
        waitpid(pid, &status, 0);
        if(WIFEXITED(status))
        {
            exit_code = (WEXITSTATUS(status));
        }
        return(exit_code);
    }

    else if(tree -> token == REDIRECTION_O)
    {
        int exit_code;
        pid_t pid;
        int fd = open(tree -> redirection, O_WRONLY | O_TRUNC | O_CREAT, 0666);
        if(fd == -1)
        {
            fprintf(stderr, "error open file");
            return 1;
        }
        if((pid = fork()) == 0)
        {
            dup2(fd, 1);
            exit_code = run_tree(tree -> left);
            exit(exit_code);
        }
        if(pid < 0)
        {
            fprintf(stderr, "error son generation");
            return 1;
        }
        int status;
        waitpid(pid, &status, 0);
        if(WIFEXITED(status))
        {
            exit_code = (WEXITSTATUS(status));
        }
        return(exit_code);
    }

    else if(tree -> token == REDIRECTION_OO)
    {
        int exit_code;
        pid_t pid;
        int fd = open(tree -> redirection, O_WRONLY | O_APPEND | O_CREAT, 0666);
        if(fd == -1)
        {
            return 1;
        }
        if((pid = fork()) == 0)
        {
            dup2(fd, 1);
            exit_code = run_tree(tree -> left);
            exit(exit_code);
        }
        if(pid < 0)
        {
            fprintf(stderr, "error son generation");
            return 1;
        }
        int status;
        waitpid(pid, &status, 0);
        if(WIFEXITED(status))
        {
            exit_code = WEXITSTATUS(status);
        }
        return(exit_code);
    }

    else if(tree -> token == PROG)
    {
        int exit_code = 0;
        pid_t pid;
        if((pid = fork()) == 0)
        {
            execvp((tree -> command)[0], tree -> command);
            fprintf(stderr, "program launch error");
            exit(127);
        }
        int status;
        waitpid(pid, &status, 0);

        if(WIFEXITED(status))
        {
            exit_code = (WEXITSTATUS(status));
        }
        else
        {
            if(WIFSIGNALED(status))
            {
                exit_code = (128 + WTERMSIG(status));
            }
        }
        return(exit_code);        
    }
    return 0;
}