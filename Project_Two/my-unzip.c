#include <stdio.h>
#include <stdlib.h>

// Decompress file encoded with run-length encoding (RLE)
void decompress_file(FILE *fp)
{
    unsigned int count;

    // Read runs until fread fails (EOF)
    while (fread(&count, sizeof(count), 1, fp) == 1)
    {
        const int c = fgetc(fp);
        if (c == EOF)
        {
            fprintf(stderr, "my-unzip: invalid input file\n");
            exit(1);
        }
        // Print character count times
        for (unsigned int i = 0; i < count; i++)
        {
            fputc(c, stdout);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n");
        return 1;
    }

    // Decompress each file sequentially
    for (int i = 1; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "rb");
        if (!fp)
        {
            fprintf(stderr, "my-unzip: cannot open file\n");
            return 1;
        }
        decompress_file(fp);
        fclose(fp);
    }

    return 0;
}