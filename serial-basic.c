/******************************************************************************
 * Programacao Concorrente
 * MEEC 21/22
 *
 * Projecto - ParteA_pararel-1
 *
 * Tiago Portugal      Aluno: 100092
 * Francisco Oliveira  Aluno: 99939
 *****************************************************************************/
#include "functions.h"
#include <pthread.h>
#include "image-lib.h"

int main(int argc, char *argv[])
{
	char pal[50], nome_fData[50], aux_2[50], ch = '.', ret[50], *extin = ".png";
	int filesCount = 0, i = 0, aux = 0, realdiv, restdiv;
	FILE *fData;
	int n_threads = 0;
	data *ptr_data;

	/*verification that the number of arguments is correct and the n_threads is positive*/
	if (argc != 3 || atoi(argv[2]) < 1)
	{
		printf("Valor de argumentos/trheads inválido");
		return 0;
	}

	// read the argument
	strcpy(aux_2, argv[1]);
	// Strored in 1 global var, to be used in thread functions
	strcpy(nome_dir, aux_2);
	strcpy(nome_fData, strcat(aux_2, "/image-list.txt"));

	n_threads = atoi(argv[2]);
	pthread_t thread_id[n_threads];

	fData = AbreFicheiro(nome_fData, "r");
	if (fData == NULL)
		return 0;

	// Count the number of words
	while (fData != NULL)
	{
		if (fscanf(fData, "%s", pal) != 1)
		{
			break;
		}
		strcpy(aux_2, pal);
		strcpy(ret, strrchr(aux_2, ch));
		printf("%s\n", ret);
		printf("%s\n", extin);
		if (strcmp(ret, extin) == 0)
		{
			filesCount++;
		}
	}

	// memory allocation of array
	array = (char **)malloc(filesCount * sizeof(char *));
	if (array == NULL)
	{
		printf("Failed to allocate memory");
		return 0;
	}
	rewind(fData);

	// read and store the name of the images
	while (fData != NULL)
	{
		if (fscanf(fData, "%s", pal) != 1)
		{
			break;
		}
		// checks that the problem file has a .pals extension
		strcpy(aux_2, pal);
		strcpy(ret, strrchr(aux_2, ch));
		printf("%s\n", ret);
		printf("%s\n", extin);
		if (strcmp(ret, extin) != 0)
		{
			printf("%s não tem o formato .png\n", pal);
		}
		else
		{
			array[i] = (char *)malloc((strlen(pal) + 1) * sizeof(char));
			if (array[i] == NULL)
			{
				printf("Failed to allocate memory");
				return 0;
			}
			strcpy(array[i], pal);
			printf("%s\n", pal);
			i++;
		}
	}

	// memory allocation of the struct array
	ptr_data = (data *)malloc(n_threads * sizeof(data));
	if (ptr_data == NULL)
	{
		printf("Failed memory allocation");
		return 0;
	}

	/* creation of output directories */
	if (create_directory(RESIZE_DIR) == 0)
	{
		fprintf(stderr, "Impossible to create %s directory\n", RESIZE_DIR);
		exit(-1);
	}
	if (create_directory(THUMB_DIR) == 0)
	{
		fprintf(stderr, "Impossible to create %s directory\n", THUMB_DIR);
		exit(-1);
	}
	if (create_directory(WATER_DIR) == 0)
	{
		fprintf(stderr, "Impossible to create %s directory\n", WATER_DIR);
		exit(-1);
	}
	printf("\n");

	// division of the files, store in the structs
	realdiv = filesCount / n_threads;
	restdiv = filesCount % n_threads;

	// define which images go to each thread
	if (n_threads < filesCount)
	{

		for (i = 0; i < n_threads; i++)
		{
			ptr_data[i].ind_inicial = aux;
			if (restdiv > 0)
			{
				ptr_data[i].ind_final = (aux + (realdiv));
				aux += (realdiv + 1);
				restdiv--;
			}
			else
			{
				ptr_data[i].ind_final = (aux + (realdiv - 1));
				aux += realdiv;
			}
		}
	}
	else
	{
		for (i = 0; i < filesCount; i++)
		{
			ptr_data[i].ind_inicial = i;
			ptr_data[i].ind_final = i;
		}
	}

	// thread creation
	for (i = 0; i < n_threads; i++)
	{
		pthread_create(&thread_id[i], NULL, thread_function, &ptr_data[i]);
	}

	// phtread join
	for (i = 0; i < n_threads; i++)
	{
		pthread_join(thread_id[i], NULL);
	}

	// free memory allocation(free structs inside threads)
	fclose(fData);

	for (i = 0; i < filesCount; i++)
	{
		free(array[i]);
	}

	free(ptr_data);
	exit(0);
}
