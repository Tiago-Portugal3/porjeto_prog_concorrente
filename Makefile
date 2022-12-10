all: serial-basic

serial-basic: serial-basic.c functions.h image-lib.c image-lib.h 
	gcc -Wall -pedantic serial-basic.c image-lib.c functions.c -g -o ap-paralelo-1 -lgd -lpthread

clean:
	rm serial-basic

clean-data:
	rm -fry ./Resize-dir ./Thumbnail-dir ./Watermark-dir
