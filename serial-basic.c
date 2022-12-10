/******************************************************************************
 * Programacao Concorrente
 * MEEC 21/22
 *
 * Projecto - Parte1
 *                           serial-complexo.c
 *
 * Compilacao: gcc serial-complexo -o serial-complex -lgd
 *
 *****************************************************************************/
#include "functions.h"
#include <pthread.h>
#include "image-lib.h"

/******************************************************************************
 * main()
 *
 * Arguments: (none)
 * Returns: 0 in case of sucess, positive number in case of failure
 * Side-Effects: creates thumbnail, resized copy and watermarked copies
 *               of images
 *
 * Description: implementation of the complex serial version
 *              This application only works for a fixed pre-defined set of files
 *
 *****************************************************************************/

int main(int argc, char *argv[])
{
	char pal[100], nome_fData[100], nome_dir[100];
	int filesCount = 0, i = 0, aux = 0, realdiv, restdiv;
	FILE *fData;
	int n_threads = 0;
	struct data *ptr_data;

	/*verification that the number of arguments is correct and the n_threads is positive*/
	if (argc != 3 || atoi(argv[2]) < 0)
	{
		return 0;
	}

	// read the arguments
	strcpy(nome_dir, argv[1]);

	strcpy(nome_fData, strcat(nome_dir, "/image-list.txt"));

	n_threads = atoi(argv[2]);

	pthread_t thread_id[n_threads];

	// printf("%s\n %d", nome_fData, n_threads);

	fData = AbreFicheiro(nome_fData, "r");

	while (fData != NULL)
	{
		if (fscanf(fData, "%s", pal) != 1)
		{
			break;
		}

		filesCount++;
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

		array[i] = (char *)malloc((strlen(pal) + 1) * sizeof(char));
		if (array[i] == NULL)
		{
			printf("Failed to allocate memory");
			return 0;
		}
		strcpy(array[i], pal);
		i++;
	}

	// test to see the words
	for (size_t i = 0; i < filesCount; i++)
	{
		printf("%s\n", array[i]);
	}

	// memory allocation of the struct array
	ptr_data = (struct data *)malloc(n_threads * sizeof(struct data));
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

	watermark_img = read_png_file("watermark.png");
	if (watermark_img == NULL)
	{
		fprintf(stderr, "Impossible to read %s image\n", "watermark.png");
		exit(-1);
	}
	// division of the files, store in the structs
	realdiv = filesCount / n_threads;
	restdiv = filesCount % n_threads;
	

	//initialize the structs
	for (i = 0; i < n_threads; i++)
	{
		ptr_data[i].ind_inicial =0;
		ptr_data[i].ind_final=0;
	}
	
   //define which images go to each thread
	for (i = 0; i < n_threads; i++)
	{
		ptr_data[i].ind_inicial += aux;
		ptr_data[i].ind_final += (aux + (realdiv - 1));
		aux += realdiv;
	}
	// maneira melhor para dividir as imagens que sobram?
	ptr_data[n_threads - 1].ind_final += (restdiv);

	for (i = 0; i < n_threads; i++)
	{
		printf("ptr_data_inicial:  trhread->%d = %d\n", i, ptr_data[i].ind_inicial);
		printf("ptr_data_final: trhread->%d = %d\n", i, ptr_data[i].ind_final);
	}

	// thread creation
	for (i = 0; i < n_threads; i++)
	{
		pthread_create(&thread_id[i], NULL, thread_function, NULL);
	}

	// phtread join
for ( i = 0; i < n_threads; i++)
{
	pthread_join(&thread_id[i],NULL);
}

	// free memory allocation(free structs inside threads)

	exit(0);
}
