#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper use
    if (argc != 2)
    {
        fprintf(stderr,  "Usage: %s forensic_image.raw\n", argv[0]);
        return 1;
    }

    char *infile = argv[1];

    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Couldn't open: %s.\n", infile);
        return 2;
    }

    // A fat system has blocks of 512Bytes
    unsigned char *block_data = malloc(512);

    int files_founded = 0;
    FILE *outptr = NULL;

    // While i can read an entire block of data
    while (fread(block_data, 512, 1, inptr))
    {
        // Check magic number
        if (block_data[0] == 0xff  && block_data[1] == 0xd8 && block_data[2] == 0xff && (block_data[3] & 0xe0))
        {

            char filename[8];
            sprintf(filename, "%03d.jpg", files_founded);
            files_founded++;
            // Check if there is a file open
            if (outptr != NULL)
            {
                fclose(outptr);
            }

            outptr = fopen(filename, "w");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", filename);
                return 3;
            }

        }

        if (outptr != NULL)
        {
            fwrite(block_data, 512, 1, outptr);
        }
    }

    // free memory
    free(block_data);

    // Close open file
    if (outptr != NULL)
    {
        fclose(outptr);
    }

    // Close infile
    fclose(inptr);

    // Success
    return 0;
}
