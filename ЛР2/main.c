#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Структура для хранения результата дочернего процесса
struct child_proc_res
{
    int id;
    unsigned long long factorial;
};

// Вычисление факториала
unsigned long long factorial(int n)
{
    unsigned long long fact = 1;

    for (int i = 2; i <= n; i++)
        fact *= i;

    return fact;
}

// Записать результат
void save_proc_result(unsigned long long fact, int tmp_file_id, struct child_proc_res cpr)
{
    cpr.id = getpid();
    cpr.factorial = fact;

    if (write(tmp_file_id, &cpr, sizeof(cpr)) != sizeof(cpr))
    {
        perror("Не удалось записать результат!\n");
        exit (7);
    }
}

int main(int argc, char *argv[])
{
    // Число сочетаний с повторениями из n по m
    int n, m;

    // id процессов, которые считают (n + m - 1)!, 
    // m! и (n - 1)! соответственно
    int proc_id_nm_1, proc_id_m, proc_id_n_1;

    // Фаткориалы (n + m - 1)!, 
    // m! и (n - 1)! соответственно
    unsigned long long nm_1_fact, m_fact, n_1_fact;

    // Необходимо для определения размера файла
    struct stat file_stat;

    // Структура для хранения результата дочернего процесса
    struct child_proc_res cpr;

    // ----------------------------------------------------------------------------
    if (argc != 3)
    {
        perror("Неверное число аргументов!\n");
        exit (1);
    }

    if (!sscanf(argv[1], "%d", &n) || !sscanf(argv[2], "%d", &m))
    {
        perror("Неверный тип переданных аргументов!\n");
        exit (2);
    }

    if (m < 0 || n < 1 || (n + m - 1) > 20)
    {
        perror("Значения параметров вне допустимых пределов!\n");
        exit (3);
    }

    if (n < m)
    {
        perror("n должен быть больше k\n");
        exit (4);
    }

    // ----------------------------------------------------------------------------
    int tmp_file_id;
    if ((tmp_file_id = open("tmp_file", O_CREAT | O_RDWR)) == -1)
    {
        perror("Не удалось открыть файл\n");
        exit (5);
    }

    // ----------------------------------------------------------------------------
    proc_id_nm_1 = fork();
    if (proc_id_nm_1 == 0)
    {
        save_proc_result(
            factorial(n + m - 1),
            tmp_file_id,
            cpr
        );
        exit (0);
    }
    else if (proc_id_nm_1 == -1)
    {
        perror("Не удалось создать процесс!\n");
        exit (6);
    }

    // ----------------------------------------------------------------------------
    proc_id_m = fork();
    if (proc_id_m == 0)
    {
        save_proc_result(
            factorial(m),
            tmp_file_id,
            cpr
        );

        exit (0);
    }
    else if (proc_id_m == -1)
    {
        perror("Не удалось создать процесс!\n");
        exit (6);
    }

    // ----------------------------------------------------------------------------
    proc_id_n_1 = fork();
    if (proc_id_n_1 == 0)
    {
        save_proc_result(
            factorial(n - 1),
            tmp_file_id,
            cpr
        );
        exit (0);
    }
    else if (proc_id_n_1 == -1)
    {
        perror("Не удалось создать процесс!\n");
        exit (6);
    }

    // Родительский опрашивает временный файл, пока он не заполнится 
    // необходимым объемом данных
    do {
        fstat(tmp_file_id, &file_stat);
    } while(file_stat.st_size != sizeof (struct child_proc_res) * 3);

    // ----------------------------------------------------------------------------
    lseek(tmp_file_id, 0, SEEK_SET);

    for(int proc_id = 1; proc_id <= 3; proc_id++)
    {
        while(!read(tmp_file_id, &cpr, sizeof cpr));

        if(cpr.id == proc_id_nm_1)  
            nm_1_fact = cpr.factorial;

        else if(cpr.id == proc_id_m)     
            m_fact = cpr.factorial;

        else if(cpr.id == proc_id_n_1)   
            n_1_fact = cpr.factorial;
    }

    close(tmp_file_id);

    printf("Число сочетаний с повторениями: %lld\n", nm_1_fact / (m_fact * n_1_fact));

    remove("tmp_file");

    return 0;
}