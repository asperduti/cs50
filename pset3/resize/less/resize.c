// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int resize_factor = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    if ((resize_factor <= 0) || (resize_factor > 100))
    {
        fprintf(stderr, "n must be a possitive integer less or equal than 100.( 0 < n <= 100)");
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
    bi_out.biHeight *= resize_factor;
    bi_out.biWidth *= resize_factor;

    // determine padding for scanlines
    int infile_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outfile_padding = (4 - (bi.biWidth * resize_factor * sizeof(RGBTRIPLE)) % 4) % 4;

    int sizeof_scanline = bi_out.biWidth * sizeof(RGBTRIPLE) + outfile_padding;
    bf_out.bfSize = (sizeof(BITMAPFILEHEADER)) + (sizeof(BITMAPINFOHEADER)) + sizeof_scanline * abs(bi_out.biHeight);
    bi_out.biSizeImage = sizeof_scanline * abs(bi_out.biHeight);
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_out, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);


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
            for (int k = 0; k < resize_factor; k++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

        }
        // skip over padding, if any
        fseek(inptr, infile_padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < outfile_padding; k++)
        {
            fputc(0x00, outptr);
        }

        // copy the scanline resize_factor times
        fseek(outptr, -(sizeof(RGBTRIPLE) * bi_out.biWidth + outfile_padding), SEEK_CUR);

        char *tmp_scanline = malloc(sizeof_scanline);
        fread(tmp_scanline, sizeof_scanline, 1, outptr);


        for (int k = 0; k < resize_factor - 1; k++)
        {
            fwrite(tmp_scanline, sizeof_scanline, 1, outptr);
        }
        free(tmp_scanline);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
