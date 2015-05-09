#include "pbmimage.h"
#include <stdlib.h>
#include <stdio.h>

PBMImage *imageFromPBMFile(const char *filename)
{
    char buff[16];
    PBMImage *img;
    FILE *fp;
    int c;
    
    //open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp)
    {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }
    
    //read image format
    if (!fgets(buff, sizeof(buff), fp))
    {
        perror(filename);
        exit(1);
    }
    
    //check the image format
    if (buff[0] != 'P' || buff[1] != '4')
    {
        fprintf(stderr, "Invalid image format (must be 'P4')\n");
        exit(1);
    }
    
    //alloc memory form image
    img = (PBMImage *)malloc(sizeof(PBMImage));
    if (!img)
    {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    //check for comments
    c = getc(fp);
    while (c == '#')
    {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }
    
    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->x, &img->y) != 2)
    {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
    }
    
    //memory allocation for pixel data
    img->pixel = (PBMPixel*)malloc(img->x * img->y * sizeof(PBMPixel));
    
    if (!img)
    {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    while (fgetc(fp) != '\n') ;
    
    //read pixel data from file
    int row,col,bit;
    for (row=0;row<img->y;row++)
    {
        for (col=0;col<img->x;col+=8)
        {
            c=fgetc(fp);
            if (feof(fp))
            {
                fprintf(stderr,"Error reading image '%s' - more data expected\n", filename);
                exit(1);
            }
            for (bit=0;bit<8;bit++)
            {
                if ((c&0x80) == 0)
                {
                    img->pixel[col+bit+(row*img->x)] = clear;
                }
                else
                {
                    img->pixel[col+bit+(row*img->x)] = set;
                }
                c <<= 1;
            }
        }
    }
    
    fclose(fp);
    return img;
}

