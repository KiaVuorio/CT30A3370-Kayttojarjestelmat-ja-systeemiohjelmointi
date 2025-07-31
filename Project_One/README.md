# Reverse Utility

## Overview

This utility reads lines from a file or standard input and prints them in reverse order. It supports three modes of operation:

- Reading from standard input and printing to standard output
- Reading from an input file and printing to standard output
- Reading from an input file and writing to an output file
  Build

To compile the program, simply run:

```
make
```

This will produce an executable named reverse.

## Usage

Run the program in one of the following ways:

```shell
./reverse
./reverse input.txt
./reverse input.txt output.txt
```

When run without arguments, the program reads lines from standard input until EOF (Ctrl+D), then prints them in reverse order.
When given one argument (input file), it reads the file and prints reversed lines to standard output.
When given two arguments (input and output files), it reads from the input file and writes reversed lines to the output file.

## Error Handling

If the input and output files are the same, the program prints the error message
Input and output file must differ and exits with status code 1.
If a file cannot be opened, the program prints
error: cannot open file 'filename' and exits with status code 1.
If memory allocation fails, it prints
malloc failed and exits with status code 1.
If the user provides more than two arguments, the program prints
`usage: reverse <input> <output>` and exits with status code 1.
All error messages are printed to standard error.

## Assumptions

Lines can be arbitrarily long.
Files can be arbitrarily large.
The program stores all lines in memory before printing them in reverse order.
