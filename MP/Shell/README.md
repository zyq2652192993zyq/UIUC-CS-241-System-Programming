> # Shell

## DISCLAIMER

If your code fork bombs on any autograde, then you will automatically fail this MP. Please make sure that your `fork()` code is correct before committing your code for the nightly autograder.

If you use `system()`, you will automatically fail this MP.

## Backstory

Now that you’ve completed your text editor, your mentor has assigned you a new task of implementing a simple Unix shell. The basic function of a shell is to accept commands as inputs and execute the corresponding programs in response. You will be provided the `Log`, `Vector`, and `format` libraries for your use.

## Overview

Your shell should operate like bash, which you are probably used to using. To execute multiple commands, your shell should run in a loop like this:

- Print a command prompt
- Read the command from standard input
- Print the PID of the process executing the command (with the exception of built-in commands), and run the command

## Starting your shell

Your shell will support two optional arguments: `-h` and `-f`. Both options can potentially be used at once.

`-h` takes the filename of the history file. The shell should load in the history file as its history. Upon exit, the history file should be updated.

```bash
./shell -h <filename>
```

`-f` takes the name of the file to be executed by the shell. Your shell will run the commands in the file in sequential order. If the end of the file is reached without executing an exit command, then the shell should issue an exit command. See the following example file and execution:

commands.txt

```bash
./shell -f commands.txt
(pid=1234)/home/user$ cd cs241
(pid=1234)/home/user/cs241$ echo Hey!
Command executed by pid=1235
Hey!
(pid=1234)/home/user/cs241$ exit
```

## Specifics

### Prompting

When prompting for a command, your shell should print a prompt in the following format:

```
(pid=<pid>)<path>$
```

`<pid>` is the current process ID, and `<path>` is a path to the current working directory. Note the lack of a newline at the end of this prompt.

### Reading in the Command

The shell will read in a command from `stdin` (or a file if `-f` was specified).

### Running the Command

The shell should run the command that was read in previously.

If the command is run by a new process, the PID of the process should be printed like this:

```
Command executed by pid=<pid>
```

This should be printed before any of the output of the command is printed.

### History

Your shell should store the command that was just executed. Every command should be stored unless otherwise noted.

### Ignore Ctrl+C

Usually when we do *Ctrl+C*, the current running program will exit. However, we want the shell to ignore the *Ctrl+C signal*. The shell will not exit when a *Ctrl+C* signal comes.

## Commands

Shell supports two types of commands: built-in and non-built-in. While built-in commands are executed without creating a new process, a non-built-in command MUST create a new process to execute the program for that particular command.

Words in a command will be separated by a single space, and there will be no extra whitespace anywhere in the command. Your shell does not need to support quotes (for example, `echo "hello there"`). We will not test a blank line.

## Built-in Commands

### `cd <path>`

Changes the current working directory of the shell to `<path>`. The starting point for pathnames that don’t start with `/` will be the current directory.

If there is an error, then the shell should print `<path>: No such file or directory`.

```
(pid=1234)/home/user$ cd code
(pid=1234)/home/user/code$ cd imaginary_directory
imaginary_directory: No such file or directory
(pid=1234)/home/user/code$
```

### `exit`

Terminates the shell and saves the history of the shell if `-h` was specified.

### `!history`

Prints for each command (in chronological order) its line number (0 indexed), a tab, the command itself, and a newline. Do not store this command in the history.

```
(pid=1234)/home/user$ !history
0    ls -l
1    pwd
2    ps
(pid=1234)/home/user$
```

### `#<n>`

Prints and executes the nth command in history (chronological order), where n is a non-negative integer. Other values of n will not be tested. Note that the command run should be stored in the history. If n is not a valid index then print `Invalid Index`, and do not store anything in the history. Note that this command is never stored in the history, only the command that is being referenced in the history. The following example assumes a fresh history:

```
(pid=1234)/home/user$ echo Echo This!
Command executed by pid=1235
Echo This!
(pid=1234)/home/user$ echo Another echo
Command executed by pid=1236
Another echo
(pid=1234)/home/user$ !history
0    echo Echo This!
1    echo Another echo
(pid=1234)/home/user$ #1
echo Another echo
Command executed by pid=1237
Another echo
(pid=1234)/home/user$ #9001
Invalid Index
(pid=1234)/home/user$ !history
0    echo Echo This!
1    echo Another echo
2    echo Another echo
(pid=1234)/home/user$
```

### `!<prefix>`

Prints and executes the last command that has the specified prefix. Note that the command run should be stored in the history. If no match is found print `No Match`, and do not store anything in the history. Note that this command is never stored in the history, only the command that is being referenced in the history. The following example assumes a fresh history:

```
(pid=1234)/home/user$ echo Echo This!
Command executed by pid=1235
Echo This!
(pid=1234)/home/user$ echo Another echo
Command executed by pid=1236
Another echo
(pid=1234)/home/user$ !e
echo Another echo
Command executed by pid=1237
Another echo
(pid=1234)/home/user$ !echo E
echo Echo This!
Command executed by pid=1238
Echo This!
(pid=1234)/home/user$ !d
No Match
(pid=1234)/home/user$ !history
0    echo Echo This!
1    echo Another echo
2    echo Another echo
3    echo Echo This!
(pid=1234)/home/user$
```

## Non-built-in Commands

For commands that are not built-in, the shell should consider the command name to be the name of a file that contains executable binary code. Such a code must be executed in a process different from the one executing the shell. You must use `fork()`, `exec()`, `wait()`.

The `fork()`, `exec()`, `wait()` paradigm is as follows: `fork()` a child process. The child process must execute the command with `exec()`, while the parent must `wait()` for the child to terminate before printing the next prompt. It is important to note that, upon a successful execution of the command, `exec()` never returns to the child process. `exec()` only returns to the child process when the command fails to execute successfully. In that case, shell should print (without the quotes): “xxx: not found”, where xxx is the executable name. For example:

```
(pid=1234)/home/user$ invalid_command --flag-that-is-ignored also_ignored also_ignored
Command executed by pid=1235
invalid_command: not found
(pid=1234)/home/user$
```

Note that all non-built-in commands should be stored in the history, even ones that are invalid.

Some non built-in commands that you may try to see whether your Shell works as it should are:

```
/bin/ls
pwd
ps
echo hello
```

Please read the disclaimer at the top of the page! We don’t want to have to give any failing grades.

### `&`

A command suffixed with & should be run in the background. The shell should be ready to take the next command before the given command has finished running. There may or may not be a single space between the rest of the command and `&`. For example, `pwd&` and `pwd &` are both valid. Additionally, since spawning a background process introduces a race condition, it is okay if the prompt gets misaligned as in the following example:

```
(pid=1873)/home/user$ pwd &
(pid=1873)/home/user$ Command executed by pid=1874
/home/user
When I type, it shows up on this line
```

## Compile and Run

Because we have provided `Log` and `Vector` as precompiled archive files, please make sure to work on this assignment on your student VM. We can’t say what will happen on any other machine when you try to compile the assignment.

To compile the release version of the code run:

```bash
make
```

This will compile your code with some optimizations enabled. If you use a debugger on the ‘release’ build, it will not be able to show you the original source code, or line numbers, most of the time. Optimizations sometimes expose some bugs in your code that would not show up when no optimizations are enabled, but since optimizations tend to reorder your code while compiling, an optimized version of your code is not optimal for debugging.

To compile your code in debug mode, run `make debug` instead of `make`

If you compile in release mode, you will an executable called `shell`. If you compile in debug mode, you will get an executable call `shell-debug`.



























































