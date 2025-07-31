# Project three

This project implements Unix shell called wish in C. It supports interactive 
and batch modes. The shell can execute external commands by searching directories 
listed in a configurable path. Supports for built-in commands exit, cd and path, 
and also output redirection >, and parallel command execution using &.  

Handles errors and prints messages to stderr. Commands are executed by fork() and execv().

## Compilation

To compile the project run:
```sh
make
```
This will create executable `wish`.
