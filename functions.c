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
		printf("image-list.txt nof found in this directory");
		return (NULL);
	}
	return (fp);
}

// tHread funciton that do the watermark/resize/thumbnail
void *thread_function(void *ptr)
{
	/* input images */
	gdImagePtr in_img, watermark_img;
	/* output images */
	gdImagePtr out_thumb_img, out_resized_img, out_watermark_img;
	/* file name of the image created and to be saved on disk	 */
	long int c = 0;
	data *pass_arg = (data *)ptr;
	char *name_resize, *name_water, *name_thumb, name_file[20], *name_read_file, nome_dir_thread[50];

	// if the thread has no images assigned skip
	if (pass_arg->ind_inicial != -1)
	{
		// cycle to make the transformations to all the images assigned to the thread
		for (int i = pass_arg->ind_inicial; i <= pass_arg->ind_final; i++)
		{

			// concatenate the directories name and the image name
			strcpy(nome_dir_thread, nome_dir);
			strcat(nome_dir_thread, "/");
			strcpy(name_file, array[i]);
			name_read_file = (char *)malloc(strlen(name_file) + strlen(nome_dir_thread) + 1);
			strcpy(name_read_file, strcat(nome_dir_thread, name_file));

			name_resize = (char *)malloc(strlen("serial-Resize/") + strlen(name_file) + 1);
			name_water = (char *)malloc(strlen("serial-Watermark/") + strlen(name_file) + 1);
			name_thumb = (char *)malloc(strlen("serial-Thumbnail/") + strlen(name_file) + 1);

			strcpy(name_water, "serial-Watermark/");
			strcat(name_water, name_file);
			strcpy(name_resize, "serial-Resize/");
			strcat(name_resize, name_file);
			strcpy(name_thumb, "serial-Thumbnail/");
			strcat(name_thumb, name_file);

			watermark_img = read_png_file("watermark.png");
			if (watermark_img == NULL)
			{
				fprintf(stderr, "Impossible to read %s image\n", "watermark.png");
				exit(-1);
			}

			// see if the output image already exists
			if (access(name_water, F_OK) != -1)
			{
				printf("%s encontrado\n", name_water);
			}
			else
			{
				/*
				 * Add the watermarks
				 */
				/* load of the input file */
				in_img = read_png_file(name_read_file);
				if (in_img == NULL)
				{
					fprintf(stderr, "Impossible to read %s image\n", array[i]);
					continue;
				}

				/* add watermark */
				out_watermark_img = add_watermark(in_img, watermark_img);
				if (out_watermark_img == NULL)
				{
					fprintf(stderr, "Impossible to creat thumbnail of %s image\n", array[i]);
				}
				else
				{
					/* save watermark */
					sprintf(out_file_name, "%s%s", WATER_DIR, array[i]);
					if (write_png_file(out_watermark_img, out_file_name) == 0)
					{
						fprintf(stderr, "Impossible to write %s image\n", out_file_name);
					}
					printf("watermark  %s\n", array[i]);
					gdImageDestroy(out_watermark_img);
				}
				gdImageDestroy(in_img);
			}

			// see if the output image already exists
			if (access(name_resize, F_OK) == 0)
			{
				printf("%s encontrado\n", name_resize);
			}
			else
			{
				/*
				 * To resize images and add watermark
				 */
				in_img = read_png_file(name_read_file);
				/* load of the input file */
				if (in_img == NULL)
				{
					fprintf(stderr, "Impossible to read %s image\n", array[i]);
					continue;
				}
				/* resizes of each image */
				out_watermark_img = add_watermark(in_img, watermark_img);
				if (out_watermark_img == NULL)
				{
					fprintf(stderr, "Impossible to add watermark to %s image\n", array[i]);
				}
				else
				{
					out_resized_img = resize_image(out_watermark_img, 800);
					if (out_resized_img == NULL)
					{
						fprintf(stderr, "Impossible to resize %s image\n", array[i]);
					}
					else
					{
						/* save resized */
						sprintf(out_file_name, "%s%s", RESIZE_DIR, array[i]);
						if (write_png_file(out_resized_img, out_file_name) == 0)
						{
							fprintf(stderr, "Impossible to write %s image\n", out_file_name);
						}
						gdImageDestroy(out_resized_img);
						printf("resize %s\n", array[i]);
					}
					gdImageDestroy(out_watermark_img);
				}
				gdImageDestroy(in_img);
			}

			// see if the output image already exists
			if (access(name_thumb, F_OK) != -1)
			{
				printf("%s encontrado\n", name_thumb);
			}
			else
			{
				/*
				 * Add watermark and create thumbnails
				 */
				/* load of the input file */
				in_img = read_png_file(name_read_file);
				if (in_img == NULL)
				{
					fprintf(stderr, "Impossible to read %s image\n", array[i]);
					continue;
				}

				/* creation of thumbnail image */
				out_watermark_img = add_watermark(in_img, watermark_img);
				if (out_watermark_img == NULL)
				{
					fprintf(stderr, "Impossible to creat thumbnail of %s image\n", array[i]);
				}
				else
				{
					out_thumb_img = make_thumb(out_watermark_img, 150);
					if (out_thumb_img == NULL)
					{
						fprintf(stderr, "Impossible to creat thumbnail of %s image\n", array[i]);
					}
					else
					{
						/* save thumbnail image */
						sprintf(out_file_name, "%s%s", THUMB_DIR, array[i]);
						if (write_png_file(out_thumb_img, out_file_name) == 0)
						{
							fprintf(stderr, "Impossible to write %s image\n", out_file_name);
						}
						printf("thumbnail %s\n", array[i]);
						gdImageDestroy(out_thumb_img);
					}
					gdImageDestroy(out_watermark_img);
				}
				gdImageDestroy(in_img);
			}
			free(name_water);
			free(name_resize);
			free(name_thumb);
			free(name_read_file);
			gdImageDestroy(watermark_img);
		}
	}

	return (void *)c;
}