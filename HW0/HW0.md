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

这个部分在视频0090-Environment。

比如想要打印环境变量的信息：

```c
#include <stdio.h>

extern char **environ;

int main()
{
    char **ptr = environ;
    while (*ptr) {
        prinf("%s\n", *ptr);
        ++ptr;
    }
    
    return 0;
}
```

```bash
$ ./main
# 这里将会打印一长串环境变量信息，只截取其中一部分 
LANG=C.UTF-8
USER=kylin
PWD=/home/kylin
HOME=/home/kylin
NAME=Lenovo-PC
```

如果想改变环境变量，那么可以在终端输入：

```bash
$ export NAME=PC
```

再次运行程序查看，发现`NAME`变量被设置成了`PC`：

```bash
$ ./main
LANG=C.UTF-8
USER=kylin
PWD=/home/kylin
HOME=/home/kylin
NAME=PC
```

另外一种查看环境变量信息的方法是，比如仍然想查看`NAME`的信息：

```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *name = getenv("NAME");
    printf("%s\n", name);
    
    return 0;
}
```

```bash
$ ./main
LANG=C.UTF-8
USER=kylin
PWD=/home/kylin
HOME=/home/kylin
NAME=PC
```







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

`getline`的函数原型是：

```c
 #include <stdio.h>                                        
                                                           
 ssize_t getline(char **lineptr, size_t *n, FILE *stream); 
```

用法：

```c
#include <stdio.h>

int main()
{
	char *buff = NULL;
	ssize_t res = getline(&buff, &res, stdin);
	printf("%d : %s\n", (int)res, buff);

	return 0;
}
```

```bash
$ gcc main.c -o main
$ ./main
ABCDE
6 : ABCDE

$
```

注意到函数`getline`读入了一个空格，另外`getline`函数动态分配了内存，需要手动释放。

```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *buff = NULL;
	ssize_t res = getline(&buff, &res, stdin);
    if (res > 0 && buff[res - 1] == '\n') {
        buff[res - 1] = '\0';
    }
    
	printf("%d : %s\n", (int)res, buff);
    free(buff);

	return 0;
}
```

这样输出结果后，末尾不会多出一个空格。

# Chapter 6

## 1.Introducing signals and forks



## 2.Introducing fork and waitpid





