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

void *thread_function(void *ptr)
{
	long int c = 0;
	data *pass_arg = (data *)ptr;
	/*
	printf("thread numero = %d\n", pass_arg->number);
	printf("ind_inicial = %d\n", pass_arg->ind_inicial);
	printf("ind_final = %d\n", pass_arg->ind_final);
	*/
	/*
	 * To resize images and add watermark
	 */
	if (pass_arg->ind_inicial != -1)
	{
		for (int i = pass_arg->ind_inicial; i <= pass_arg->ind_final; i++)
		{
			/*
			 * Add the watermarks
			 */
			printf("watermark  %s\n", array[i]);
			/* load of the input file */
			in_img = read_png_file(array[i]);
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
				gdImageDestroy(out_watermark_img);
			}
			gdImageDestroy(in_img);

			/*
			 * To resize images and add watermark
			 */
			in_img = read_png_file(array[i]);
			printf("resize %s\n", array[i]);
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
					printf("1\n");
				}
				gdImageDestroy(out_watermark_img);
				printf("2\n");
			}
			gdImageDestroy(in_img);
			printf("3\n");

			/*
			 * Add watermark and create thumbnails
			 */
			printf("thumbnail %s\n", array[i]);
			/* load of the input file */
			in_img = read_png_file(array[i]);
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
					gdImageDestroy(out_thumb_img);
				}
				gdImageDestroy(out_watermark_img);
			}
			gdImageDestroy(in_img);

			gdImageDestroy(watermark_img);
		}
	}

	return (void *)c;
}