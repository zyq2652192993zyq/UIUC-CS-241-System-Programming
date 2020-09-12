typedef struct _image_struct {
	unsigned char *file;
	unsigned char **image;
	int height;
	int width;
} image_struct;

// Waits for all writes initiated by image_write_aio to be completed
//
// THIS IS NOT IMPLEMENTED FOR YOU AND IS NOT REQUIRED
void bmp_wait_all();

// Takes the size of width and height of the square image
// to write, an array which represents that image and the
// iteration you are on. It will use asynchronous i/o to
// write the image to a file. If you use this function
// you must wait for all the writes to finish. bmp_wait_all()
// is one way to do this.
//
// THIS IS NOT IMPLEMENTED FOR YOU AND IS NOT REQUIRED
void image_write_aio(image_struct *is, int iter);

// This function is identical to image_write_aio except it
// will block program execution while it writes to the file.
void image_write(image_struct *is, int iter);

// This updates a single pixel of an image. It exposes a
// specific part of the writing to file which so as to better
// exploit the parallelism of even the write portion of the
// program. v is the value you would like to add to the image
// x and y are the coordinates of that value and n is the
// size of the image.
void update_image(image_struct *is, double v, int x, int y);

// This sets up an image of the desired size which you can
// then use in all the other bmp functions. The unsigned char *
// returned is the start of the actual image data and should be
// used when working with the image, in update_image for example.
// The unsigned unsigned char *, *file, will be modified to point
// to the beginning of all the allocated memory for the image,
// including the bmp header and should be used with image_write
// and image_write_aio.
image_struct *image_setup(int width, int height);

void image_destroy(image_struct *is);

// This combines image_setup, update_image and image_write calls
// into a single function. This can be used in a parallel
// implementation but would make your program less parallel.
// This call is still possibly less efficient for a serial
// implementation but is perfectly acceptable for it.
int write_to_file(double **tmptab, int startx, int starty, int width,
                  int height, int iter);
