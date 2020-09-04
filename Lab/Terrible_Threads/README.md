> # Terrible Threads

## Learning Objectives

- Synchronization primitives
- Common patterns in multithreaded programs
- Thread-safe data structures and their design

## Overview

There are three main components to this lab, two of which are graded. These are `Rendezvous` (not graded), `Semamore`, and Thread-safe Queue. Each of these represent very common synchronization problems (or slight twists on them) that will do you well to become familiar with.

Good luck!

## Rendezvous (UNGRADED)

This is a problem for you to think about. We have provided a worked solution to this problem but PLEASE try to solve this problem before looking at the solution!

Problem description:

Given two threads, a and b, and the fact that both have to run two tasks (a1, a2, b1, b2), how do you get both a1 and b1 to run before either a2 and b2? In `rendezvous.c`, you need to modify the two functions (``modifyB_printA & modifyA_printB`) using semaphores so that both quotes A and B are modified before being printed.

## `semamore.c`

NOTE: A `semamore ` is **NOT** a real thing! It is simply a made up clever name!

A normal semaphore blocks when the value within the semaphore reaches 0. A `semamore`  blocks when it reaches 0, but also blocks when it reaches some maximum value. You can think of a `semamore ` as a top-bounded semaphore. In `semamore.c`, you are given four functions to work on, `semm_init`, `semm_wait`, `semm_post`, `semm_destroy. semm_post` is the important difference. When `semm_post` reaches `max_val `(as defined in the `semamore` struct in `semamore.h`), it blocks.

There are four functions in total you will be writing.

### `semm_init`

Before attempting to write `semm_init`, please do read the `semamore.h` file which defines the `Semamore` struct. In `semm_init`, you are given a `Semamore` struct which has already been allocated (no need to directly use malloc here), a value, and a max value.

The value represents the exact same concept as a normal semaphore. The `max_val` represents the top bound as described earlier. Ensure here that all the values in the struct are properly set and initialized.

### `semm_wait`

Here you are passed a pointer to a `Semamore` struct. This function should behave like a normal semaphore. If the value is at 0 in the `Semamore` struct, you should block. If the value is currently not at 0, then we decrement it.

### `semm_post`

This function will specifically have different behavior than a normal semaphore. If the value is at `max_val` in the `Semamore` struct, then you need to block. If the value is currently not at `max_val`, then we increment it.

### `semm_destroy`

Here you are passed a `Semamore ` struct in which you want to clean up the data. You only have to cleanup members of the struct here, rather than the struct itself. Wherever `semm_destroy` is called by is responsible for freeing the `Semamore` struct itself. Any memory allocated in `semm_init` should be dealt with here. To reiterate, you do not need to free the `Semamore` struct itself.

## `queue.c`

**NOTE: Do not use semaphores or your `semamore` here.**

Your task is to build a thread safe queue, that also may or may not be bounded, by implementing the functions in `queue.c`. The `maxSize` of the queue can be set to either a positive number or a non-positive number. If positive, your queue will block if the user tries to push when the queue is full. If not positive, your queue should never block upon a push (the queue does not have a max size). Obviously, if your queue is empty then you should block on a pull. You should make use of the node struct to store and retrieve information. In the end, your queue implementation should be able to handle concurrent calls from multiple threads. `queue_init` and queue_destroy will not be called by multiple threads.

The queue is completely independent of the data that the user feeds it. The queue should not attempt to free this data, instead leaving that to the user of the queue.

### `queue_init`

NOTE: This will only ever be called by a single thread!

Before attempting to write `queue_init`, please do read the provided `queue.h` file which defines the queue_t struct. In `queue_init`, you are given a queue_t struct which has already been allocated (no need to directly use malloc here) and a `maxSize` for your queue.

The value of `maxSize` will change the behavior of your queue when it is positive vs non-positive as described above. Ensure that this is set correctly as well as initializing values within this queue_t struct.

### `queue_push`

NOTE: This can be called by multiple threads!

In `queue_push`, you are given a pointer to a `queue_t` struct as well as a data item. This function should take the given data, create a new node for it (a queue_node_t), and place it on the queue while following the rules we defined above. Specifically, if the queue is bounded (meaning you have a positive number as the `maxSize`) and the queue is full (at that `maxSize` value), then you should block.

You should be pushing the new node on in constant time. Be careful with the various cases you might run into when inserting the new node (think back to 125/225)! Ensure that you have updated the variables in queue_t properly!

### `queue_pull`

NOTE: This can be called by multiple threads!

In `queue_pull`, you are given a pointer to a `queue_t` struct. This function should take the queue and get the data located in the current head of the list, free and then update the head, and finally return the data while following the rules we defined above. Specifically, if the queue is empty, then you should block.

You should be getting data to be returned from your queue. Ensure that you have updated the variables in queue_t properly!

### queue_destroy

NOTE: This will only ever be called by a single thread!

Here you receive a pointer to a `queue_t` struct. You do not have to free the `queue_t` struct itself, but you must cleanup the items for which an allocation was made within the struct here. Be sure you have freed all memory you allocated. To reiterate, do not free the queue_t struct itself, just anything that required an allocation in `queue_init`.

## Testing

Since the implementation of your `semamore` is quite close to an actual semaphore, please test this on your own in a variety of ways. Be careful of race conditions! They can be hard to find!

You will find a file called `testqueue.c` in the lab. We would like you to write your own test cases by applying your queue to the same problem solved in `testqueue.c`. This by no means an exhaustive test, just an example. Further testing is required in order to be assured of the correctness of your queue. This is ungraded, but it is highly recommended.

Helpful Hints and Notes

- Make sure you thoroughly test both your `Semamore` and Queue! Race conditions can be hard to spot!
- Attempting to visualize your code or diagram it in certain cases can sometimes be a huge aid and is highly recommended!

-------

这个作业往年也叫`luscious_locks`或`critical_concurrency`，考察的方法是一致的。

## Rendezvous 

第一个任务是修改`rendezvous.c`里的程序，使用`semaphore`，两个线程分别执行`a1,a2`和`b1, b2`的任务，现在需要保证`a1, b1`在`a2, b2`前执行。

初看被下面这一大堆莫名奇妙的字符确实让人摸不着头发，但是看了`modifyA_printB`的函数知道每个字符字母序减1。

```
"J!epo(u!lopx!ipx!nboz!pg!zpv!ibwf!fwfs!nfu!Ejkltusb-!cvu!"
"zpv!qspcbcmz!lopx!uibu!bssphbodf!jo!dpnqvufs!tdjfodf!jt!"
"nfbtvsfe!jo!obop.Ejkltusbt/!.!Bmbo!Lbz"
```

转换完为：

```
I don't know how many of you have ever met Dijkstra, but
you probably know that arrogance in computer science is
measured in nano-Dijkstras. - Alan Kay
```

这个题目非常类似于在`LeetCode`上的第一个`easy`题，需要让线程的执行顺序按照某种确定的方式运行。这里一个线程去修改（还原到需要打印的字符），并在修改完成后打印，具体来说，`modifyA_printB`是还原A后打印B的内容，相应的，`modifyB_printA`是还原B的内容后打印A的内容。

之所以需要信号量是因为，如果我们不做一些限制，很有可能`modifyA_printB`执行的快一些，已经还原了A，但是此时B还没有还原完成，所以此时打印很可能打印的是不完整的B。

那么我们完全可以用两个信号量去标识是否完成。注意`sem_post`和`sem_wait`的顺序不能反过来，因为如果反过来就造成了死锁。

最后的输出结果：

```
quote_B:        The question of whether computers can think is like the question of whether submarines can swim.” - Edsger W. Dijkstra
quote_A:        I don't know how many of you have ever met Dijkstra, but you probably know that arrogance in computer science is measured in nano-Dijkstras. - Alan Kay
```

## `semamore`

`semamore`并不是一个真实的库，而是希望我们根据`semamore.h`自己手写来去实现`semaphore`里面的`sem_init, sem_post, sem_wait, sem_destroy`函数。

```c
#include <pthread.h>

/**
 * The struct for a Semamore.
 * It contains the value the Semaphore is initially, as well as its maximum
 * value (before it should block).
 * Also contains an associated mutex and condition variable.
 */

typedef struct {

    int value, max_val;

    pthread_mutex_t m;
    pthread_cond_t cv;

} Semamore;

// Initializes the members of the Semamore struct.
void semm_init(Semamore *s, int value, int max_val);

// Blocks when value is at 0, then decrememnts the value once it is not at 0.
void semm_wait(Semamore *s);

// Blocks when value is at max_val, then increments the value once it is not at
// max_val.
void semm_post(Semamore *s);

// Cleans up associated memory with the struct, but does not free the struct
// itself.
void semm_destroy(Semamore *s);
```

可以借助条件变量和互斥量来实现，测试程序就利用`LeetCode`的`1114.Print in Order`来进行测试。













