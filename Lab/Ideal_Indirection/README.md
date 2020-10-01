> # Ideal_indirection

## Learning Objectives

- Virtual memory
- The virtual address translation process
- Memory Management Unit (MMU)
- Page Tables
- Translation Lookaside Buffer (TLB)

## Required Reading

Read this in the `wikibook` before starting the lab.

- [Introduction to Virtual Memory](https://github.com/angrave/SystemProgramming/wiki/Virtual-Memory%2C-Part-1%3A-Introduction-to-Virtual-Memory)

## Overview

This lab simulates the behavior of virtual memory for multiple processes. Before getting started, we highly recommend that you have read the `wikibook` and answer the [Google Form](http://goo.gl/forms/4Us4vBPJnj). Next, please read through the provided code as many files have comments on usage that you will find useful. There are a few different files in this lab that each represent a different component important to virtual memory. Reading through these files can start to help one understand the roles of the different hardware and software involved with virtual memory.

You will only have to write one function in `mmu.c`, but it requires a good understand of the `wikibook` and decent knowledge of the rest of the provided code.

Good luck!

## `MMU.c`

This is the function you are to complete in `mmu.c`:

```c
void *MMU_get_physical_address(MMU *mmu, void *virtual_address, size_t pid)
```

This should convert a virtual address to a physical address from a three level page table.

`mmu` here is a pointer to the MMU struct defined in `mmu.h`, virtual_address is the virtual address you are transforming into a physical address, and `pid` is the ID of this process.

From the `wikibook`: “For most memory accesses of most programs, there is a significant chance that the TLB has cached the results… the TLB [may] not have the result cached and the MMU must use the much slower page table to determine the physical frame.”

Thus for any virtual address, you should check whether the result has been already cached in the TLB (see `tlb.h`). Otherwise you must search the page tables.

Virtual addresses use 64 bits with only 51 bits used for the actual virtual address as you can see from the diagram below. MISC is not counted as part of the virtual address here. In this simulation, the virtual address is broken up as follows for a three level page table.

```
|----------|---------|---------|---------|-----------|
  MISC (13) VPN1 (12) VPN2 (12) VPN3 (12) Offset (15)
```

Using `PageTable_get_entry()` (see `pagetable.h`) will allow you to traverse through the page table using the `VPNx` addresses. Consider what should be done if the page table or frame we are trying to access has not yet been created.

Be sure to read all of the functions within `mmu.h`, paying close attention to the comments to see the circumstances in which these functions should be called.

NOTE: You must use the `PageTable_get_entry` and `PageTable_set_entry` functions rather than directly accessing the `PageTable` struct. The same goes for the MMU with modifying the `num_tlb_misses` and `num_page_faults`. These should be done using the provided functions.

## Running the simulator

You can run the simulator with the following command (see `simulator.c`):

```bash
./simulator <num_processes> <num_addresses> <num_iterations> <random seed>
```

## Testing

- Make sure you thoroughly test your code as usual. We have provided some tests cases, but we encourage you to write your own as well. Use the provided test cases as a reference to learn to create tests with good coverage.

------

这个`lab`应该是到目前为止遇到的比较复杂的一个。`lab`的目的是为了模拟`TLB`和`MMU`的工作原理，核心程序为`tlb.c, mmu.c`，对于程序整体的理解需要通过`simulator.c`。

首先`simulator.c`需要传入4个参数

```
./simulator <num_processes> <num_addresses> <num_iterations> <random seed>
```

`num_processes`：进程的数量

`num_addresses`：一个进程处理的任务包含多个虚拟地址

`num_iterations`：进程迭代多次，来模拟时间片轮转

`random seed`：随机数种子，用来模拟产生`pid`

对于多个进程的模拟通过多线程来实现，首先通过`random seed`来产生`pid`和虚拟地址，并通过`get_physical_addresses`函数得到虚拟地址对应的物理地址。在生成每个物理地址的时候，通过互斥量来保证线程安全，通过函数`ask_mmu_for_physical_address`来获取物理地址。`ask_mmu_for_physical_address`函数包含了一些打印的信息，核心是通过`MMU_get_physical_address`来获得物理地址，通过传入两个重要的参数`virtual_address`和`pid`来获得最终需要的物理地址。

`MMU_get_physical_address`在`mmu.c`文件内，也是我们需要去修改和完善的函数。完成这个函数需要去理解三级页表和TLB。在`mmu.h`内定义的`MAX_PROCESS_ID`和`PAGE_NUMBER_LENGTH`，32768是$2^{15}$，意味着我们需要15个bit作为`offset`，然后每级页表需要12个bit。这样总共需要51个bit，为了存储和运算的方便，采用64bit来存储，所以高位的13个bit在实验中不会起作用。注意生成`virtual_address`的代码：

```c
char *ptr = NULL;
process -> virtual_addresses[j] = ptr + (rand() % ((size_t)1 << VIRTUAL_ADDRESS_LENGTH));
```

代码保证了最后`virtual_address`只有后51位起作用。然后是对于TLB的理解，TLB在本实验里采用单向链表来实现，策略是LRU，意味着当TLB达到最大容量的时候，删除的是最近最不常的数据，也就是链表末尾的数据。查找过程就是线性查找，因为在`tlb.h`内定义了`MAX_NODES`为16，也就是`tlb`内最多存储16个数据，所以线性查找的性能可以认为是$O(1)$的。这里需要先看一下`TLB`是如何被定义的：

```c
typedef struct TLB {
    // The key is the virtual address with the offset removed.
    // This is because all virtual addresses with the same 3 virtual page numbers
    // will map to the same frame in memory.
    void *key;
    // The value is the beginning of the frame that corresponds to the key
    // provided.
    void *value;
    size_t pid;
    struct TLB *next;
} TLB;
```

键是`key`（也就是`virtual_address`去掉后15位，在`mmu.h`内也解释了，如果三级页表号相同，对应的是相同的帧`frame`）和`pid`，值是`value`，也就是物理地址。所以首先就是提取出三级页表号得到`key`，通过位运算来实现，首先和`0x7FFFFFFFFFFFF`进行与运算，提取出后51位，然后右移去掉`offset`的15位，这样就得到了三级页号作为`key`，然后通过`TLB_get_physical_address`去查找对应的物理地址是否存在于TLB中。

在`MMU_get_physical_address`里，有`uintptr_t`类型，这个类型包含在头文件`stdint.h`内：

```c
/* Types for `void *' pointers.  */
#if __WORDSIZE == 64
# ifndef __intptr_t_defined
typedef long int		intptr_t;
#  define __intptr_t_defined
# endif
typedef unsigned long int	uintptr_t;
#else
# ifndef __intptr_t_defined
typedef int			intptr_t;
#  define __intptr_t_defined
# endif
typedef unsigned int		uintptr_t;
#endif
```

意味着当机器是64位的时候，`uintptr_t`是`unsigned long int`，如果是32位机器，则代表`unsigned int`。

关注下`TLB_get_physical_address`的原理，这个部分的实现感觉有些奇怪，利用快慢指针，其实完全可以把判断条件的部分携程`while (fast && fast -> next)`即可，等价于`slow`和`fast`。就是一个线性查找，函数比较好理解。

这样通过`TLB_get_physical_address`我们得到了一个`physical_address`，如果`physical_address`是空指针的话，意味着在TLB里并不存在，出现一次`tlb miss`，通过函数`MMU_tlb_miss`来记录一下。这时不得不在页表里查找了，在页表里查找的过程为，首先通过`pid`得到一个一级页表的指针，也就是第一级的`entry`，指针指向的是一个页表的起始位置，先前通过位运算提取出`vpn1, vpn2, vpn3`，也就是在每级页表内的`offset`，可能存在的情况就是对应的页表不存在，发生缺页的情况，此时需要调用`PageTable_create`生成页表，并通过`PageTable_set_entry`设定`entry`，另外通过`MMU_raise_page_fault`记录缺页情况。注意`idx3`指向的是`frame`的起始地址，如果不存在，需要调用`kernel.c`里的`ask_kernel_for_frame`来申请一块和`frame`大小相同的内存，将起始地址赋值给`idx3`，然后通过`offset`得到物理地址，记得此时要将这个不存在于TLB的地址通过`TLB_add_physical_address`加入。

































