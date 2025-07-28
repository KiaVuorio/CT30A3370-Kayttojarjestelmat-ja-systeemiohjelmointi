#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{ // Check for minimum arguments: program + search term
    if (argc < 2)
    {
        fprintf(stderr, "my-grep: searchterm [file ...]\n");
        return 1;
    }

    char *search = argv[1];
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if (argc == 2)
    {
        // No files given, read from standard input (stdin)
        while ((read = getline(&line, &len, stdin)) != -1)
        { // Check if the search term is in the current line (case sensitive)
            if (strstr(line, search))
            {
                fputs(line, stdout);
            }
        }
        free(line);
        return 0;
    }

    for (int i = 2; i < argc; i++)
    {
        fp = fopen(argv[i], "r");
        if (!fp)
        {
            fprintf(stderr, "my-grep: cannot open file\n");
            free(line);
            return 1;
        }

        // Read lines one by one from the file using getline
        while ((read = getline(&line, &len, fp)) != -1)
        {
            if (strstr(line, search))
            {
                fputs(line, stdout);
            }
        }

        fclose(fp);
    }

    free(line);
    return 0;
}