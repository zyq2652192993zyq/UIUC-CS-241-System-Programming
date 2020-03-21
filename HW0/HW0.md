> # HW0

HW0作业的目的是为了熟悉Linux系统编程以及如果是C语言零基础，可以借此熟悉其语法。

每一个video后都有一个在线编辑器，编写完成代码后点击`Run it`就可以运行了。

# Chapter 1

## 1.Hello World(System call style)

Starting with an empty file, can you write a complete C program to print out your favorite haiku poem, saying or famous CS quote? Use `write` system call to print your text. Your main method should return an integer value zero (to mean it completed successfully). Hint you'll need an `#include` and a `main` function that returns an `int`.

```c
#include <unistd.h> /* Declare write() function */

int main()
{
	char ch[] = "Hi, my name is Kylin Zhang\n";
	write(1, ch, 27);
	
	return 0;
}
```

```bash
$ gcc -lm -Wall -fmax-errors=10 -Wextra program.c -o program
```

## 2.Hello Standard Error Stream



## 3.Writing to files



## 4.Not every is a system call



# Chapter 2

## 1.Not all bytes are 8 bits



## 2.Follow the int pointer



## 3.sizeof character arrays, increasing pointers



# Chapter 3

## 1.Program arguments argc argv



## 2.Environment Variables



## 3.String searching(Strings are just char arrays)



## 4.Lifetime of automatic variables



# Chapter 4

## 1.Malloc, heap and time



## 2.Heap allocation Gotchas



## 3.struct, typedefs and a linked list



## 4.Duplicating strings, memory allocation and deallocation of structures



# Chapter 5

## 1.Reading characters, Trouble with gets



## 2.Introducing sscanf and friends



## 3.getline is useful



# Chapter 6

## 1.Introducing signals and forks



## 2.Introducing fork and waitpid





