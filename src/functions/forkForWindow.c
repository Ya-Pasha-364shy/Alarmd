#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Функция вызова всплывающего окна */
void forkForWindow(int cnt)
{
    if (cnt <= 0)
        return;
    char buff[256];
    snprintf(buff, 256, "%d", cnt);
    pid_t pid = fork();
    if (pid == -1)
    {
        printInLog("Unable to fork in function forkForWindow()",
                   NULL, NULL, NULL, 0.0, 0.0);
    }
    else if (pid > 0)
    {
        signal(SIGCHLD, SIG_IGN); // ignore signal kill child process
    }
    else
    {
        // we are the child
        char cwd[1024];              // рабочая директория
        char *cwd1 = "/bin/alarmpj"; // расположение всплывающего окна
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            printInLog("Function getcwd() error", NULL, NULL, NULL, 0.0, 0.0);
        }
        char *windowName = (char *)calloc(strlen(cwd) + strlen(cwd1) + 1, sizeof(char));
        strcat(windowName, cwd);
        strcat(windowName, cwd1);
        if (execl(windowName, windowName, buff, NULL) == -1)
        {
            printInLog("Unable to exec in function forkForWindow()",
                       NULL, NULL, NULL, 0.0, 0.0);
        }
        free(windowName);
    }
}