typedef enum { clear, set } PBMPixel;

typedef struct
{
    int x, y;
    PBMPixel * pixel;
} PBMImage;

PBMImage *imageFromPBMFile(const char *filename);
