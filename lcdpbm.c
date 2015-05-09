//======================
//
// Display Properties
//
#define DISPLAY_WIDTH 192
#define DISPLAY_HEIGHT 64

#include "ks0108fb.h"
#include "pbmimage.h"
#include <stdlib.h>

char * filename = NULL;
int offset_x = 0;
int offset_y = 0;
int grid = 0;
int clear_request = 0;

void writePBMImage(FrameBuffer * fb, PBMImage * img, int offset_x, int offset_y)
{
    int x,y;
    for (y=0; y<img->y;y++)
    {
        for (x=0; x<img->x;x++)
        {
            if ((x+offset_x>=0) && (x+offset_x<fb->x) && (y+offset_y>=0) && (y+offset_y<fb->y))
            {
                if (img->pixel[(y*img->x)+x] == set)
                {
                    fb->pixel[((y+offset_y)*fb->x)+x+offset_x] = 255;
                }
                else
                {
                    fb->pixel[((y+offset_y)*fb->x)+x+offset_x] = 0;
                }
            }
        }
    }
    
    return;
}


void printUsage(int argc, const char * argv[])
{
    printf( "USAGES: %s [options] [filename]\n", argv[0] );
    printf( "Reads a monochrome bitmap from file and sends it to a graphic display.\n" );
    printf( "Supported image types: PBM raw ('P4')\n" );
    printf( "  -h          This help message\n" );
    printf( "  -c          Clear display before writing data\n" );
    printf( "  -g w        Write out a grid with specific width 'w'\n" );
    printf( "  -o x y      Display image with an offset of (x,y)\n");
}

int main(int argc, const char * argv[])
{
    if (argc < 2)
    {
        printUsage(argc, argv);
        exit(1);
    }

    if (processArguments(argc, argv) == 0)
    {
        FrameBuffer * fb = FBcreate(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if (!fb)
        {
            fprintf(stderr,"couldn't create framebuffer\n");
            exit( 1);
        }
     
        if (grid > 0)
        {
            FBgrid(fb, grid);
            FBwrite(fb);
            exit(0);
        }
        
        if (clear_request > 0)
        {
    	    FBwrite(fb);
        }
        
        //write out image
        PBMImage * img = imageFromPBMFile(filename);
        writePBMImage(fb, img, offset_x, offset_y);
        
        //write out rect of affected buffer
        int x = offset_x;
        int y = offset_y;
        int width = img->x;
        int height = img->y;
        
        if (x<0)
    	    x = 0;
    	if (y<0)
    	    y = 0;
    	if (offset_x + img->x > fb->x)
    	    width = fb->x - offset_x;
    	if (offset_y + img->y > fb->y)
    	    height = fb->y - offset_y;

        FBupdateRect(fb, x, y, width, height);
    }
    else
        return 1;

    return 0;
}

/**
 * Process and set up command line arguments
 */
int processArguments(int argc, const char * argv[] )
{
    int i;
    for( i = 1; i < argc; ++i )
    {
        // Handle command line switches
        if (argv[i][0] == '-')
        {
            // Which switch was given
            switch (argv[i][1])
            {
                    // Help
                case '?':
                case 'h':
                    printUsage( argc, argv );
                    exit(0);
                    break;
                    
                case 'c':
            	    clear_request = 1;
                    break;
                    
                    // Specify offset
                case 'o':
                    if( i+2 < argc )
                    {
                        if( (sscanf(argv[i+1], "%i", &offset_x) == EOF) || (sscanf(argv[i+2], "%i", &offset_y) == EOF) )
                        {
                            fprintf(stderr,"Invalid offset x:%s and y:%s \n", argv[i+1], argv[i+2]);
                            return 11;
                        }
                        i+=2; // Account for "follow on" arguments
                    }
                    else
                    {
                        fprintf(stderr,"Not enough arguments given with 'o' flag.\n" );
                        return (int)'o';
                    }
                    break;
                    
                    // a test flag for the lcd
                case 'g':
                    if( i+1 < argc )
                    {
                        if (sscanf(argv[i+1], "%i", &grid) == EOF)
                        {
                            fprintf(stderr,"Invalid grid width:%s \n", argv[i+1]);
                            return 1;
                        }
                        i+=1; // Account for "follow on" arguments
                    }
                    else
                    {
                        fprintf(stderr,"Not enough arguments given with 'g' flag.\n" );
                        return (int)'g';
                    }
                    break;
                default:
            	    printf("unknown option %s\n", argv[i]);
            	    break;

            }	// end switch: flag value
        }	// end if: '-'
        // Else assume it's a filename
        else
        {
            if (filename == NULL)
            {
                filename = (char*)argv[i];
            }
            else
            {
                fprintf(stderr,"unknown option: %s - filename already set as: %s\n", argv[i], filename );
                return 1;
            }
        }
    }	// end for: each command line argument
    
    // Make sure we have a filename
    if(( filename == NULL ) && (grid == 0))
    {
        fprintf(stderr, "No suitable filename could be determined.\n" );
        return 1;
    }

    return 0;
}

