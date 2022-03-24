#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        errno = EINVAL;
        perror("Неверное число аргументов\n");
        exit(1);
    }

    int levels = atoi(argv[1]);

    if (levels < 1 || levels > 5)
    {
        errno = EINVAL;
        perror("Число уровней должно быть в диапазоне от 1 до 5\n");
        exit(1);
    }

    //-----------------------------------------------------------------------
    // Программу process будем запускать, передавая туда номер максимального
    // уровня (в номерации с 0 это levels-1) и номер начального уровня, т.е 1,
    // т.к. нулевой уровень занята самым главным процессом

    char max_level[8], curr_level[8];
    snprintf(max_level, sizeof(max_level), "%d", levels - 1);
    snprintf(curr_level, sizeof(curr_level), "%d", 0);

    printf("Уровень - 0. Процесс %d запущен\n", (int)getpid());

    if (levels - 1 > 0)
    {    
        if (execl("process", max_level, curr_level, NULL) == -1)
        {
            errno = ENOENT;
            perror("Не удалось запустить программу process\n");
            exit(3);
        }
    }
    else
    {
        printf("Процесс %d завершен\n", (int)getpid());
        exit(0);
    }

    return 0;
}