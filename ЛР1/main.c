#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <locale.h>

const int MAX_SIZE = 20;

typedef struct
{
    char name[256];
    int links;
} files_t;

int cmpf(const void *_a, const void *_b) // функция-компаратор для сортировки структуры файлов (имя+число ссылок)
{
    files_t *a = (files_t *) _a;
    files_t *b = (files_t *) _b;
    return b->links - a->links;
}

int recurse_func(char* path)
{
    DIR *d;
    int cnt=0;
    struct dirent *dir; // сведения о файле
    struct stat dstat;  // подробная статистика о файле
    char buf[512];
    files_t* f = malloc(sizeof(files_t));
    printf("Directory: %s\n", path);
    d = opendir(path);
	
    if(!d)
    {
        perror("Not directory or does not exist OR no read permission!\n");
        return 1;
    }
	
    while ((dir = readdir(d)) != NULL)
           {
	       // Если обычный файл
               if(dir->d_type==DT_REG)
	       {
                   sprintf(buf, "%s/%s", path, dir->d_name);
                   stat(buf, &dstat);
                   cnt++;
                   f=realloc(f,sizeof(files_t) * cnt);
                   memcpy(f[cnt-1].name,dir->d_name, sizeof(dir->d_name)); // копируем в массив имя файла
                   f[cnt-1].links = dstat.st_nlink; // и число ссылок на него
               }

           }
    // Сортируем массив структур
    qsort(f,cnt,sizeof(files_t),cmpf);
    
    int i = 0;
    for(; i < cnt; i++) 
		printf("     file: %s - %d reference\n", f[i].name, f[i].links);
    printf("---------------------------------------------\n");

    free(f);
    	
    rewinddir(d);
	
    while ((dir = readdir(d)) != NULL)
           {
	       // Если файл является директорией
               if(dir->d_type==DT_DIR)
               {
                   if(strcmp(dir->d_name,"..") && strcmp(dir->d_name,"."))
                   {
                        sprintf(buf,"%s/%s",path,dir->d_name);
                        recurse_func(buf);
                   }
               }
           }

    closedir(d);
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Rus");
    if(argc != 2) 
    {
	if (argc < 2)
        {    
             perror("Initial directory name not set\n");
             return 2;
        }
	else
	{
	    perror("Too many parameters\n");
       	    return 3;
	}
    }
    recurse_func(argv[1]);

    return 0;
}
