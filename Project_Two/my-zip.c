#include <stdio.h>
#include <stdlib.h>

// Compress file using run-length encoding (RLE)
// Format: write a 4-byte integer for count + 1 byte for character
void compress_file(FILE *fp)
{
    int c, prev = EOF;
    unsigned int count = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c != prev)
        {
            // Write out previous run if exists
            if (prev != EOF)
            {
                fwrite(&count, sizeof(count), 1, stdout);
                fputc(prev, stdout);
            }
            prev = c;
            count = 1;

            continue;
        }

        count++;
        // If count reaches max unsigned int, flush the run
        if (count == 0xFFFFFFFF)
        {
            fwrite(&count, sizeof(count), 1, stdout);
            fputc(prev, stdout);
            count = 0;
        }
    }

    // Write out last run if exists
    if (prev != EOF)
    {
        fwrite(&count, sizeof(count), 1, stdout);
        fputc(prev, stdout);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "my-zip: file1 [file2 ...]\n");
        return 1;
    }
    // Process all files in order
    for (int i = 1; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "rb");
        if (!fp)
        {
            fprintf(stderr, "my-zip: cannot open file\n");
            return 1;
        }
        compress_file(fp);
        fclose(fp);
    }

    return 0;
}