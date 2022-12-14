all: serial-basic

serial-basic: serial-basic.c functions.h image-lib.c image-lib.h 
	gcc -Wall -pedantic serial-basic.c image-lib.c functions.c -g -o ap-paralelo-1 -lgd -lpthread

clean:
	rm ap-paralelo-1

clean-data:
	rm -fr ./serial-Resize ./serial-Thumbnail ./serial-Watermark
