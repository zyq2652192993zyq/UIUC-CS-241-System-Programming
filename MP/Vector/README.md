> # README-VECTOR

# Groundwork For Text Editor and Shell

You are an intern for Macrohard and your team has decided to have you write a text editor and shell (more details for each when the time comes), which everyone on the team will use. These projects will take you several weeks and your mentor has decided on the following timetable:

- Week A: Implement a Vector and Document/Log
- Week B: Implement a Text Editor
- Week C: Implement a Shell

The Text Editor will be a 1 line text editor that will accept commands from stdin. Your mentor has decided to abstract away documents for this text editor.

The Shell is a terminal. Like all good terminals; your shell will need to remember what commands its user has given it. Your mentor has decided to abstract all of this away as a log.

Now your mentor has realized that a log and document are not that different in the sense they are both just an array of strings. So to implement the log and document your mentor has decided that you will create a “vector” to store strings.

# Vector

A vector is an array that grows and shrinks as a user adds and removes items from it. However your vector will need to be rich with enough features so one can easily create a log or document from it. Your implementation of a vector should go in `vector.c`, which is the only file that will be sent to your team lead for review. As an intern looking to become a fulltime employee, you should create testcases in `vector_test.c` to show that you are a responsible programmer. Your mentor has left notes in `vector.h` on how you should implement this vector.

# Document and Log

Your document and log are really similar, so your mentor has negotiated with the team lead for you to only implement one of them. The mentor has chosen for you to implement the document. Both the document and log will use your vector to do all the heavy lifting. Still there are some functions that your document needs to have that your vector shouldn’t. Your mentor has provided all the trivial functions for you that assume a correctly implemented vector. The only file that will be reviewed by your team lead is `document.c`. Since Macrohard frowns on interns who rely on QAs to catch their mistakes you are encouraged to write your own testcases in `document_test.c`. Your mentor has left some notes in `document.h`.

# Undefined Behavior

Undefined Behavior is simply a scenario or edge case for which there is no documentation describing how the code should react. For example try finding in the man pages what happens when you feed NULL in `strcmp()`. Your mentor will not answer questions like “What if my user wants an element past the end of the vector?”, because that is undefined behavior. So for the entirety of this MP you should use `assert()` statements to check that your user is passing your function valid input before operating on the input. For example if you were implementing `strcmp(const char *s1, const char *s2)` then your code should look like the following:

```c
#include <assert.h>
strcmp(const char *s1, const char *s2) {
    assert(s1 != NULL && s2 != NULL);
    // Compare the two strings
    .
    .
    .
    return blah;
}
```

# Compile and Run

To compile the release version of the code run:

```
make clean
make
```

To compile your code in debug mode, run `make debug` instead of `make`

To test vector:

```
./vector_test
```

or

```
./vector_test-debug
```

To test document:

```
./document_test
```

or

```
./document_test-debug
```

-----

这个作业的主要目标就是用C语言实现一个C++里的容器`vector`，然后在此基础上实现一个`documrnt.c`。核心要点是对于`malloc`的理解，以及如何利用C语言实现面向对象。

实现的时候，`vector.h`已经给出接口，所以需要在`vector.c`内实现各个函数，在`vector.c`文件内，已经给出了结构体的定义，并给出了一个辅助函数`Vector_dealloc_elem`。注释为`COMPLETE`的是需要完善的函数。

```c
struct Vector;
typedef struct Vector Vector;

typedef void *(*copy_constructor_type)(void *elem);

typedef void (*destructor_type)(void *elem);

Vector *Vector_create(copy_constructor_type c, destructor_type d);

void Vector_destroy(Vector *vector);

size_t Vector_size(Vector *vector);

size_t Vector_capacity(Vector *vector);

void Vector_resize(Vector *vector, size_t new_size);

void Vector_set(Vector *vector, size_t index, void *elem);

void *Vector_get(Vector *vector, size_t index);

void Vector_insert(Vector *vector, size_t index, void *elem);

void Vector_delete(Vector *vector, size_t index);

void Vector_append(Vector *vector, void *elem);
```

实现`vector`的部分的重点在于`resize`函数的实现，要考虑扩容的问题，以及如何更新`size`和`capacity`。

* 初始`capacity`为10，`size`为0.
* 如果`new_size`大于`capacity`，那么就需要扩容，不断双倍扩容，直到`capacity`大于`new_size`。
* 如果`new_size`小于`capacity`，还需要考虑缩容的情况，如果`capacity > new_size * 2`，意味着容量过大，可以释放掉一部分空间，这时候不断缩小`capacity`，直到`capacity > new_size`，但是又不会超过`2 * new_size`。然后使用`realloc`缩小容量。特殊情况是可能存在`capacity < 10`的情况，为了保证`vector`初始预留空间，所以应该维持初始化时的空间大小，这个变量用`INITIAL_CAPACITY`来表示。
* 最后只剩下一种情况，就是`INITIAL_CAPACITY < new_size < capacity`，那么只需要和原来的`size`比较，看是否需要删除一部分元素。

两个函数`copy_constructor`和`destructor`需要在使用的时候用户自己定义，因为为了实现类似于C++的`vector`中的`template`效果，用`void *`来替代实现。

至于`document`的实现其实如果用C++来描述就是：有一个`vector`，存储的元素类型时`string`，新的一行相当于又插入了一个`string`。所以`document`的实现相对比较简单。

