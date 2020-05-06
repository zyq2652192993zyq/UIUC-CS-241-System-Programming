> # Text Editor

# Backstory

You have just finished implementing a `Document` library and now your mentor wants you to implement a text editor using your `Document` library. Since you’ve already turned your `Document` library over to the QA team, who has run a brutal comprehensive set of tests (and fixed any bugs they found), your mentor has decided to provide you with a compiled version of the post-QA `Document` and `Vector` (as an “archive file”) for use in your text editor.

# Overview

Your editor can be run like any other editor with:

```bash
$ ./editor <filename>
```

The filename provided will be loaded into the `Document` for manipulation using `Document_create_from_file()`. Then, `editor_main.c` reads commands from `stdin`. When a command is read, the appropriate function in `editor.c` is called. You will need to fill in the functions in `editor.c`.

**Important:** Lines will be 1 indexed for this assignment!

**Important:** Send all your debug printing to `stderr` instead of `stdout`.

Please keep the following in mind when implementing your text editor:

- Lines are 1 indexed (are they 1 indexed in the document? Be careful!)
- There is no limit on the number of lines
- There is no limit on the number of characters per line
- Some lines may be empty (you should treat these as “”)
- Only when the user gives the save command will the contents of the file on disk be modified.
- All your editor operations should use the `Document` library to modify the document. Don’t write directly to the file!

# Reference Implementation

In case some of the documentation does not go into detail on edge cases or if you have questions on how your text editor should behave under certain conditions, please refer to `editor_reference`.

`editor_reference` is an executable that the company stole (don’t ask), which is what your manager wants implemented, since the company does not have its source code. For evaluation purposes `editor_reference` is the final say for how your editor should behave.

# Format Library

We have provided a format library that handles all printing to `stdout`. This is to ensure that you do not lose points for not matching the format that the autograder expects.

Please take a look at `format.c` and `format.h`. These files include all the error messages and print outs that you need. (There is also an easter egg function that might come in handy for multi line writes)

# Features

Make sure you can perform the following basic tasks from your editor program:

- Display the contents of a file.
- Write and append text to a file at a specified line number.
- Delete text from a file at a specified line number.
- Find text in a file and display the matching text with the line number.
- Save the file to disk.
- Quit the editor

# Display contents

Your text editor should be able to print out the content of the file/Document that is loaded.

Your editor provides two print commands:

1. print a single line
2. print the whole Document

### print a single line

The user will provide the “p” command along with a line number when they want to print out a single line from a file. It’s also useful to get some context, so we will print out the 5 lines above and below this line for them as well. If there are not 5 lines above or below the requested line (say the line requested is line 2), then print out as many lines as you can, up to 5.

To print the contents of the file `editor.h` using the editor, the user would first open the file with the editor:

```bash
$ ./editor editor.h
```

Then, to print out whatever is on the 30th line of this file, type:

```bash
p 30
```

Then hit enter.

The editor will now print out line 30 of the file (including five lines above and below for context). (this is done by calling `handle_display_command()` in `editor.c` with the string “p 3” as `command`).

```bash
$ ./editor editor.h
p 30
25	 * based on the command passed in.
26	*/
27	void handle_append_command(Document *document, const char *command);
28
29	/**
30	 * Handles deleting from the document
31	 * based on the command passed in.
32	*/
33	void handle_delete_command(Document *document, const char *command);
34
35	/**
```

Suppose we had a file `things_on_my_table.txt` which contained the following:

```
mug
salt
T.V. remote
```

(since I am very clean there are only three things on my table)

If we try to print out line 3 of this file, the printout will include up to 5 lines above and below the line specified:

```bash
$ ./editor things_on_my_table.txt
p 2
1    mug
2    salt
3    T.V. remote
```

**Important:** the “p” command includes line numbers in it’s print out. Make sure to use `format.{c,h}` to print this line out. Remember that lines are 1 indexed, but make sure to take a look at `format.c`.

### print the whole document

The user will provide the “p” command (with no line number specified) when they wish to print the whole document.

For example (using the same file we used above):

```bash
$ ./editor things_on_my_table.txt
p
1    mug
2    salt
3    T.V. remote
```

Again, make sure to use `format.{c,h}` to print these lines out.

### Errors

If the user asks you to print a document which is empty (calls “p” with or without a line number on an empty document), use `print_document_empty_error()` in `format.{c,h}` to tell them they can’t do that.

If a user asks you to print a specific line which does not exist, please call `invalid_line()` in `format.{c,h}` to let them know.

# Writing and Appending Text

There are two write modes for your editor: “write” and “append”.

### write command

The “w” command should **OVERWRITE** a line completely. For example:

```bash
w 3 I like cats!
```

This will **OVERWRITE** line 3 with “I like cats!”.

### append command

“a” will **APPEND** it’s argument to the end of a line. For example, if line three contained “I like cats!”, then the user ran this command:

```bash
a 3  I also like dogs.
```

(append the string “ I also like dogs.” to line three, notice the extra space)

They should then be able to run the print command and see the following output:

```bash
1   mug
2   salt
3   I like cats! I also like dogs.
```

Note: If the line a user is appending to is currently empty, that’s fine!

Note: If your editor is asked to write or append to a line that does not exist yet, then it should fill in the gap with empty lines. (How can you use `Document` to make this easy?)

### muti-line writes or appends

An extra feature your editor needs to account for is multi-line writes. For example, the user can input the following:

```bash
w 3 I like cats!$Dogs are alright.
```

This will OVERWRITE line 3 with “I like cats!”.

Second, because there is a “$”, this will ALSO create a NEW line (the new line four) reading “Dogs are alright.”.

The old line 4 and all subsequent lines after get shifted down. Both the “w” and “a” modes may contain multiple dollar signs, and an additional line should be inserted for each.

So, for a complete example:

```bash
$ ./editor things_on_my_table.txt
p
1   mug
2   salt
3   I like cats!
a 3  I also like dogs$But I prefer cats.
p
1   mug
2   salt
3   I like cats! I also like dogs
4   But I prefer cats.
```

# Deleting text

The “d” command should delete a single line from a file. For example:

```bash
$ ./editor things_on_my_table.txt
p
1    mug
2    salt
3    I like cats!
4    Dogs are alright.
d 3
p
1    mug
2    salt
3    Dogs are alright.
```

Notice that the line “Dogs are alright” shifted up. (Is there anything in `Document` that might help you with this?)

### Errors

If a user attempts to delete a line which does not exist, let them know! Call `invalid_line` from `format.h`.

# Finding Text

Your text editor should be able to find all occurrences of a word and display the occurrences of the word with the line number the word occurred on.

If your user inputs the following into standard input:

```bash
/like cats
```

Then your text editor should print all occurrences (the whole line) with their line numbers in following format:

```bash
3    I [like cats]!
```

You only need to surround the first occurrence of the search term in each line with square brackets. Use the `print_search_line()` function in `format.h` for this.

So, for another example, suppose I had a file `kitties.txt` with the content:

```bash
I like cats
I like cats
Dogs are alright
I like cats
```

And I ran a search operation:

```bash
$ ./editor kitties.txt
/cats
1	I like [cats]
2	I like [cats]
4	I like [cats]
```

# Saving Text

Your text editor should be able to save all of the changes you make to the file on disk (otherwise, what’s the point?)

The save command is just the character “s”

```bash
s
```

# Quitting Your Text Editor

Your text editor should be able to quit without memory leaks.

So if your user inputs the following into standard input:

```bash
q
```

Then your text editor should quit and free/destroy your `Document` (even if there are changes the user has not saved).

# Awesome Scripts

Since the editor reads commands from standard in, we can create a file, fill it with commands, then send those commands to the editor.

Suppose you think 241 is the best class ever and you want to write a script that makes sure everyone else knows that too. Create a file containing the following:

```bash
w 1 I LOVE 241
w 2 241 is my FAVORITE class at UIUC
w 3 I$LOVE$CS$241
s
q
```

Lets try running this two different ways. First, on a file that doesn’t exist:

```bash
$ ./editor new_file < script
p
1   I LOVE 241
2   241 is my FAVORITE class at UIUC
3   I
4   LOVE
5   CS
6   241
```

And on a file that already has some content:

```bash
$ ./editor editor.c < script
$ head editor.c
I LOVE 241
241 is my FAVORITE class at UIUC
I
LOVE
CS
241

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
```

Scripts like this might be able to help you debug. Perhaps you can impress your mentor with a bash script (google it) that run some editor scripts over some files you create, then uses the tool `diff` (check the man pages) to make sure your editor did the right thing.

# Compile and Run

Because we have provided `Document` and `Vector` as a precompiled archive file, please make sure to work on this assignment on your student VM. We can’t say what will happen on any other machine when you try to compile the assignment.

To compile the release version of the code run:

```bash
make
```

This will compile your code with some optimizations enabled. If you use a debugger on the ‘release’ build, it will not be able to show you the original source code, or line numbers, most of the time. Optimizations sometimes expose some bugs in your code that would not show up when no optimizations are enabled, but since optimizations tend to reorder your code while compiling, an optimized version of your code is not optimal for debugging.

To compile your code in debug mode, run `make debug` instead of `make`

If you compile in release mode, you will an executable called `editor`. If you compile in debug mode, you will get an executable call `editor-debug`

We have also provided a file `editor_test.c` where you can programmatically test your editor. This compiles to `editor_test` and `editor_test-debug`

--------

# 前言

如果不限定必须使用之前写的`Document`来实现的话，**块状链表或者伸展树**更适合来实现一个`editor`，可以通过几个算法题来体会：

- [ ] 洛谷-P5599 【XR-4】文本编辑器
- [ ] 洛谷-P4008 [NOI2003]文本编辑器
- [ ] 洛谷-P4567 [AHOI2006]文本编辑器
- [ ] UVA12417 Formula Editor（公式编辑器 ）
- [ ] UVA11212 Editing a Book（编辑书稿 ）

另外在做完这个还可以参考`Build Your Own Editor`的实现，来吸收设计思路：https://viewsourcecode.org/snaptoken/kilo/index.html

在实现之前应该仔细阅读要求，心中先有一个程序的框架，然后再去写代码。

* 内容展示功能
    * `p`加行号显示上下5行的内容，不足则仅显示存在的部分。
    * 单独一个命令`p`则显示全部内容。
    * 如果传入的行号不在范围内，则用`format.c`提供的接口`print_document_empty_error()`打印错误信息。
* 增加文本功能
    * `w`命令执行覆写功能
    * `a`将新增文本添加到每一行的末尾。
    * 行号超出原有行号，中间的行用空行填充。

* 删除文本功能
    * `d`命令删除行号对应的内容。
    * 行号超出范围进行错误处理。
* 查找文本
    *  对于查找到的文本用方括号显示，每一行只需要显示第一个即可。
* 保存和退出功能

初看需求还挺多，其实完全可以转化成一个算法题，既然要求用`Document`实现，而`Document`是基于`Vector`实现的，那么来将需求一一对应。

* 内容展示，读取`p`后的行号，相当于索引，上下5行就是`max(1, number - 5)`和`min(number + 5, n)`。
* 增加文本，就是对每一行在字符串的末尾添加字符串即可。
* 删除功能相当于删除`Vector`索引对应的元素，后面的元素前移一个位置，时间复杂度可是$O(n)$。
* 查找这个就是对每一行进行字符串查找，因为只需要找到第一个出现的，所以暴力查找即可。如果找出每一行所有的内容，那么KMP走起，每一行查找时间复杂度$O(mn)$，全文查找的时间复杂度可是$O(row \times m \times n)$，其中`row`是总行数，`m`是每一行对应的字符串的长度，`n`代表需要查找的字符串的长度。
* 保存功能和退出功能会出现一种情况，不选择保存而直接退出，那么前面的操作都会失效，这里很容易让人联想到可持久化数据结构，但是这个作业应该没那么复杂，只需要对原`Document`建立一个副本`tmp`，然后输入`s`命令就可以把`tmp`数据写入原`Document`即可。`q`命令结束程序即可，类似算法题里的不确定数量输入，只需要一个`while`循环，不断读入命令来决定操作，`q`相当于对应`break`来退出循环，结束程序。