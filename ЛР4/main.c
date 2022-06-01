#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "data_base.h"

struct message
{
    long msg_type;
    char str[512];

} msg;

int main()
{
    data_base* db = (data_base*)malloc(sizeof(data_base));

    if (read_bd(db, "data_base.txt") == 0)
        printf("Все ок! Считали\n");
    else 
        printf("БД пустая. Добавляйте записи\n");
    // -----------------------------------------------------------

    // -----------------------------------------------------------
    int K;

    printf("Укажите количество потомков: ");
    scanf("%d", &K);

    if (K == 0)
    {
        print(db);
        return 0;
    }
    if (K < 0)
    {
        fprintf(stderr, "Число потомков не может быть отрицательным\n");
        exit(1);
    }
    if (K > 100)
    {
        fprintf(stderr, "Слишком много потомков\n");
        exit(1);
    }
    
    // -----------------------------------------------------------

    // Создаем очередь сообщений
    int msgqid;

    if (( msgqid = msgget(IPC_PRIVATE, S_IRWXU | IPC_CREAT)) < 0)
    {
        printf("Не удалось создать очередь сообщений\n");
    	exit(1);
    }

    pid_t pid;

    for (int i = 0; i < K; i++)
    {
        switch (fork())
        {
        case 0: 
        {
            msg.msg_type = 1;

            // Формируем сообщение
            sprintf(msg.str, "Привет. Я потомок №%d. Добавь меня в БД\n", i);

            if( msgsnd(msgqid, &msg, sizeof(msg), 0) == -1 ) 
            { 
                printf("Не удается отправить сообщение от потомка %d.\n", pid);
                exit(1);
            }

        	exit(0);
        }   

        case -1:
        {
            fprintf(stderr, "Не удалось создать потомка\n");
            exit(1);
        }
        }
    }

    for (int i = 0; i < K; i++)
    {
        msgrcv(msgqid, &msg, sizeof(msg), 1, 0);

        if (insert(db, msg.str) == -1)
            printf("Запись уже содержится в БД:  %s. Число записей: %d\n", msg.str, get_records_count(db));
        else
            printf("Число записей в БД:  %d\n", get_records_count(db));
    }

    print(db);

    // -----------------------------------------------------------
    if (msgctl(msgqid, IPC_RMID, NULL) == -1) 
    {
        printf("Не получается удалить очередь сообщений\n");
    }

    return 0;
}