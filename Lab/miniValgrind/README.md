> # miniValgrind

# Demo

Your section leaders will attempt to demo a implementation of a working Mini Valgrind. Please do also make an effort to read over the code that is provided for you.

# Learning Objects

* Metadata
* Review of memory management and linked lists
* Preparation for the Malloc MP

# Overview

For this lab, you will be implementing a small version of Valgrind. Valgrind is a great tool which monitors your memory usage which you have likely used already. Your version will print out a summary of the memory leaks in a particular C program This lab is meant in part as preparation for your Malloc MP, introducing some topics and techniques which you will find helpful when approaching Malloc soon.

# Main Concepts

The main concept of this lab is using some extra memory for each allocation (which we will call metadata) to track each block of allocated memory. We have provided you with a struct `_meta_data` in `mini_valgrind.h`. The metadata is set up as a node of a linked list (remember 125 and 225?) which should store information for each allocated block of the requested size. This includes the memory address of the actual block, line number it was allocated at, the size, filename, and a pointer to the next allocated block.

If you do feel that you need a refresher on linked lists or other prerequisite concepts, feel free to ask a CA/TA one-on-one.

# `mini_valgrind.c` and `mini_valgrind.h`

There are five functions in total you must be writing.

**mini_malloc**

Here you are to write a wrapper function for malloc that will not only allocating the required space but also allocate and set up metadata to track each requested block allocated by the standard `malloc`. A call to `insert_meta_data` should be made to insert into the linked list of allocated blocks. **NOTE:** you do not have to write your own implementation of `malloc` using `sbrk` or related system calls. You will call the standard `malloc`, simply allocating more space than normally due to the metadata. Return null if `malloc` fails, otherwise return the pointer to the allocated block of memory **NOT** the metadata.

Take a look at the `#define` statements in `mini_valgrind.h` to understand how this is being used. Note the two macros `__FILE__` and `__LINE__`.

**insert_meta_data**

Here you are passed a pointer to your metadata, the size of the block without metadata, the filename, and the line number from which the allocation was made in the .c file being run. You should ensure your metadata is set up here, and insert into the linked list at the head **in constant time**. You should be adding new nodes to the head of your linked list defined in the `mini_valgrind.h` file. Ensure that you update the `total_usage` here and deal with insertion into a linked list just as you have in 125/225.

**mini_free**

Here you are passed a pointer to a block of previously allocated memory. To implement this function, use `remove_meta_data` properly, and also consider what happens when you free a `NULL` pointer.

Take a look at the `#define` statements in `mini_valgrind.h` to understand how this is being used.

**remove_meta_data**

Remove your metadata passed in as a parameter from the linked list here. Ensure that you update `total_free` or `bad_frees` (depending on whether the block pointed to was previously allocated, not already freed, etc.) here and deal with removal from a linked list just as you have in 125/225, keeping in mind the different cases that might come about (such as removal of the head node, etc.). You should free the metadata and requested block.

**destroy**

Here you must delete all nodes of the linked list that have been created. Ensure that you **DO NOT** add to `total_free` here. This is called when the program has finished executing and so any blocks that have not been deallocated should be counted as memory leaks.

# Reference Executables

You will be given a reference executable as usual for this lab. As usual, please direct as many “What should my code do in case X” questions as you can to the reference implementation first, but do feel free to ask us after checking. NOTE: You must run make each time you would like to test with the reference when you change `test.c`.

# Testing

If you would like to check your program, you may write tests within `test.c`. We recommend checking your program on a variety of inputs.

Helpful Hints and Notes

- **DO NOT EDIT** print_report! You risk failing the autograder if you do! No one wants that!
- A review of pointer arithmetic might be useful here.
- Notice `char file_name[MAX_FILENAME_LENGTH]`; within the struct in `mini_valgrind.h`. Ensure that you write the filename over properly as the length of `file_name` is bounded by `MAX_FILENAME_LENGTH`.

------

`mini_valgrind.h`头文件给出了``_meta_data``结构体的定义，以及一些函数的声明，我们的工作是去完成这些函数。

* `#undef`资料参考：https://www.cnblogs.com/Oude/p/12038942.html

注意程序里面使用`#undef`了`malloc, realloc, free`。

这个项目的关键点是理解`meta_data`的原理以及如何处理`realloc`，可以通过下面这张图来理解：

![meta_data_linked_list](http://cs241.cs.illinois.edu/images/assignment-docs/lab/mini_valgrind/mini_valgrind.jpg)

我们通过`#undef malloc`进制直接调用系统的`malloc`，因为系统的`malloc`也会产生`meta_data`信息，这样就只允许调用自己定义的接口。仔细理解一下在`mini_valgrind.h`里的定义：

```c
typedef struct _meta_data {
    // size of the memory allocated by malloc
    size_t size;
    size_t line_num;
    char file_name[MAX_FILENAME_LENGTH];
    struct _meta_data *next;
} meta_data;
```

其中`size`代表分配给`memory`的大小，`line_num`记录在程序的第几行调用了`malloc`来进行动态内存分配，值等于`__LINE__`；`file_name`就是记录程序的文件名，值等于`__FILE__`，`next`指针指向下一个`meta_data`的部分。

注意我们已经`#undef`里`malloc`，所以使用`malloc(size)`实际上调用的是`mini_malloc(size, __FILE__, __LINE__)`，并且不是调用系统的`sbrk()`函数来分配内存，而是模拟整个过程。这里`mini_malloc`的返回类型仍然是`void *`，执行`malloc`首先分配给`meta_data`空间，然后分配给`memory`空间，返回的指针是`ptr1`，但是我们实际希望返回的是`ptr2`，所以还需要加上一个`sizeof(meta_data)`，用全局变量`total_usage`来记录使用的空间的大小。

从图中还可以看出，内存块之间是链表的形式，所以插入新的内存块的过程是，假如新的内存块为`md`，原起始位置为`head`，则分配内存后`head`位置是`head + md + sizeof(meta_data)`。

`realloc`根据注释部分的提示可以知道，首先需要检查指针`ptr`。传入的`ptr`如果是个空指针，那么调用`malloc`为它分配内存。在`total_usage`的计算上，如果如果`new_size`大于`old_size`，那么`total_usage`增加值为两者的差值，如果是小于等于的关系，则`total_usage`没有任何变化。另外还需要更新`total_frees`的信息。

因为传入的指针`ptr`指向的是`memory`起始地址，但是这个内存块究竟多大我们并不知道，所以需要从链表的首部开始遍历，去查找这个`ptr`的具体位置，从而来得到它的`meta_data`信息，才能去比较`old_size`和`new_size`的大小，从而来决定后续的操作。

`free`的实现借助了一个辅助函数`remove_meta_data`，设计思路和`realloc`基本一致，只不过顺序倒过来。在`free`这里需要去更新`total_frees`的信息。

另外老师还提供了`print.c`和`print.h`两个文件，这两个文件无需修改，但是要在自己的测试程序里面进行调用。最后的打印信息是：

```
Heap report:


Program made 1 bad call(s) to free.

Heap summary:

	Total memory usage: 68 bytes, Total memory free: 68 bytes
	No leak, all memory are freed, Congratulations!

```

