#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>

#include "bmp.h"

int main (int argc, char *argv[])
{
    //ensure proper usage
    if (argc != 4)
    {
        fprintf (stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    // convert n to float
    double f = atof(argv[1]);
    if (f <= 0.0 || f > 100.0)
    {
        fprintf (stderr, "please choose a positve resizing factor of 100 or less\n");
        return 2;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];


  // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // if factor is 1.0....copy infile
    if (f == 1.0)
    {
        // write outfile's BITMAPFILEHEADER
        fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    
        // write outfile's BITMAPINFOHEADER
        fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
        
        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
    
            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
    
            // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    
        // close infile
        fclose(inptr);
    
        // close outfile
        fclose(outptr);
    
        // success
        return 0;
    }
    
    // for factors other than 1.0....
    
    // store original sizes
    LONG origWidth = bi.biWidth;
    LONG origHeight = abs(bi.biHeight);
    
   // modify resized header files
    bi.biWidth = (LONG)(bi.biWidth * f);
    bi.biHeight = (LONG)(bi.biHeight * f);
   
   /*
    printf ("original bfType is %c\n", bf.bfType);
    printf ("original bfSize is %d\n", bf.bfSize);
    printf ("original biSize is %d\n", bi.biSize);
    printf ("original biWidth is %d\n", origWidth);
    printf ("original biHeight is %d\n", origHeight);
    printf ("original padding is %d\n", padding);
    printf ("original biSizeImage is %d\n", bi.biSizeImage);
    */
    // determine padding for scanlines
    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // complete header modifications
    bi.biSizeImage = (DWORD)(((sizeof(RGBTRIPLE) * bi.biWidth) + newPadding) *abs(bi.biHeight));
    bf.bfSize = (DWORD)(bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
    /*
    printf ("new bfType is %c\n", bf.bfType);
    printf ("new bfSize is %d\n", bf.bfSize);
    printf ("new biSize is %d\n", bi.biSize);
    printf ("new biWidth is %d\n", bi.biWidth);
    printf ("new biHeight is %d\n", bi.biHeight);
    printf ("new padding is %d\n", newPadding);
    printf ("new biSizeImage is %d\n", bi.biSizeImage);
    */
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    //  initiate counter values for new height and widths...
    int h = 0, w = 0;
    
    // in the case of reductions....
    if (f < 1)
    {
        // iterate over infile's scanlines
        for (int i = 0; i < origHeight; i++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < origWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
        
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
                 // for reductions
                    if (w < bi.biWidth)
                    {
                        if ((int)(j * f) == w)
                        {
                            if ((int)(i * f) == h)
                            {
                                // write RGB triple to outfile
                                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                                w++;
                            }
                        }
                    }

            }
            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);
            
            if ((int)(i * f) == h)
            {
                // then add it back (to demonstrate how)
                for (int k = 0; k < newPadding; k++)
                {
                    fputc(0x00, outptr);
                }
                h++;
                w = 0;
            }
        }   
        // close infile
        fclose(inptr);
    
        // close outfile
        fclose(outptr);
    
        // success
        return 0;
    }
    
    // in the case of enlargements...
    if (f > 1)
    {    
        // initiate size variables that will help in repositioning fread for re-writes...
        long rowSize = ((sizeof(RGBTRIPLE) * origWidth) + padding);
        int headSize = (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
         h = 1;
        
        // iterate over scanlines
        for (int i = 0; i < abs(bi.biHeight); i++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < origWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
                
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // for enlargements
                while (w < bi.biWidth)
                {
                    if ((int)(w / f) == j)
                    {
                        // write RGB triple to outfile...
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                        w++;
                    }
                    else
                        break;
                }
            }
            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }
            
            // ...for vertical stretching.....
            if (h < f)
            {
                //  reposition for re-write...
                long pos = ((rowSize * (int)(i / f)) + headSize);
                fseek(inptr, pos, SEEK_SET);
            }
            h++;
            if (h > f)
                {
                    h = 1;
                }
            w = 0;
        }
       
        // close infile
        fclose(inptr);
    
        // close outfile
        fclose(outptr);
    
        // success
        return 0; 
    }
}