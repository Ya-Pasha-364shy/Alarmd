#include <time.h>
#include <stdio.h>
#include <string.h>

/* Определение печати приставки в лог */
float printDimension(float volume, char dim[3]) {
    //memset(dim,0,sizeof(dim));

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

/* Вывод в лог-файл */
void printInLog(const char *text_error, const char *param_name,
                const char *limit_param_name, const char *directory_name,
                float param, float limit_param)
{
    time_t t = time(NULL);
    FILE *file;
    file = fopen("/var/log/Logfile.log", "a");
    if (!file)
    {
        file = fopen("/var/log/Logfile.log", "w");
    }
    struct tm *date = localtime(&t);
    if (text_error == NULL)
    {
        if (param_name != NULL && limit_param_name != NULL)
        {
            if (directory_name == NULL)
            {
                char dimension[3];
                char dimensionLimit[3];
                fprintf(file, "[%02d/%02d/%04d %02d:%02d:%02d] [Alarm] %s: %.2f %s, %s: %.2f %s\n", date->tm_mday,
                        date->tm_mon + 1, date->tm_year + 1900, date->tm_hour, date->tm_min, date->tm_sec,
                        param_name, printDimension(param, dimension), dimension, limit_param_name,
                        printDimension(limit_param, dimensionLimit), dimensionLimit);
            }
            else
            {
                char dimension[3];
                char dimensionLimit[3];
                fprintf(file, "[%02d/%02d/%04d %02d:%02d:%02d] [Alarm] directory: %s, %s: %.2f %s, %s: %.2f %s\n", date->tm_mday,
                        date->tm_mon + 1, date->tm_year + 1900, date->tm_hour, date->tm_min, date->tm_sec,
                        directory_name, param_name, printDimension(param, dimension), dimension, limit_param_name,
                        printDimension(limit_param, dimensionLimit), dimensionLimit);
            }
        }
        else
        {
            fprintf(file, "[%02d/%02d/%04d %02d:%02d:%02d] [Error] %s\n", date->tm_mday,
                    date->tm_mon + 1, date->tm_year + 1900, date->tm_hour, date->tm_min, date->tm_sec,
                    "Invalid writing in /var/log/Logfile.log");
        }
    }
    else
    {
        fprintf(file, "[%02d/%02d/%04d %02d:%02d:%02d] [Error] %s\n", date->tm_mday,
                date->tm_mon + 1, date->tm_year + 1900, date->tm_hour, date->tm_min, date->tm_sec,
                text_error);
    }
    fclose(file);
}
