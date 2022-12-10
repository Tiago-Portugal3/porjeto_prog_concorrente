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

/* input images */
gdImagePtr in_img, watermark_img;
/* output images */
gdImagePtr out_thumb_img, out_resized_img, out_watermark_img;
/* file name of the image created and to be saved on disk	 */
char out_file_name[100], **array;
 
struct data
{
    int nn_files,ind_inicial,ind_final;
    //informação sobre os indices a serem usados(1º e último?);
};

FILE *AbreFicheiro(char *nome, char *mode);

void *thread_function(void *arg);
#endif