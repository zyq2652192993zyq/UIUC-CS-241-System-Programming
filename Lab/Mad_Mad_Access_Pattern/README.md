> # Mad Mad Access Pattern

## Mapping Inventory

You’ve landed an internship at ***Zing!\***, a search engine start-up. Your first project is to develop a keyword advertisement inventory tool.

Advertisers can “buy” keywords such that their ad is shown whenever a user does a search that contains one of their keywords. For example, if a user searches for “stapler”, they will see ads for an office supply company who bought that keyword.

Before buying a keyword, advertisers want to know, “How much will this cost me?” To compute that, they’ll need to know how often the keyword is searched for, and how valuable the word is. For example “how” is a very common word but has little value. “Donate” is less common but highly valuable, because a user searching for “donate” is probably willing to do so.

The intern before you scanned through a large volume of search logs to determine the frequency and value of each word. Your job is to perform efficient searches on the data file she created. This file is expected to grow to be very large, larger than will fit in memory, so you will need to access it without reading it all into memory.

## Data File Structure

The file is structured like a binary search tree, where each tree node is an instance of this structure:

```c
typedef struct {
    uint32_t left_child;  // offset of node containing left child
    uint32_t right_child; // offset of node containing right child

    // Offsets are relative to the beginning of the file.
    // An offset of zero means the child does not exist.

    uint32_t count;  // number of times the word occurs in the data set
    float price;     // price of the word

    char word[0];    // contents of the word, null-terminated
} BinaryTreeNode;
```

The first 4 bytes of the input file are the letters “BTRE”. Any file which does not start with these 4 bytes is an invalid data file. The root node it stored at offset 4 in the file. Each uint32_t and float is stored in little-endian format (the same as the processors your VMs use, so no conversion will be necessary). “word” is an array of ASCII characters at the end of each structure, and it is a null-terminated string. There is no limit on the length of a word in the file or the length of the words your program will look up.

Remember the properties of a binary search tree: if a node has a left child, that child’s value is less than the node. If it has a right child, that child’s value is greater than the node. In other words:

```c
BinaryTreeNode *node = ...a node from the file...

    if (node->left_child) {
        BinaryTreeNode *left = ...load node at node->left_child...
            assert(strcmp(left->word, node->word) < 0);
    }

if (node->right_child) {
    BinaryTreeNode *right = ...load node at node->right_child...
        assert(strcmp(right->word, node->word) > 0);
}
```

![BTRE](https://courses.engr.illinois.edu/cs241/sp2016/images/241_memory_mapped_madness_diagram.png)

## Files

You’ll be given:

- `tree.h` - contains the struct definition above and a detailed description of the data in `sample.data`
- `Makefile` - the `makefile` you should use to build lookup1 and lookup2
- `lookup_reference` - a reference implementation of lookup1
- `sample.data` - a small file containing the words “sample”, “word”, “list”, “for”, “this”, and “program”
- `utils.h` - Printing functions for you to use
- `input_file` - Sample input to create_file executable.
- `create_file` - Creates binary tree structured data file corresponding to `input_file`. This will be used for your test cases.
- `print_file` - Prints out human readable form of binary tree structured file from above.

Make two versions of your program. Both should produce the same results, but using different file access methods.

## Version 1: `fseek` / `fread`

When reading a node from the file, use **`fseek()`** to jump to the correct position and read the node with **`fread()` and/or `fgetc()`**. You may not use `mmap()` for this part.

Put the code for this in `lookup1.c`.

## Version 2: `mmap`

Use **`mmap()`** to map the entire file into memory without reading any part of the file directly. When reading a node from the memory mapped file, use pointer arithmetic to jump to the correct position and read the node using regular pointer dereferencing.

[mmap Introduction](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-6%3A-Memory-mapped-files-and-Shared-memory)

Put the code for this in `lookup2.c`.

**Notice that you can use only `mmap()` to map the WHOLE file** for this version. Do not use other functions to read files.

## In both versions you should:

- Time your program:
  - The timer should start before you open the file
  - The timer should end after your `close(version 1)/unmap(version 2)` the file.
  - Output the duration of your time with `printTime()`
- Use only the printing function we give you in `utils.h`

## Sample usage

```
lookup1 <data_file> <keyword> [<keyword> ...]
lookup2 <data_file> <keyword> [<keyword> ...]

% ./lookup1 sample.data list sample werd
list: 12 at $0.04
sample: 25 at $10.60
werd not found
Execution time:0.000146 seconds

% ./lookup2 sample.data list sample werd
list: 12 at $0.04
sample: 25 at $10.60
werd not found
Execution time:0.000026 seconds
```

## Testing notes:

- Write your test cases to include comparison of performance between two versions- lookup1 and lookup2. Think about which one is faster and why?
- In order to generate different binary tree structured data files based on input required for each test case, use create_data executable. See input_file for sample input to this executable.
- print_file executable will provide the human readable form of binary tree structured data file generated from create_data executable.

Error cases:

- If run with less than 2 arguments, your program should print an error message describing the arguments it expects and exit with error code 1.
- If the data file cannot be read or the first 4 bytes are not “BTRE”, print a helpful error message and exit with error code 2.

For each word that is found, print its count and its price, where the price is always printed with exactly two digits to the right of the decimal point.

-------

首先可以利用在`prepare_provided`下面的程序简单了解下整个流程：`create_data`需要两个参数`input_file, output_file`，在这里`input_file`就是目录下的可执行文件，`output_file`可以自己定义，`print_file`将输出结果以可读的形式展示，使用方法

```bash
$ ./create_data input_file out.txt
$ ./print_file out.txt
offset 76: "for", count=340, price=$56.93, left=0, right=0
offset 52: "list", count=30, price=$0.04, left=76, right=120
offset 120: "program", count=59, price=$0.01, left=0, right=0
offset 4: "sample", count=10, price=$10.60, left=52, right=28
offset 96: "this", count=3, price=$0.33, left=0, right=0
```

做好这个作业的核心需要看`tree.h`内关于数据格式的定义，`sample.data`内的数据是16进制格式的：

```
4254 5245 3400 0000 1c00 0000 1900 0000
9a99 2941 7361 6d70 6c65 0000 6000 0000
0000 0000 1100 0000 b81e a53f 776f 7264
0000 0000 4c00 0000 7800 0000 0c00 0000
0ad7 233d 6c69 7374 0000 0000 0000 0000
0000 0000 0a00 0000 52b8 6342 666f 7200
0000 0000 0000 0000 0900 0000 c3f5 a83e
7468 6973 0000 0000 0000 0000 0000 0000
0300 0000 0ad7 233c 7072 6f67 7261 6d00
```

```
sample.data contains this tree:
                sample
              /        \
          list          word
         /    \         /
      for   program   this

  offset 4: "sample", count=25, price=$10.60, left=52, right=28
  offset 28: "word", count=17, price=$1.29, left=96, right=0
  offset 52: "list", count=12, price=$0.04, left=76, right=120
  offset 76: "for", count=10, price=$56.93, left=0, right=0
  offset 96: "this", count=9, price=$0.33, left=0, right=0
  offset 120: "program", count=3, price=$0.01, left=0, right=0
```

前4个字符是`BTRE`，需要跳过，所以从34开始的数据才有意义。

```
34 00 00 00: 代表左子节点的偏移量，34转为十进制是52，意味着左子节点的偏移量在52位置处
1C 00 00 00: 右子节点的偏移量，同上换算得到偏移量为28
19 00 00 00: 代表根节点的count数值，换算后为25
9A 99 29 41: 浮点数，代表价格，换算后得到10.60
73 61 6D 70 6C 65 00: 代表单词，通过null结尾
```

所以`lab`的核心内容就是利用`sample.data`内的数据，读入信息，构建一棵二叉树，然后查找信息。`utils`里提供了一些输出需要的函数：

```c
void printTime(const double time);
void openFail(const char *file_name);
void mmapFail(const char *file_name);
void formatFail(const char *file_name);

void printFound(const char *word, const uint32_t count, const float price);
void printNotFound(const char *word);
```













