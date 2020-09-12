#include "bmp.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <aio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

void bmp_wait_all() {}

void cleanup(union sigval sig) {}

void image_write_aio(image_struct *is, int iter) {}

void image_write(image_struct *is, int iter) {
    int width           = is -> width;
    int height          = is -> height;
    unsigned char *file = is -> file;

    int padding, filesize, datasize;
    char filename[50];
    FILE *f;

    padding  = (4 - (width * 3) % 4) % 4;
    datasize = (3 * width + padding) * height;
    filesize = 54 + datasize;

    sprintf(filename, "out_%04d.bmp", iter);
    f = fopen(filename, "w");
    fwrite(file, 1, filesize, f);
    fclose(f);
}

// returns a pointer to the start of the actual image
// Mdifies the passed in pointer to be the start of the new
// memory
image_struct *image_setup(int width, int height) {
    int w = width;
    int h = height;
    int padding, filesize, datasize;
    image_struct *ret = malloc(sizeof(image_struct));

    unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0,
                                        0, 0, 0, 54, 0, 0, 0 };
    unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0,
                                        0, 0, 0, 0, 1, 0, 24, 0 };

    padding = (4 - (width * 3) % 4) % 4;

    datasize = (3 * width + padding) * height;
    filesize = 54 + datasize;

    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4]  = (unsigned char)(w);
    bmpinfoheader[5]  = (unsigned char)(w >> 8);
    bmpinfoheader[6]  = (unsigned char)(w >> 16);
    bmpinfoheader[7]  = (unsigned char)(w >> 24);
    bmpinfoheader[8]  = (unsigned char)(h);
    bmpinfoheader[9]  = (unsigned char)(h >> 8);
    bmpinfoheader[10] = (unsigned char)(h >> 16);
    bmpinfoheader[11] = (unsigned char)(h >> 24);
    bmpinfoheader[20] = (unsigned char)(datasize);
    bmpinfoheader[21] = (unsigned char)(datasize >> 8);
    bmpinfoheader[22] = (unsigned char)(datasize >> 16);
    bmpinfoheader[23] = (unsigned char)(datasize >> 24);

    ret->file = (unsigned char *)calloc(filesize, sizeof(unsigned char));
    memcpy(ret->file, &bmpfileheader, sizeof(unsigned char) * 14);
    memcpy((ret->file) + 14, &bmpinfoheader, sizeof(unsigned char) * 40);
    ret->image = malloc(sizeof(unsigned char *) * height);

    int i;
    for (i = 0; i < height; i++) {
        ret->image[i] = &ret->file[i * (3 * width + padding) + 54];
    }
    
    ret->width  = width;
    ret->height = height;

    return ret;
}

void image_destroy(image_struct *is) {
    free(is -> file);
    free(is -> image);
    free(is);
}

void update_image(image_struct *is, double v, int x, int y) {
    unsigned char **img = is -> image;
    int height          = is->height;

    double r, g, b;
    y = (height - 1) - y;
    v = ((v + 1.) / (2.));

    r = v * 255; // Red channel
    g = v * 255; // Green channel
    b = v * 255; // Red channel

    r = min(r, 255);
    g = min(g, 255);
    b = min(b, 255);

    img[y][3 * x]     = (unsigned char)(r);
    img[y][3 * x + 1] = (unsigned char)(g);
    img[y][3 * x + 2] = (unsigned char)(b);
}

int write_to_file(double **tmptab, int startx, int starty, int width,
                  int height, int iter) {
    int x, y;

    image_struct *is = image_setup(width, height);

    int actual = 0;
    for (y = starty; y < starty + height; y++) {
        for (x = startx; x < startx + width; x++, actual++) {
            if ((x == 0 || x == (startx + width - 1) || y == 0 ||
               y == (starty + height - 1)) && tmptab[y][x] != 0)
                printf("%lf\n, %d, %d\n", tmptab[y][x], x, y);
                update_image(is, tmptab[y][x], x, y);
        }
    }

    image_write(is, iter);
    image_destroy(is);

    return 0;
}
