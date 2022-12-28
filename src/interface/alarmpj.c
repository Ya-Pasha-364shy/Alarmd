#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#define  MAX_STROKE_LEN 168

char* find_conf_file(void);
void print_conf(void);
void print_log(void);
void print_alarm(void);
void delete_alarm(void);
float get_num_kb(char buffer[128]);
float printDimension(float volume, char dim[3]);
void print_sysparam(void);
int get_item(int count);
void print_menu(void);

char* find_conf_file(void)
{
    FILE *dir = fopen("/var/tmp/configdir.txt", "r");
    long size = 0;
    fseek(dir, 0L, SEEK_END);
    size = ftell(dir) - 1;
    rewind(dir);
    char *dir_conf = (char*)malloc(sizeof(char)*size+1);
    fread(dir_conf, size, 1, dir);   
    dir_conf[size+2] = '\0';
    fclose(dir);
    return dir_conf;
}

void print_conf(void)
{
    char *dir_conf = find_conf_file();
    FILE *file = fopen(dir_conf, "r");
    char c;
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(file);
    }
    fclose(file);
    free(dir_conf);
    printf("\n");
}

void print_log(void)
{
    FILE *file = fopen("/var/log/Logfile.log", "r");
    char c;
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(file);
    }
    fclose(file);
}

void print_alarm(void)
{
    pid_t p = fork(); // create a child process
    if (p == 0)
    {
        execl("/bin/sh", "sh", "-c", "/bin/grep Alarm /var/log/Logfile.log", (char *)NULL);
        perror("execl"); // reaching this line is necessarily a failure
        exit(1); // terminate child process in any case
    }
    waitpid(p, NULL, 0);
}

void delete_alarm(void)
{
    pid_t p = fork(); // create a child process
    if (p == 0)
    {
    execl("/bin/sh", "sh", "-c", "sudo /bin/sed -i '/Alarm/d' /var/log/Logfile.log", (char *)NULL);
    perror("execl"); 
    exit(1); 
    }
    waitpid(p, NULL, 0);
}

float get_num_kb(char buffer[128])
{
    int i = 0;
    int num = 0;
    while (buffer[i])
    {
        if (buffer[i] >= 48 && buffer[i] <= 57)
        {
            num = num * 10 + buffer[i] - 48;
        }
        i++;
    }

    return num;
}

float print_dimension(float volume, char dim[3]) {

    for (int i = 0; i < 3; i++) {
		if (volume / 1024.0 < 1.0) {
			if (i == 0) {
				strcpy(dim, "Kb\0");
				return volume;
			}
			if (i == 1) {
				strcpy(dim, "Mb\0");
				return volume;
			}
			if (i == 2) {
				strcpy(dim, "Gb\0");
				return volume;
			}
		} else volume /= 1024.0;
	}
	strcpy(dim, "Tb\0");
	return volume;
}

void print_sysparam(void)
{
    FILE *mem_and_swap;
    char buffer_mem_total[128] = {0};
    char buffer_mem_free[128] = {0};
    char buffer_swap_total[128] = {0};
    char buffer_swap_free[128] = {0};
    mem_and_swap = fopen("/proc/meminfo", "r");
    while (strncmp(buffer_mem_total, "MemTotal", 8) != 0)
    {
        fgets(buffer_mem_total, sizeof(buffer_mem_total), mem_and_swap);
    }
    while (strncmp(buffer_mem_free, "MemFree", 7) != 0)
    {
        fgets(buffer_mem_free, sizeof(buffer_mem_free), mem_and_swap);
    }
    while (strncmp(buffer_swap_total, "SwapTotal", 9) != 0)
    {
        fgets(buffer_swap_total, sizeof(buffer_swap_total), mem_and_swap);
    }
    while (strncmp(buffer_swap_free, "SwapFree", 8) != 0)
    {
        fgets(buffer_swap_free, sizeof(buffer_swap_free), mem_and_swap);
    }
    char dimension[3];

    printf("Mem:\n");
    printf("MemTotal:\t%.2f %s\n", print_dimension(get_num_kb(buffer_mem_total),dimension), dimension);
    printf("MemFree:\t%.2f %s\n", print_dimension(get_num_kb(buffer_mem_free),dimension), dimension);
    printf("Used Mem:\t%.2f %s\n\n", print_dimension(get_num_kb(buffer_mem_total) - get_num_kb(buffer_mem_free),dimension), dimension);
    printf("Swap:\n");
    printf("SwapTotal:\t%.2f %s\n", print_dimension(get_num_kb(buffer_swap_total),dimension), dimension);
    printf("SwapFree:\t%.2f %s\n", print_dimension(get_num_kb(buffer_swap_free),dimension), dimension);
    printf("Used Swap:\t%.2f %s\n\n", print_dimension(get_num_kb(buffer_swap_total) - get_num_kb(buffer_swap_free),dimension), dimension);
    
    fclose(mem_and_swap);
    printf("Directories:\n");

    char *dir_conf = find_conf_file();
    FILE *dir = fopen(dir_conf, "r");
    char c;
    int lines_count = 0;
    while (!feof(dir))
    {

        if (fgetc(dir) == '\n')
        {
            lines_count++;
        }
    }
    rewind(dir);
    lines_count++;
    char *buffer_dir = (char*)malloc(sizeof(char)*MAX_STROKE_LEN*lines_count);
    memset(buffer_dir, 0, sizeof(buffer_dir));
    while (fread(buffer_dir, sizeof(char), MAX_STROKE_LEN*lines_count, dir));
    int flag = 0;
    int before  = 0;
    int after = 0;
    int counter = 0;
    int i = 0;
    int j = 0;
    int s = 0;
    int k = 0;
    int n = 0;

    printf("\n");
    while (!flag)
    {
        if (before >= strlen(buffer_dir))
        {
            break;
        }
        for (i = before; buffer_dir[i] != '\n'; i++)
        {
            if (buffer_dir[i] == '\0')
            {
                flag = 1;
                break;
            }
        }
        after = i;
        char *tmp = (char *)malloc(sizeof(char) * after);

        memset(tmp,0,sizeof(tmp));
        counter = 0;
        for (j = before; j < after; j++)
        {
            tmp[counter++] = buffer_dir[j];
        }
        tmp[counter] = '\0';

        counter = 0;
        if (strncmp(tmp, "directoryName", 12) == 0)
        {
            while(tmp[counter] != '\0')
            {
                if (tmp[counter++] == '/')
                {
                k = --counter;
                break;
                }
            }
            s = strlen(tmp);
            char *pdir = (char*)malloc(sizeof(char) * (s-k+1));
            pdir[(s-k)+1]='\0';
            for (n = 0; n < s; n++)
            {
                pdir[n] = tmp[k++];
                if (k == s)
                {
                    break;
                }
            }
            pid_t p = fork(); // create a child process
            if (p == 0)
            {
                execl("/usr/bin/du", "/usr/bin/du", "-sh", pdir, NULL);
                perror("execl"); 
                exit(1); 
            }
            waitpid(p, NULL, 0);
            free(pdir);
        }
        before = after + 1;
        free(tmp);
    }
    fclose(dir);
    free(buffer_dir);
}

int get_item(int count)
{
    int item = 0;
    char s[100]; // строка для считывания введённых данных
    scanf("%s", s); // считываем строку
    // пока ввод некорректен, сообщаем об этом и просим повторить его
    while (sscanf(s, "%d", &item) != 1 || item < 1 || item > count) { // sscanf(s, "%d", &item) != 1 
        printf("Неправильно выбран пункт. Попробуйте снова: "); // выводим сообщение об ошибке
        scanf("%s", s); // считываем строку повторно
    }
    return item;
}

void print_menu(void)
{
    printf("Меню взаимодействия с демоном AlarmPj:\n");
    printf("1. Вывод текущих параметров конфигурации демона.\n");
    printf("2. Вывод текущего лога.\n");
    printf("3. Вывод имеющихся аварий (выход параметра за предел).\n");
    printf("4. Очистка имеющихся аварий по запросу.\n");
    printf("5. Вывод текущих параметров системы.\n");
    printf("6. Выход.\n");
    printf("Введите цифру пункта: ");
}

int main(int argc, char *argv[])
{   
    int item = 0;
    while (item != 6)
    {
        print_menu();
        item = get_item(6);
        printf("\n");
        switch (item)
        {
            case 1:
                print_conf();
                printf("\n");
                break;
            case 2:
                print_log();
                printf("\n");
                break;
            case 3:
                print_alarm();
                printf("\n");
                break;
            case 4:
                delete_alarm();
                printf("\n");
                break;
            case 5:
                print_sysparam();
                printf("\n");
                break;
        }
    }
    return 0;
}