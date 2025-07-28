#include <stdio.h>
#include <stdlib.h>

// Helper function to print contents of a file pointer to stdout
void print_file(FILE *fp)
{
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        fputs(buffer, stdout);
    }
}

int main(int argc, char *argv[])
{
    // If no files specified, just exit with success (return 0)
    if (argc == 1)
    {
        return 0;
    }

    for (int i = 1; i < argc; i++)
    { // Open the file in read mode
        FILE *fp = fopen(argv[i], "r");
        if (!fp)
        { // If fopen fails, print error message and exit with failure
            fprintf(stderr, "my-cat: cannot open file\n");
            return 1;
        }
        print_file(fp);
        fclose(fp);
    }

    return 0;
}