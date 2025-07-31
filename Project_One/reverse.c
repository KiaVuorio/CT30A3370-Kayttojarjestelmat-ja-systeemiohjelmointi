#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a node for a singly linked list to store lines of input
typedef struct Node
{
    char *line;
    struct Node *next;
} Node;

// Push a new line to the front of the linked list
Node *push(Node *head, char *line)
{
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    new_node->line = line;
    new_node->next = head;
    return new_node;
}

// Free all memory associated with the linked list
void free_list(Node *head)
{
    while (head != NULL)
    {
        Node *tmp = head;
        head = head->next;
        free(tmp->line);
        free(tmp);
    }
}

int main(int argc, char *argv[])
{
    FILE *input = stdin;
    FILE *output = stdout;

    // Handle too many arguments
    if (argc > 3)
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // Handle input file if provided
    if (argc >= 2)
    {
        input = fopen(argv[1], "r");
        if (input == NULL)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    // Handle output file if provided
    if (argc == 3)
    {
        if (strcmp(argv[1], argv[2]) == 0)
        {
            fprintf(stderr, "Input and output file must differ\n");
            if (input != stdin)
                fclose(input);
            exit(1);
        }
        output = fopen(argv[2], "w");
        if (output == NULL)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            if (input != stdin)
                fclose(input);
            exit(1);
        }
    }

    char *line = nullptr;
    size_t len = 0;
    ssize_t read;
    Node *head = nullptr;

    // Read all lines and push them to the linked list (in reverse order)
    while ((read = getline(&line, &len, input)) != -1)
    {
        char *stored_line = malloc(read + 1);
        if (stored_line == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            free(line);
            free_list(head);
            if (input != stdin)
                fclose(input);
            if (output != stdout)
                fclose(output);
            exit(1);
        }
        strcpy(stored_line, line);
        head = push(head, stored_line);
    }
    free(line);

    // Print the lines in reverse order
    for (Node *curr = head; curr != NULL; curr = curr->next)
    {
        fprintf(output, "%s", curr->line);
    }

    free_list(head);

    if (input != stdin)
        fclose(input);
    if (output != stdout)
        fclose(output);

    return 0;
}