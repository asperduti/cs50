// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize n infile outfile\n");
        return 1;
    }

    // remember args
    double resize_factor = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    if ((resize_factor <= 0.0) || (resize_factor > 100.0))
    {
        fprintf(stderr, "n must be a possitive integer less or equal than 100.0.( 0.0 < n <= 100.0)");
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w+");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
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
        return 4;
    }

    // Update Bitmap Headers
    BITMAPFILEHEADER bf_out;
    memcpy(&bf_out, &bf, sizeof(BITMAPFILEHEADER));

    BITMAPINFOHEADER bi_out;
    memcpy(&bi_out, &bi, sizeof(BITMAPINFOHEADER));
    // sizeof_scanline = bi_out.biWidth * 3(bytes per pixes) + padding
    // bfSize = (sizeof(BITMAPFILEHEADER)) + (sizeof(BITMAPINFOHEADER)) + sizeof_scanline * bi_out.biHeight
    bi_out.biHeight = -1 * floor(abs(bi_out.biHeight) * resize_factor);
    bi_out.biWidth = floor(bi_out.biWidth * resize_factor);

    // determine padding for scanlines
    int infile_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outfile_padding = (4 - (bi_out.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    int sizeof_scanline = bi_out.biWidth * sizeof(RGBTRIPLE) + outfile_padding;
    bf_out.bfSize = (sizeof(BITMAPFILEHEADER)) + (sizeof(BITMAPINFOHEADER)) + sizeof_scanline * abs(bi_out.biHeight);
    bi_out.biSizeImage = sizeof_scanline * abs(bi_out.biHeight);
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_out, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);

    // calculate aspect ratios
    double width_ratio = (double) bi.biWidth / bi_out.biWidth;
    double height_ratio = (double) bi.biHeight / bi_out.biHeight;

    // temporary scanline
    RGBTRIPLE scanline[bi.biWidth];

    // saved row
    int saved_row = -1;

    // iterate over outfile's scanlines
    for (int i = 0, biHeight = abs(bi_out.biHeight); i < biHeight; i++)
    {
        // Lineal mapping beetwean rows in infile and outfile
        int row_oldfile = i * width_ratio;

        // read and save the need it row
        if (saved_row != row_oldfile)
        {
            fseek(inptr, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (((sizeof(RGBTRIPLE) * bi.biWidth) + infile_padding) * row_oldfile), SEEK_SET);
            fread(&scanline, sizeof(RGBTRIPLE) * bi.biWidth, 1, inptr);
            saved_row = row_oldfile;
        }

        // iterate over pixels in outfile's scanline
        for (int j = 0; j < bi_out.biWidth; j++)
        {
            // Lineal mapping beetwean colummns in infile and outfile
            int column_oldfile = j * width_ratio;
            fwrite(&scanline[column_oldfile], sizeof(RGBTRIPLE), 1, outptr);
        }


        // then add it back (to demonstrate how)
        for (int k = 0; k < outfile_padding; k++)
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
