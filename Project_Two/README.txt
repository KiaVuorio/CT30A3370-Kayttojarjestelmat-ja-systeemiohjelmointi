README.txt
==========
This project demonstrates file handling, string processing, and binary data manipulation in C.  
Each utility aims to behave similarly to its Unix counterpart, with simplified functionality.

This project implements simplified versions of commonly used Unix command-line utilities:
- `cat`
- `grep`
- `zip` 
- `unzip` 

Each utility is implemented as a standalone C program.

---

1. my-cat
---------
Emulates the behavior of the `cat` command. It reads one or more text files and prints their contents to standard output.

- If no files are specified, the program exits silently.
- If a file cannot be opened, it prints:  
  `my-cat: cannot open file`

---

2. my-grep
---------- 
Emulates the behavior of the `grep` command. It searches for a specified search term in one or more files or from standard input.

- If only `searchterm` is provided, input is read from standard input (`stdin`).
- The search is **case-sensitive**.
- Lines containing the search term are printed to standard output.
- If a file cannot be opened, it prints:  
  `my-grep: cannot open file`
- If no search term is provided, it prints a usage message:  
  `my-grep: searchterm [file ...]`

---

3. my-zip
--------- 
Compresses files using Run-Length Encoding (RLE). Output is written in binary format to standard output.

- A sequence of repeated characters is encoded as:
  - a 4-byte unsigned integer for the repetition count
  - followed by 1 byte for the character

- Multiple input files are concatenated and compressed as a single stream.
- If a file cannot be opened, it prints:  
  `my-zip: cannot open file`

---

4. my-unzip
----------- 
Decompresses files previously compressed using `my-zip`.

- Reads a 4-byte unsigned integer (count) followed by 1 byte (character), and prints the character `count` times.

- Multiple RLE files are decompressed sequentially.
- If the compressed file is malformed or ends unexpectedly, it prints:  
  `my-unzip: invalid input file`
- If a file cannot be opened, it prints:  
  `my-unzip: cannot open file`

---

Compilation
-----------
Compile each program with `gcc` as follows:
gcc -o my-cat my-cat.c
gcc -o my-grep my-grep.c
gcc -o my-zip my-zip.c
gcc -o my-unzip my-unzip.c