#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include "locale.h"

// Запись в БД
typedef struct record
{
    char _str[512];
    struct record* _next_record;
    
} record;

// Сама БД
typedef struct data_base
{
    record* _rcrd;
    
} data_base;

// Проверяем, если ли уже запись в БД
int check(const data_base* db, char* str);

// Добавить запись в БД
int insert(data_base* db, char *str);

// Получить число записей в БД
int get_records_count(const data_base* db);

// Считываем БД из файла
int read_bd(data_base* db, const char* file_name);

// Принтуем БД
void print(const data_base* db);