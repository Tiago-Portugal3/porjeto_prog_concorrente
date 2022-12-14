#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "image-lib.h"
/* the directories wher output files will be placed */
#define RESIZE_DIR "./serial-Resize/"
#define THUMB_DIR "./serial-Thumbnail/"
#define WATER_DIR "./serial-Watermark/"

char out_file_name[100], **array,nome_dir[50];

typedef struct Data
{
    int number;
    int ind_inicial ;
    int ind_final ;
    // informação sobre os indices a serem usados(1º e último?);
} data;

FILE *AbreFicheiro(char *nome, char *mode);

void *thread_function(void *arg);
#endif