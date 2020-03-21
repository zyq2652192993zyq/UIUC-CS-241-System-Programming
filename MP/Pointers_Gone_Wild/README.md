> # MP-1 Pointers Gone Wild

# Introduction

In CS 125, CS 225, and other classes, you have used various languages that are considered to be “C based”, but up to now you may have very limited experience in C programming. This MP will provide a short programming introduction to pointers, strings, and functions in C.

This machine problem will be divided up into two pieces. In the first piece, you will be modifying functions originally with errors such that the outputs match the given output. In the second piece, you will need to write some code to call some ‘creatively defined’ functions so that those functions produce the desired output.

For this MP, you may modify:

- `part1-functions.c`
- `part2-main.c`

All other files will be replaced with new/different files for grading. If you modify any other files for debugging purposes, please ensure you test your program with the original file.

# Part 1

There are erroneous/unimplemented functions in `part1-functions.c`. Your task is to modify functions according to comment above each function such that the output of part1 looks exactly as follows:

```
== one() ==
3^2 + 4^2 = 25
10^2 + 10^2 = 200
== two() ==
20 not passed!
100 passed!
== three() ==
The value of p is: 4
== four() ==
The value is between zero and one.
The value is not between zero and one.
== five() ==
x and y are equal.
x and y are different.
== six() ==
4 == 4.000000
432 == 432.000000
== seven() ==
a is a letter.
a is not a letter.
== eight() ==
Hello
== nine() ==
The value of p is: 12.500000
== ten() ==
0 == 0?
== eleven() ==
Hello World!
== twelve() ==
0.000000 0.100000 0.200000 0.300000 0.400000 0.500000 0.600000 0.700000 0.800000 0.900000 
== thirteen() ==
0 10 40 90 160 250 360 490 640 810 
== fourteen() ==
orange and blue!
ORANGE and blue!
Orange and BLUE!
orange and blue!
== fifteen() ==
You passed in the value of one!
You passed in the value of two!
You passed in some other value!
== sixteen() ==
Hello
== seventeen() ==
The radius of the circle is: 17.500000.
The radius of the circle is: 10.000000.
== eighteen() ==
Result: 323
Result: 2499
== clear_bits() ==
170
0
171
0
20
0
== little finite automatons
5
4
6
7
```

# Part 2

We have pre-uploaded some files to your mp0 svn directory, including `part2-functions.c`. Inside `part2-functions.c`, you will see twelve different functions, including `first_step()` (re-printed below).

```c
void first_step(int value) {
    if (value == 81)
        printf("1: Illinois\n");
}
```

To complete Part 2, you must complete the program `part2-main.c` so that `part2-main.c` makes calls to all twelve functions in `part2-functions.c` such that they print their “Illinois” line. When running `./part2`, your output should look exactly like:

```
1: Illinois
2: Illinois
3: Illinois
4: Illinois
5: Illinois
6: Illinois
7: Illinois
8: Illinois
9: Illinois
10: Illinois
11: Illinois
```

> You should NOT edit the part2-functions.c file. In fact, when we grade your program, we will replace the part2-functions.c file with a new version of the file (and we’ll change the “Illinois” string so printing out “Illinois” in a for-loop will get you no credit).

# Compile and Run

To compile the release version of the code run:

```bash
make clean
make
```

This will compile your code with some optimizations enabled, and will not include debugging information (if you use a debugger on the ‘release’ build, it will not be able to show you the original source code, or line numbers). Optimizations sometimes expose some bugs in your code that would not show up when no optimizations are enabled, but since optimizations tend to reorder your code while compiling, an optimized version of your code is not optimal for debugging.

You probably don’t need to worry about the different build types very much for this assignment, but the distinction will become more important on future assignments.

To compile your code in debug mode, run `make debug` instead of `make`

To run Part 1:

```bash
./part1
```

or

```bash
./part1-debug
```

To run Part 2:

```bash
./part2
```

or

```bash
./part2-debug
```

---

第一个Machine Problem就是为了熟悉一下C语言的写法，其中第一个部分只需要修改`part1-functions.c`里的函数，让函数输出正确。因为函数都很基础，所以按照注释的提示和输出结果即可完成。

编写`Makefile`来生成可执行文件`part1-main`，编写规则可在`Reading Notes/Linux/Makefile`查看。

```makefile
CC = gcc
CFLAGS = -Werror -Wextra -Wall -g -std=gnu11 -lm

part1-main: part1-main.o part1-functions.o
        $(CC) $(CFLAGS) part1-main.o part1-functions.o -o part1-main

part1-main.o: part1-main.c
        $(CC) -c part1-main.c

part1-functions.o: part1-functions.c
        $(CC) -c part1-functions.c

clean:
        rm *.o part1-main
```

我们将预期的输出存储在文件`part1-expect-output.txt`文件内，将`part1-main`的输出结果通过重定向到文件`part1-output.txt`中，利用`diff`命令比较结果是否一致。

```shell
#!/bin/bash
make
./part1-main > part1-output.txt
diff part1-expect-output.txt part1-output.txt
make clean
```

如果`diff`命令没有其他输出信息代表通过测试。

------

第二个部分需要修改`part2-main.c`文件，来调用12个函数输出正确的结果。

第二部分其实相当于是第一部分的逆过程，给出函数，构造参数然后得到输出结果，这里分析几个特殊的函数。

第十一个函数：

```c
void tok_step(char *str) {
    char *a = strtok(str, ",");
    a = strtok(NULL, ",");
    if (strcmp(a, "CS241") == 0)
        printf("10: Illinois\n");
    else
        printf("10: ERROR\n");
}
```

这里考察的是对于函数`strtok()`的理解，其实就是在C++里面自己手写一个`split()`函数。函数原型是：

```c
#include <string.h>

char *strtok(char *str, const char *delim);
```

分解字符串为一组字符串。`s`为要分解的字符串，`delim`为分隔符字符串。

函数第一次调用需设置两个参数。第一次分割的结果，返回串中第一个 `',' `之前的字符串,也就是上面的程序第一次输出`abc`。
第二次调用该函数`strtok(NULL,",")`,第一个参数设置为`NULL`。结果返回分割依据后面的字串。

最后一个函数:

```c
void the_end(void *orange, void *blue) {
    if (orange != NULL && orange == blue && ((char *)blue)[0] == 1 &&
        *((int *)orange) % 3 == 0) {
        printf("11: Illinois\n");
    } else
        printf("11: ERROR\n");
}
```

最后修改`Makefile`文件：

```makefile
CC = gcc
CFLAGS = -Werror -Wextra -Wall -g -std=gnu11 -lm

ALL: part1-main part2-main

part1-main: part1-main.o part1-functions.o
	$(CC) $(CFLAGS) part1-main.o part1-functions.o -o part1-main

part1-main.o: part1-main.c
	$(CC) -c part1-main.c

part1-functions.o: part1-functions.c
	$(CC) -c part1-functions.c

part2-main: part2-main.o part2-functions.o
	$(CC) $(CFLAGS) part2-main.o part2-functions.o -o part2-main

part2-main.o: part2-main.c
	$(CC) -c part2-main.c

part2-functions.o: part2-functions.c
	$(CC) -c part2-functions.c

clean:
	rm *.o part1-main part2-main
```

然后利用脚本`check.sh`来进行结果检验：

```shell
#!/bin/bash
make

./part1-main > part1-output.txt
diff part1-expect-output.txt part1-output.txt

# valgrind --tool=memcheck --leak-check=full ./part1-main

./part2-main > part2-output.txt
diff part2-expect-output.txt part2-output.txt

# valgrind --tool=memcheck --leak-check=full ./part2-main

make clean
```

