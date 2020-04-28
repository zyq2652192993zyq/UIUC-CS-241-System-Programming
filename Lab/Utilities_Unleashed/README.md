># Lab-3 Utilities Unleashed

# Demo

Your section leaders will demo a working implementation of all the C utilities.

# Learning Objectives

- C Utilities
- Fork, Exec, Wait
- Environmental Variables

# Overview

In this lab, you will be implementing the following C utilities:

- [time](https://courses.engr.illinois.edu/cs241/sp2016/utilities_unleashed.html#time)
- [env](https://courses.engr.illinois.edu/cs241/sp2016/utilities_unleashed.html#env)

Notes

- Do not worry about flags or features that we do not mention.
- Do not print any of your debug information out for your final submission.
- All printing should be handled with format.h.

# `format.c` and .h

Since this lab requires your programs to print error messages to stdout and stderr, we have provided you with format.c and format.h. You should not be printing out to stdout and stderr at all (expect for the print feature of env); instead, you should be using the functions provided in format.c and .h. In format.h you can find documentation of what each function does and you should use them whenever approriate. This is our way of ensuring that you do not lose points for formatting issues, but it also means that you are responsible for handling any and all errors mentioned in format.c and format.h.

# Reference Executables

For this lab, we have provided the following reference executables:

- time_reference
- env_reference

These are correct implementations of the programs you are being asked to make. How your programs should behave in edge cases are answerable by using the reference executables. For example, if you are wondering “What should time do if not given the correct number of arguments?” then just try it out:

```bash
./time_reference
```

and you should see the following output:

```bash
./time <command> [args]
```

# `time`

In this lab, you will be implementing `time`.

```bash
time – run a program and reports how long it took.
```

So if a user enters

```bash
./time sleep 2
```

then time will run ‘sleep’ with the argument ‘2’ and record how long it took in seconds.

```
2.002345 seconds
```

Note that we only care about wall clock time and we recommend [clock_gettime](http://linux.die.net/man/3/clock_gettime) and CLOCK_MONOTONIC.
Pro Tip: 1 second == 1,000,000,000 nanoseconds.
Nota Bene:

- You MAY NOT call on the existing time program.
- You must use fork, exec, wait (no other solutions will be accepted).
- You need to account for programs that do not terminate successfully (the program’s exit code is non-zero).
- We will only run your time with one program.
- The commands we will run can take any number of arguments.
- Do your time computations with double-precision floating pointer numbers (double) rather that single-precision (float).
- We have provided functions in format.h that we expect you to use wherever appropriate.

### Useful Resources

- [Program arguments argc argv](http://cs-education.github.io/sys/#chapter/2/section/0/activity/0)
- [Fork/Exec/Wait](https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-2%3A-Fork%2C-Exec%2C-Wait)
- [Fork and Waitpid](http://cs-education.github.io/sys/#chapter/5/section/1/activity/0)

# `env`

In this lab, you will be implementing ‘env’.

```
env – run a program in a modified environment When run without arguments, it prints a list of all the current environment variables. When run with arguments, it will be given at least two arguments:
```

This means a user will call your program like so:

```
./env <var-list> <command-name>
```

- `<var-list>` is a comma-separated list of changes that are to be made to environment variables, and `<command-name>` is the name of a command that is to be run after making those changes.
- Any additional arguments are to passed as arguments to `<command-name>`.
- Each of the environment variable changes in `<var-list>` are in the form `<destvar>=<value>`.
- `<destvar>` is the name of the environment variable that is to be changed and `<value>` is the new value.
- `<value>` may contain references to environment variables in the form `%<srcvar>`.
- Each reference to `<srcvar>` should be replaced with the value of `<srcvar>`.
- The names of the variables `<destvar>` and `<srcvar>` will contain only letters, numbers, or underscore characters.
- For each environment variable change in `<var-list>`, your program will assign `<value>` to `<destvar>` in the current environment so when `<command-name>` is executed, it will inherit the new value of the `<destvar>` variable.

For example if the user enters

```bash
./env
```

then you should print out all the environment variables. Try

```bash
./env_reference
```

in your terminal to see it in action.

If the user enters

```bash
./env TZ=MST7MDT date
```

then it changes the TZ environment variable while running the date command.

And if the user enters

```shell
./env PATH=%HOME/bin:%PATH,IDIR=/%HOME/include,LIBDIR=/%HOME/lib make -j4
```

then it changes the PATH, IDIR, and LIBDIR variables while running make with the j4 option.

Nota Bene:

- You MAY NOT use the existing env program.
- All changes in enviroment variables and execution must happen only in the child process.
- You MUST use fork/exec/wait.
- If a variable doesn’t exist, interpret its value as a zero-length string.

### Useful Resources

- [Envionrment Variables](http://cs-education.github.io/sys/#chapter/2/section/1/activity/0)
- [Enviornment Variable Functions](http://www.gnu.org/software/libc/manual/html_node/Environment-Variables.html)
- [String.h](http://man7.org/linux/man-pages/man3/string.3.html)
- [Split A String By A Delimiter](https://www.quora.com/How-do-you-write-a-C-program-to-split-a-string-by-a-delimiter)

-----

此项目的主要目标是实现`time`库和`env`命令，错误输出不需要自己来写，直接用`format.c`里面提供的函数。

此项目可以加深对`fork, exec, wait`的理解，以及对环境变量的理解。

实现`time`命令可以加深对内核态和用户态的理解。

# 实现`time`命令

课程提供了一个可执行文件`time_reference`，未提供源代码，可以作为例子来体会命令的使用。

不需要像标准命令一样实现得很完整，这里只需要能计算程序得`wall clock time`，然后遵循几点要求。

* 不允许调用已有得`time`库。
* 必须使用`fork`，`exec`，`wait`。
* 需要去处理程序异常。
* 每一次只会运行一个程序来计算时间。
* 输入命令得参数个数可能是任意的。
* 用双精度浮点数`double`而不是单精度浮点数`float`。
* 不需要自己手动将错误信息传到`stdout`或者`stderror`，要使用`format.c, format.h`内提供的函数来打印错误信息。

**可以参考的资料：**

* APUE 1.10 时间值 8.17和6.10节
* [博客：Linux探秘之用户态与内核态](https://www.cnblogs.com/bakari/p/5520860.html)

-----

去实现`time`命令，首先得学会使用`time`命令。比如我们执行以下命令：

```bash
$  time sleep 2
sleep 2 took 2.085511 seconds                           
real    0m2.014s
user    0m0.000s
sys     0m0.016s
```

进程时间也被称为CPU时间，用以度量进程使用的中央处理器资源，基本数据类型是`clock_t`，一个进程维护3个进程的时间：

* 时钟时间，也叫墙上时钟时间（wall clock time），是进程运行时间的总量。对应打印信息的`real`部分。
* 用户CPU时间。对应打印信息的`user`。
* 系统CPU时间。对应打印信息`sys`。

这里需要明确两个概念：

(1) 什么是用户态和内核态

(2) 时钟时间不是用户CPU时间与系统CPU之和，而是大于等于的关系。

**用户态和内核态**

因为操作系统的资源是有限的，如果访问资源的操作过多，必然会消耗过多的资源，而且如果不对这些操作加以区分，很可能造成资源访问的冲突。所以，为了减少有限资源的访问和使用冲突，Unix/Linux的设计哲学之一就是：对不同的操作赋予不同的执行等级，就是所谓特权的概念，与系统相关的一些特别关键的操作必须由最高特权的程序来完成。

Linux操作系统中主要采用了0和3两个特权级，分别对应的就是内核态和用户态。运行于用户态的进程可以执行的操作和访问的资源都会受到极大的限制，而运行在内核态的进程则可以执行任何操作并且在资源的使用上没有限制。很多程序开始时运行于用户态，但在执行的过程中，一些操作需要在内核权限下才能执行，这就涉及到一个从用户态切换到内核态的过程。比如C函数库中的内存分配函数`malloc()`，它具体是使用`sbrk()`系统调用来分配内存，当`malloc`调用`sbrk()`的时候就涉及一次从用户态到内核态的切换，类似的函数还有`printf()`，调用的是`wirte()`系统调用来输出字符串，等等。

![img](F:\Project\UIUC-CS-241-System-Programming\Lab\Utilities_Unleashed\assets\431521-20160523210140147-1668637440.gif)

发生从用户态到内核态的切换，一般存在以下三种情况：

1）当然就是系统调用：原因如上的分析。

2）异常事件： 当CPU正在执行运行在用户态的程序时，突然发生某些预先不可知的异常事件，这个时候就会触发从当前用户态执行的进程转向内核态执行相关的异常事件，典型的如缺页异常。

3）外围设备的中断：当外围设备完成用户的请求操作后，会像CPU发出中断信号，此时，CPU就会暂停执行下一条即将要执行的指令，转而去执行中断信号对应的处理程序，如果先前执行的指令是在用户态下，则自然就发生从用户态到内核态的转换。

　　注意：系统调用的本质其实也是中断，相对于外围设备的硬中断，这种中断称为软中断，这是操作系统为用户特别开放的一种中断。所以，从触发方式和效果上来看，这三种切换方式是完全一样的，都相当于是执行了一个中断响应的过程。但是从触发的对象来看，系统调用是进程主动请求切换的，而异常和硬中断则是被动的。

另外发现，$real \neq user + sys$，用户CPU时间和系统CPU时间之和为CPU时间，即命令占用CPU执行的时间总和。实际时间要大于CPU时间，因为Linux是多任务操作系统，往往在执行一条命令时，系统还要处理其它任务。

----

了解了基础知识，在程序实现上，主要参考Linux的man里面`clock_gettime`的解释。

```c
#include <time.h>
int clock_gettime(clockid_t clk_id, struct timespec *tp);

/* The res and tp arguments are timespec structures, as specified in <time.h>: */
struct timespec {
    time_t   tv_sec;        /* seconds */
    long     tv_nsec;       /* nanoseconds */
};
```

参数：

* `CLOCK_REALTIME`：系统实时时间，随着系统时间改变而改变，如果被用户修改，则也随之发生变化。显然这个不是我们需要的。
* `CLOCK_MONOTONIC`：从系统启动的时刻开始计时，不受系统时间被用户修改的影响。类似于计时器，是我们需要的。

函数的计时分为两部分，精度为秒的部分用`tv_sec`存储，纳秒部分的精度通过`tv_nsec`来存储，最后的输出结果应该是以秒为单位，所以还涉及一个转化问题，所以纳秒部分的差值需要除以$10^9$。

了解了所需函数的具体意义和用法，那么直接需要面对的问题是：究竟该计算哪个部分的时间？那么总结一下大致流程：

```
输入命令 -> 调用fork()开启一个子进程 -> 子进程执行命令 -> 结束子进程，父进程继续执行
```

所以并不是程序一开始就计算时间，而应该是从`fork()`开始，子进程开始执行的时候开始计算时间。

随之而来的问题是怎样去执行命令，于是用到了函数`execvp()`。

`execvp`是`exec`函数族中的一个，具体解释在这里：<https://www.cnblogs.com/XNQC1314/p/9193305.html>

传入的第一个参数是路径名和命令名，如果第一个参数包含了`/`，就按路径处理，否则在系统的`PATH`里面去寻找。

然后去关注一下提供的异常处理程序`format.h`和`format.c`。

* `print_time_usage`：如果输入的参数有问题进行的处理
* `print_env_usage`：在实现`env`命令里面会用到，如果输入的参数存在错误的相应处理。
* `print_fork_failed`：`for()`出现错误的处理。
* `print_exec_failed`：`exec`在执行过程中是非常容易出错的，对`exec`出错的处理。
* `print_environment_change_failed`：在实现`env`命令里用到，如果修改环境变量失败的处理。
* `display_results`：实现`time`命令的时候，输出最终的时间值。

程序使用方法：

```bash
$ make
$ ./time sleep 2
sleep 2 took 2.010161 seconds
# compare with time command
$ time sleep 2
real    0m2.010s
user    0m0.000s
sys     0m0.000s
```

# 实现`env`命令

`env`命令如果不带参数，则显示当前环境列表，如果带参数，则至少两个参数，命令调用方式：

```bahs
./env <var-list> <command-name>
```

其中`<var-list>`是用逗号分隔的对环境变量的修改，`<command-name>`是在执行完前面的修改后需要执行的命令。额外的参数都视作`<command-name>`的一部分。

环境变量的修改都遵从`<destvar>=<value>`的格式，`<destvar>`是环境变量的名称，`<value>`是新值。

与这部分相关联的在《APUE》的7.5节，但是介绍的很简略，另外网站还提供了GNU的[参考](https://www.gnu.org/software/libc/manual/html_node/Environment-Access.html#Environment-Access)，其实也可以通过`man`来查看。在`HW0`的3.2较少了`export`的用法和显示环境列表的写法。

然后考虑以下程序的设计思路，首先需要检验参数的个数，如果参数只有1个，只需打印环境变量的列表；只有两个，说明传递参数错误。

参数多于两个的时候，前一个是逗号分隔的环境变量名和修改值，剩下的参数是命令，于是很自然的我们需要手写一个`split`函数来切割出逗号之间的部分，剩下的部分`fork(0)`出一个新的进程，利用`execvp`来执行命令，就很类似`time`命令的实现部分。

分析到这里，难点其实就涉及一个`split`函数，复习一下`strtok`函数：

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

void split(char *str, char *delim, char **s)
{
	if (!str || !delim) return;
	
	char *ptr = strtok(str, delim);
	int cnt = 0;
	while (ptr) {
		s[cnt++] = ptr;
		ptr = strtok(NULL, delim);
	}
}
 
int main()
{  
    char s[] = "192,168,0,26";  
    char *delim = ",";
    int num = 0;
	int n = strlen(s);

	printf("%d\n", n);

	for (int i = 0; i < n; ++i) {
		if (s[i] == ',') ++num;
	}
	++num;
    printf("%d\n", num);

	char **ch = (char **)malloc(num * sizeof(char *));
	split(s, ",", ch); 
    
    split(s, delim, ch);
    for (int i = 0; i < num; ++i) {
        printf("%s ", ch[i]);   
    }

	free(ch);

	return 0;
}
```

在对一个长字符串分割的时候，第一次调用时，`strtok`函数的第一个参数传入要分割的字符串，而第二次以及后面再次调用该函数的时候，`strtok`函数的第一个参数应该传入`NULL`，这是因为在`strtok`第一个参数为`NULL`的时候，该函数默认使用上一次未分割完的字符串的未分割的起始位置作为本次分割的起始位置，直到分割结束为止。

上面的只是个小`demo`，完整的见`env.c`，写完后检验是否存在内存泄漏，发现无内存泄漏现象：

```
==531== HEAP SUMMARY:
==531==     in use at exit: 0 bytes in 0 blocks
==531==   total heap usage: 1 allocs, 1 frees, 1,024 bytes allocated
==531==
==531== All heap blocks were freed -- no leaks are possible
==531==
==531== For counts of detected and suppressed errors, rerun with: -v
==531== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

