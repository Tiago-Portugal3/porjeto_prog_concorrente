#include "functions.h"
#include "image-lib.h"
#include "gd.h"

// Function to open files
FILE *AbreFicheiro(char *nome, char *mode)
{
	FILE *fp;
	fp = fopen(nome, mode);
	if (fp == NULL)
	{
		return (0);
	}
	return (fp);
}

void *thread_function(void *arg)
{
    long int c = 0;

 
    return (void *)c;
}