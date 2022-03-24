#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define PROCESS_COUNT 2

int main(int argc, char **argv)
{
    int max_level = atoi(argv[0]);
    int level = atoi(argv[1]);

    pid_t pids[PROCESS_COUNT];

    for (int i = 0; i < PROCESS_COUNT; i++)
    {
        if ( (pids[i] = fork()) == 0)
        {
            level++;

            // Выводим уровень, нового потомка и его родителя
            printf("Уровень - %d. Процесс %d родился. (Родитель - %d)\n", 
            level, (int)getpid(), (int)getppid());

            // Если вывели потомка на последнем уровне, то просто завершаем его
            if (level == max_level) 
                exit(0);

            // Иначе снова запускаем программу process
            char curr_level[8];
            snprintf(curr_level, sizeof(curr_level), "%d", level);

            if (execl("process", argv[0], curr_level, NULL) == -1)
            {
                errno = ENOENT;
                perror("Не удалось запустить программу process\n");
                exit(3);
            }

            exit(0);
        }
        else if (pids[i] == -1)
        {
            errno = ESRCH;
            fprintf(stderr, "Процесс %d не смог создать потомка\n", (int)getpid());
            exit(1);
        }
    }

    //-------------------------------------------------------------------------
    // Ждем завершения потомков и завершаемся сами
    int status;

    for (int i = 0; i < PROCESS_COUNT; i++)
    {
        if ( (waitpid(pids[i], &status, 0)) != -1)
        {
            if (status == 0)
                printf("\tПроцесс %d успешно завершился. (Родитель - %d)\n", pids[i], (int)getpid());
            else 
                printf("\tПроцесс %d завершился c ошибкой. (Родитель - %d)\n", pids[i], (int)getpid());
        }
        else
        {
            printf("Ошибка ожидания завершения процесса %d\n", pids[i]);
        }

    }

    printf("Процесс %d завершился\n", (int)getpid());
            
    exit(0);   
}