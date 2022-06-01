#include "data_base.h"

// ================ РАБОТА С БД ================
// ---------------------------------------------
// Проверяем, если ли уже запись в БД
int check(const data_base* db, char* str)
{
    record *tmp = db->_rcrd;

    if (tmp == NULL)
        return 0;

    while (tmp)
    {
        if (strncmp(tmp->_str, str, strlen(str) - 1) == 0)
            return -1;

        tmp = tmp->_next_record;
    }

    return 0;
}

// Добавить запись в БД
int insert(data_base* db, char *str)
{
    // Проверим, нет ли этой записи в БД
    if (check(db, str) == -1)
        return -1;

    // Если нет - создаем новую запись
    record* new_record = (record*)malloc(sizeof(record));

    if (new_record == NULL)
    {
        fprintf(stderr, "Недостаточно памяти!\n");
        return -2;
    }

    strcpy(new_record->_str, str);
    new_record->_next_record = NULL;

    // Если БД еще пустая, то вставляем запись в начало
    if (db->_rcrd == NULL)
        db->_rcrd = new_record;
    // Иначе в конец
    else
    {
        record* tail = db->_rcrd;

        while (tail->_next_record)
            tail = tail->_next_record;
        
        tail->_next_record = new_record;
    }

    return 0;
}

// Считываем БД из файла
int read_bd(data_base* db, const char* file_name)
{
    FILE* data = fopen(file_name, "r");

    if (data == NULL)
    {
        fprintf(stderr, "Не получилось открыть файл %s\n", file_name);
        return -1;
    }

    char string[512];

    int ret_val;

    while (fgets (string, sizeof(string), data))
    {
        if (string[strlen(string) - 2] == '\r')
            string[strlen(string) - 2] = '\0';

        if ( (ret_val = insert(db, string)) == -1) 
        {
            fprintf(stderr, "Запись уже есть в БД!\n");
        }
        else if (ret_val == -2)
        {
            fprintf(stderr, "Ошибка добавления записи!\n");
            return -1;
        }
    }
}

// Получить число записей в БД
int get_records_count(const data_base* db)
{
    record *curr = db->_rcrd;

    if (curr == NULL)
        printf("БД не содержит записей\n");
    else
    {
        int count = 0;

        for (; curr; curr = curr->_next_record, count++);

        return count;
    }

    return 0;
}

// Принтуем БД
void print(const data_base* db)
{
    record *curr = db->_rcrd;

    if (curr == NULL)
        printf("БД не содержит записей\n");
    else
    {
        printf("__________Записи в БД__________\n");

        while (curr)
        {
            printf("%s\n", curr->_str);
            curr = curr->_next_record;
        }
        printf("\n");
    }
}
// =============================================