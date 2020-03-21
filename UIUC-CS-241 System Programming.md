> # 	UIUC-CS-241: System Programming

Textbook: [WikiBook](https://github.com/angrave/SystemProgramming/wiki)

Course website: <http://cs241.cs.illinois.edu/schedule.html>

Labs: <https://courses.engr.illinois.edu/cs241/sp2016/labs.html>

MPs: <https://courses.engr.illinois.edu/cs241/sp2016/mps.html>

# Week 1

[HW0](https://github.com/angrave/SystemProgramming/wiki/HW0)

[Lab: Know Your Tools](https://courses.engr.illinois.edu/cs241/sp2016/know_your_tools.html)

## 1.System Programming

开始之前要先完成HW0。

## 2.How to crash in C

[Lecture: Intro to C Notes](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-1%3A-Introduction)

**How do you print out pointer values? integers? strings?**

Use format specifiers "%p" for pointers, "%d" for integers and "%s" for Strings. A full list of all of the format specifiers is found [here](http://www.cplusplus.com/reference/cstdio/printf/)

Example of integer:

```c
int num1 = 10;
printf("%d", num1); //prints num1
```

Example of integer pointer:

```c
int *ptr = (int *) malloc(sizeof(int));
*ptr = 10;
printf("%p\n", ptr); //prints the address pointed to by the pointer
printf("%p\n", &ptr); /*prints the address of pointer -- extremely useful
when dealing with double pointers*/
printf("%d", *ptr); //prints the integer content of ptr
return 0;
```

Example of string:

```c
char *str = (char *) malloc(256 * sizeof(char));
strcpy(str, "Hello there!");
printf("%p\n", str); // print the address in the heap
printf("%s", str);
return 0;
```

**What's the difference between a pointer and an array? Give an example of something you can do with one but not the other.**

```c
char ary[] = "Hello";
char *ptr = "Hello";
```

Example

The array name points to the first byte of the array. Both `ary` and `ptr` can be printed out:

```c
char ary[] = "Hello";
char *ptr = "Hello";
// Print out address and contents
printf("%p : %s\n", ary, ary);
printf("%p : %s\n", ptr, ptr);
```

The array is mutable, so we can change its contents (be careful not to write bytes beyond the end of the array though). Fortunately, 'World' is no longer than 'Hello"

In this case, the char pointer `ptr` points to some read-only memory (where the statically allocated string literal is stored), so we cannot change those contents.

```c
strcpy(ary, "World"); // OK
strcpy(ptr, "World"); // NOT OK - Segmentation fault (crashes)
```

We can, however, unlike the array, we change `ptr` to point to another piece of memory,

```c
ptr = "World"; // OK!
ptr = ary; // OK!
ary = (..anything..) ; // WONT COMPILE
// ary is doomed to always refer to the original array.
printf("%p : %s\n", ptr, ptr);
strcpy(ptr, "World"); // OK because now ptr is pointing to mutable memory (the array)
```

What to take away from this is that pointers * can point to any type of memory while C arrays [] can only point to memory on the stack. In a more common case, pointers will point to heap memory in which case the memory referred to by the pointer CAN be modified.

**`sizeof()` returns the number of bytes. So using above code, what is sizeof(ary) and sizeof(ptr)?**

`sizeof(ary)`: `ary` is an array. Returns the number of bytes required for the entire array (5 chars + zero byte = 6 bytes) `sizeof(ptr)`: Same as sizeof(char *). Returns the number bytes required for a pointer (e.g. 4 or 8 for a 32 bit or 64-bit machine)

`sizeof` is a special operator. Really it's something the compiler substitutes in before compiling the program because the size of all types is known at compile time. When you have `sizeof(char*)` that takes the size of a pointer on your machine (8 bytes for a 64-bit machine and 4 for a 32 bit and so on). When you try `sizeof(char[])`, the compiler looks at that and substitutes the number of bytes that the **entire** array contains because the total size of the array is known at compile time.

```c
char str1[] = "will be 11";
char* str2 = "will be 8";
sizeof(str1) //11 because it is an array
sizeof(str2) //8 because it is a pointer
```

Be careful, using sizeof for the length of a string!

**Which of the following code is incorrect or correct and why?**

```c
int* f1(int *p) {
    *p = 42;
    return p;
} // This code is correct;

char* f2() {
    char p[] = "Hello";
    return p;
} // Incorrect!
```

Explanation: An array p is created on the stack for the correct size to hold H,e,l,l,o, and a null byte i.e. (6) bytes. This array is stored on the stack and is invalid after we return from f2.

```c
char* f3() {
    char *p = "Hello";
    return p;
} // OK
```

Explanation: p is a pointer. It holds the address of the string constant. The string constant is unchanged and valid even after f3 returns.

```c
char* f4() {
    static char p[] = "Hello";
    return p;
} // OK
```

Explanation: The array is static meaning it exists for the lifetime of the process (static variables are not on the heap or the stack).

**How do you allocate memory on the heap?**

Use malloc. There's also realloc and calloc. Typically used with sizeof. e.g. enough space to hold 10 integers

```c
int *space = malloc(sizeof(int) * 10);
```

**What's wrong with this string copy code?**

```c
void mystrcpy(char*dest, char* src) { 
    // void means no return value   
    while( *src ) { dest = src; src ++; dest++; }  
}
```

In the above code it simply changes the dest pointer to point to source string. Also the nuls bytes are not copied. Here's a better version -

```c
while( *src ) { *dest = *src; src ++; dest++; } 
*dest = *src;
```

Note it's also usual to see the following kind of implementation, which does everything inside the expression test, including copying the nul byte.

```c
  while((*dest++ = *src++ )) {};
```

**How do you write a strdup replacement?**

```c
// Use strlen+1 to find the zero byte... 
char* mystrdup(char*source) {
    char *p = (char *) malloc ( strlen(source)+1 );
    strcpy(p,source);
    return p;
}
```

**How do you unallocate memory on the heap?**

```c
int *n = (int *) malloc(sizeof(int));
*n = 10;
//Do some work
free(n);
```

**What is double free error? How can you avoid? What is a dangling pointer? How do you avoid?**

A double free error is when you accidentally attempt to free the same allocation twice.

```c
int *p = malloc(sizeof(int));
free(p);

*p = 123; // Oops! - Dangling pointer! Writing to memory we don't own anymore

free(p); // Oops! - Double free!
```

The fix is first to write correct programs! Secondly, it's good programming hygiene to reset pointers once the memory has been freed. This ensures the pointer can't be used incorrectly without the program crashing.

Fix:

```c
p = NULL; // Now you can't use this pointer by mistake
```

**What is 'typedef' and how do you use it?**

Declares an alias for a type. Often used with structs to reduce the visual clutter of having to write 'struct' as part of the type.

```c
typedef float real; 
real gravity = 10;
// Also typedef gives us an abstraction over the underlying type used. 
// In the future, we only need to change this typedef if we
// wanted our physics library to use doubles instead of floats.

typedef struct link link_t; 
//With structs, include the keyword 'struct' as part of the original types
```

In this class, we regularly typedef functions. A typedef for a function can be this for example

```c
typedef int (*comparator)(void*,void*);

int greater_than(void* a, void* b){
    return a > b;
}
comparator gt = greater_than;
```

This declares a function type comparator that accepts two `void*` params and returns an integer.



# Week 2

[Lab: Extreme Edge Cases](https://courses.engr.illinois.edu/cs241/sp2016/extreme_edge_cases)

[MP: Pointers Gone Wild](https://courses.engr.illinois.edu/cs241/sp2016/pointers_gone_wild)

## 3.C Crash Course 2

[Lecture: Wikibook C Gotchas](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas)

**String Constants are Read-Only**

```c
char array[] = "Hi!"; // array contains a mutable copy 
strcpy(array, "OK");

char *ptr = "Can't change me"; // ptr points to some immutable memory
strcpy(ptr, "Will not work");
```

String literals are character arrays stored in the code segment of the program, which is immutable. Two string literals may share the same space in memory. An example follows:

```c
char *str1 = "Brandon Chong is the best TA";
char *str2 = "Brandon Chong is the best TA";
str1 == str2; // true
```

The strings pointed to by `str1` and `str2` may actually reside in the same location in memory.

Char arrays, however, contain deep copies of string literals assigned to them, and different arrays represent different memory locations. These following char arrays do not reside in the same place in memory.

```c
char arr1[] = "Brandon Chong didn't write this";
char arr2[] = "Brandon Chong didn't write this";
arr1 == arr2;         // false
&arr1[0] == &arr2[0]; // false
```

**Buffer Overflow / Underflow**

```c
int i = 10, array[10];
for (; i >= 0; i--) array[i] = i;
```

C does not perform bounds-checking on array accesses. The above example writes into `array[10]` which is outside the array bounds. This can corrupt other variables on the stack as well as the very implementation of the call stack, exposing your program to attacks from hackers. In practice, these overflows often result from using unsafe library calls or putting the wrong size limit on a safer library call.

```c
gets(array); // Let's hope the input is shorter than my array! (NEVER use gets)
fgets(array, 4096, stdin); // Whoops 
```

**Handling Pointers to Out-of-Scope Automatic Variables**

```c
int *f() {
    int result = 42;
    static int imok;
    int *p;
    {
      int x = result;
      p = &x;
    }
    //imok = *p;      // Not OK: x has already gone out of scope
    //return &result; // Not OK: result will go out of scope after the function returns 
    return &imok;     // OK - static variables are not on the stack
}
```

Automatic variables are bound to stack memory only as long as they are in scope. After they go out of scope, the data stored at their memory addresses becomes undefined. Static variables reside in the data segment, which is safe to access even when those variables are not in scope.

**`sizeof(type *)` versus `sizeof(type)`**

```c
struct User {
   char name[100];
};
typedef struct User user_t;

user_t *user = (user_t *) malloc(sizeof (user_t *));
```

In the above example, we needed to allocate enough bytes for the struct. Instead, we allocated enough bytes to hold a pointer. Writing to the user pointer might corrupt the heap. The correct code is shown below.

```c
struct User {
   char name[100];
};
typedef struct User user_t;

user_t *user = (user_t *) malloc(sizeof (user_t));
```

**Strings Require `strlen(s)+1` Bytes**

Every string must have a null byte after the last actual character. To store the string `"Hi"` it takes 3 bytes: `[H] [i] [\0]`.

```c
char *strdup(const char *input) {     /* return a copy of 'input' */
    char *copy;
    copy = malloc(sizeof (char *));   /* nope! this allocates space for a pointer, not a string */
    copy = malloc(strlen(input));     /* Almost...but what about the null terminator? */
    copy = malloc(strlen(input) + 1); /* That's right. */
    strcpy(copy, input);              /* strcpy will provide the null terminator */
    return copy;
}
```

**Failing to Initialize Memory**

```
void myfunct() {
    char array[10];
    char *p = malloc(10);
    printf("%s %s\n", array, p);
}
```

Automatic (stack) variables and heap memory obtained with `malloc` are not initialized to zero by default. The function above results in undefined behavior.

**Dangling Pointers**

```c
char *p = malloc(10);
strcpy(p, "Hello");
free(p);
//  .. later ...
strcpy(p,"World"); 
```

Accessing freed memory results in undefined behavior. A defensive programming practice is to set pointers to NULL as soon as the memory is freed, since there is no other way to reliably check if a pointer points to a valid address. The following macro accomplishes this.

```c
#define safer_free(p) {free(p); (p) = NULL;}
```

**Forgetting to Copy `getline` Buffer**

```c
#include <stdio.h>

int main(void){
    char *line = NULL;
    size_t linecap = 0;
    char *strings[3];

    // assume stdin contains "1\n2\n\3\n"
    for (size_t i = 0; i < 3; ++i)
        strings[i] = getline(&line, &linecap, stdin) >= 0 ? line : "";

    // this prints out "3\n3\n\3" instead of "3\n\2\n1\n"
    for (size_t i = 3; i--;) // i=2,1,0
        printf("%s", strings[i]);
}
```

Since `getline` reuses a buffer, all pointers in the `strings` array are actually pointing to the same memory. We can fix this by setting the assignment of `strings[i]` to a deep copy of the buffer.

```c
strings[i] = getline(&line, &linecap, stdin) >= 0 ? strdup(line) : "";
```

Fun fact: providing "1\n123456789abcdef\n3\n" to the broken version of the program might cause it to print out "3\n3\n1\n" instead of "3\n3\n3\n". Now why might that be? *Hint: use Valgrind*

```c
 #include <stdio.h>
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
```

`getline()` reads an entire line from stream, storing the address of the buffer containing the text into `*lineptr`.  The buffer is null-terminated and includes the newline character, if one was found. 
If `*lineptr`  is set to NULL and `*n` is set 0 before the call, then `getline()` will allocate a buffer for storing the  line.   This  buffer  should  be freed by the user program even if `getline()` failed.
Alternatively,  before  calling  `getline()`,  `*lineptr` can contain a pointer to a malloc(3)-allocated buffer `*n` bytes in size.  If the buffer is not large enough to hold the line, `getline()` resizes it with `realloc(3)`, updating `*lineptr` and `*n` as necessary. 
In either case, on a successful call, `*lineptr` and `*n` will be updated to reflect the buffer address and allocated size respectively. 

**C Preprocessor macros and precedence**

Preprocessing is an operation performed **before** the program is actually compiled. It is nothing but text substitution, i.e. copy-and-paste. Consider the following code.

```c
#define BADD(x,y) x+y
char buffer[BADD(5,5)*2];
```

After preprocessing, the code will look exactly like this.

```c
char buffer[5+5*2];
```

Notice that the buffer takes up 15 bytes instead of 20, since multiplication has higher precedence than addition. To prevent this kind of behavior, make sure to surround all macro variables as well as the entire macro expression with parentheses.

```c
#define ADD(x,y) ((x)+(y))
```

**C Preprocessor Macros and Side-Effects**

```c
#define min(a,b) ((a)<(b) ? (a) : (b))
int x = 4;
if (min(x++, 100)) printf("%d is six", x);
```

The conditional expression effectively expands to `x++ < 100 ? x++ : 100`, which results in `x` being incremented twice. There is no good way to prevent these kinds of side effects when using Standard C macros. But [GNU C](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html) provides some workarounds (only works when using GCC).

**`sizeof` and Side-Effects**

```c
int a = 0;
size_t size = sizeof(a++);
printf("size: %lu, a: %d", size, a);
```

This code prints out the following.

```c
size: 4, a: 0
```

The expression passed into `sizeof` is not actually evaluated at runtime in most cases, since the type (hence the size) of the expression can be calculated at compile time. Though there are exceptions in the case of [C99 variable-length arrays](http://port70.net/~nsz/c/c11/n1570.html#6.5.3.4p2), since their sizes are determined at runtime.

## 4.C Crash Course 3

[Lecture: Wikibook Input and Output](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-2%3A-Text-Input-And-Output)

**How else can I print strings and single characters?**

Use `puts( name )` and `putchar( c )` where name is a pointer to a C string and c is just a `char`

**How do I print to other file streams?**

Use `fprintf( _file_ , "Hello %s, score: %d", name, score);` Where _file_ is either predefined 'stdout' 'stderr' or a FILE pointer that was returned by `fopen` or `fdopen`

**Can I use file descriptors?**

Yes! Just use `dprintf(int fd, char* format_string, ...);` Just remember the stream may be buffered, so you will need to assure that the data is written to the file descriptor.

**How do I print data into a C string?**

Use `sprintf` or better `snprintf`.

```c
char result[200];
int len = snprintf(result, sizeof(result), "%s:%d", name, score);
```

The return value of snprintf is the length that would have been written given enough space, excluding the ending NULL byte.

```c
char x[5];
int size = snprintf(x, 5, "%s%s%s", "12", "34", "56"); // writes "1234" + null
printf("%d\n", size); // output 6
```

Source: [this StackOverflow post](https://stackoverflow.com/questions/12746885/why-use-asprintf) and man page.

**What if I really really want `printf` to call `write` without a newline?**

Use `fflush( FILE* inp )`. The contents of the file will be written. If I wanted to write "Hello World" with no newline, I could write it like this.

```c
int main(){
    fprintf(stdout, "Hello World");
    fflush(stdout);
    return 0;
}
```

**How is `perror` helpful?**

Let's say that you have a function call that just failed (because you checked the man page and it is a failing return code). `perror(const char* message)` will print the English version of the error to stderr

```c
int main(){
    int ret = open("IDoNotExist.txt", O_RDONLY);
    if(ret < 0){
        perror("Opening IDoNotExist:");
    }
    //...
    return 0;
}
```

**How do I parse numbers from strings?**

Use `long int strtol(const char *nptr, char **endptr, int base);` or `long long int strtoll(const char *nptr, char **endptr, int base);`.

What these functions do is take the pointer to your string `*nptr` and a `base` (ie binary, octal, decimal, hexadecimal etc) and an optional pointer `endptr` and returns a parsed value.

```c
int main(){
    const char *nptr = "1A2436";
    char* endptr;
    long int result = strtol(nptr, &endptr, 16);
    return 0;
}
```

Be careful though! Error handling is tricky because the function won't return an error code. If you give it a string that is not a number it will return 0. This means you cant differentiate between a valid "0" and an invalid string. See the man page for more details on strol behavior with invalid and out of bounds values. A safer alternative is use to `sscanf` (and check the return value).

```c
int main(){
    const char *input = "0"; // or "!##@" or ""
    char* endptr;
    long int parsed = strtol(input, &endptr, 10);
    if(parsed == 0){
        // Either the input string was not a valid base-10 number or it really was zero!
    }
    return 0;
}
```

**How do I parse input using `scanf` into parameters?**

Use `scanf` (or `fscanf` or `sscanf`) to get input from the default input stream, an arbitrary file stream or a C string respectively. It's a good idea to check the return value to see how many items were parsed. `scanf` functions require valid pointers. It's a common source of error to pass in an incorrect pointer value. For example,

```c
int *data = (int *) malloc(sizeof(int));
char *line = "v 10";
char type;
// Good practice: Check scanf parsed the line and read two values:
int ok = 2 == sscanf(line, "%c %d", &type, &data); // pointer error
```

We wanted to write the character value into c and the integer value into the malloc'd memory. However, we passed the address of the data pointer, not what the pointer is pointing to! So `sscanf` will change the pointer itself. i.e. the pointer will now point to address 10 so this code will later fail e.g. when free(data) is called.

**Why is `gets` dangerous? What should I use instead?**

The following code is vulnerable to buffer overflow. It assumes or trusts that the input line will be no more than 10 characters, including the terminating byte.

```c
char buf[10];
gets(buf); // Remember the array name means the first byte of the array
```

`gets` is deprecated in C99 standard and has been removed from the latest C standard (C11). Programs should use `fgets` or `getline` instead.

Where each has the following structure respectively:

```c
char *fgets (char *str, int num, FILE *stream); 

ssize_t getline(char **lineptr, size_t *n, FILE *stream);
```

Here's a simple, safe way to read a single line. Lines longer than 9 characters will be truncated:

```c
char buffer[10];
char *result = fgets(buffer, sizeof(buffer), stdin);
```

The result is NULL if there was an error or the end of the file is reached. Note, unlike `gets`, `fgets` copies the newline into the buffer, which you may want to discard-

```c
if (!result) { return; /* no data - don't read the buffer contents */}

int i = strlen(buffer) - 1;
if (buffer[i] == '\n') 
    buffer[i] = '\0';
```

**How do I use `getline`?**

```c
// ssize_t getline(char **lineptr, size_t *n, FILE *stream);

/* set buffer and size to 0; they will be changed by getline */
char *buffer = NULL;
size_t size = 0;

ssize_t chars = getline(&buffer, &size, stdin);

// Discard newline character if it is present,
if (chars > 0 && buffer[chars-1] == '\n') 
    buffer[chars-1] = '\0';

// Read another line.
// The existing buffer will be re-used, or, if necessary,
// It will be `free`'d and a new larger buffer will `malloc`'d
chars = getline(&buffer, &size, stdin);

// Later... don't forget to free the buffer!
free(buffer);
```

## 5. fork and waitpid

[Lecture: Wikibook forking](https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-1%3A-Introduction)

**What does fork do?**

The `fork` system call clones the current process to create a new process. It creates a new process (the child process) by duplicating the state of the existing process with a few minor differences (discussed below). The child process does not start from main. Instead it returns from `fork()` just as the parent process does.

Just as a side remark, in older UNIX systems, the entire address space of the parent process was directly copied (regardless of whether the resource was modified or not). These days, kernel performs [copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write), which saves a lot of resources, while being very time efficient.

**What is the simplest `fork()` example?**

```c
#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("I'm printed once!\n");
    fork();
    // Now there are two processes running
    // and each process will print out the next line.
    printf("You see this line twice!\n");
}
```

```bash
I'm printed once!
You see this line twice!
You see this line twice!
```

**Why does this example print 42 twice?**

he following program prints out 42 twice - but the `fork()` is after the `printf`!? Why?

```c
#include <unistd.h> /*fork declared here*/
#include <stdio.h> /* printf declared here*/
int main() {
    int answer = 84 >> 1;
    printf("Answer: %d", answer);
    fork();
    return 0;
}
```

```bash
Answer: 42Answer: 42
```

The `printf` line *is* executed only once however notice that the printed contents is not flushed to standard out (there's no newline printed, we didn't call `fflush`, or change the buffering mode). The output text is therefore still in process memory waiting to be sent. When `fork()` is executed the entire process memory is duplicated including the buffer. Thus the child process starts with a non-empty output buffer which will be flushed when the program exits.

If we change the program like in the following, the program only prints 42 once.

```c++
#include <unistd.h> /*fork declared here*/
#include <stdio.h> /* printf declared here*/
int main() {
    int answer = 84 >> 1;
    printf("Answer: %d\n", answer);
    fork();
    return 0;
}
```

```bash
Answer: 42
```

**How do you write code that is different for the parent and child process?**

Check the return value of `fork()`.

If fork() returns -1, that implies something went wrong in the process of creating a new child. One should check the value stored in *errno* to determine what kind of error occurred; commons one include EAGAIN and ENOMEM (check [this page](http://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Errors/unix_system_errors.html) to get a description of the errors).

Similarly, a return value of 0 indicates that we are in the child process, while a positive integer shows that we are in parent process. The positive value returned by fork() gives as the process id (*pid*) of the child.

Here's one way to remember which is which:

The child process can find its parent - the original process that was duplicated - by calling `getppid()` - so does not need any additional return information from `fork()`. The parent process however can only find out the id of the new child process from the return value of `fork`:

```c
pid_t id = fork();
if (id == -1) exit(1); // fork failed 
if (id > 0)
{ 
    // I'm the original parent and 
    // I just created a child process with id 'id'
    // Use waitpid to wait for the child to finish
} else { // returned zero
    // I must be the newly made child process
}
```

**What is a fork bomb ?**

A 'fork bomb' is when you attempt to create an infinite number of processes. A simple example is shown below:

```c
while (1) fork();
```

This will often bring a system to a near-standstill as it attempts to allocate CPU time and memory to a very large number of processes that are ready to run. Comment: System administrators don't like fork-bombs and may set upper limits on the number of processes each user can have or may revoke login rights because it creates a disturbance in the force for other users' programs. You can also limit the number of child processes created by using `setrlimit()`.

fork bombs are not necessarily malicious - they occasionally occur due to student coding errors.

**How does the parent process wait for the child to finish?**

Use `waitpid` (or `wait`).

```c
pid_t child_id = fork();
if (child_id == -1) { perror("fork"); exit(EXIT_FAILURE);}
if (child_id > 0) { 
    // We have a child! Get their exit code
    int status; 
    waitpid( child_id, &status, 0 );
    // code not shown to get exit status from child
} else { // In child ...
    // start calculation
    exit(123);
}
```

**Can I make the child process execute another program?**

Yes. Use one of the [`exec`](http://man7.org/linux/man-pages/man3/exec.3.html) functions after forking. The `exec` set of functions replaces the process image with the the process image of what is being called. This means that any lines of code after the `exec` call are replaced. Any other work you want the child process to do should be done before the `exec` call.

```c
#include <unistd.h>
#include <sys/types.h> /* pid_t declare here */
#include <sys/wait.h> /* waitpid() declare here */
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char**argv) {
    pid_t child = fork();
    if (child == -1) return EXIT_FAILURE;
    if (child) { /* I have a child! */
        printf("The parent process begins.\n");
        int status;
        waitpid(child , &status ,0);
        printf("The child process ends.\n");
        return EXIT_SUCCESS;

    } else { /* I am the child */
        // Other versions of exec pass in arguments as arrays
        // Remember first arg is the program name
        // Last arg must be a char pointer to NULL
		printf("At present, this is child process\n");
        execl("/bin/ls", "ls","-alh", (char *) NULL);

        // If we get to this line, something went wrong!
        perror("exec failed!");
    }
}
```

**A simpler way to execute another program**

```c
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char**argv) {
    system("ls");
    return 0;
}
```

The `system` call will fork, execute the command passed by parameter and the original parent process will wait for this to finish. This also means that `system` is a blocking call: The parent process can't continue until the process started by `system` exits. This may or may not be useful. Also, `system` actually creates a shell which is then given the string, which is more overhead than just using `exec` directly. The standard shell will use the `PATH` environment variable to search for a filename that matches the command. Using system will usually be sufficient for many simple run-this-command problems but can quickly become limiting for more complex or subtle problems, and it hides the mechanics of the fork-exec-wait pattern so we encourage you to learn and use `fork` `exec` and `waitpid` instead.

**What is different in the child process than the parent process?**

The key differences include:

- The process id returned by `getpid()`. The parent process id returned by `getppid()`.
- The parent is notified via a signal, SIGCHLD, when the child process finishes but not vice versa.
- The child does not inherit pending signals or timer alarms. For a complete list see the [fork man page](http://man7.org/linux/man-pages/man2/fork.2.html)

**Do child processes share open filehandles?**

Yes! In fact both processes use the same underlying kernel file descriptor. For example if one process rewinds the random access position back to the beginning of the file, then both processes are affected.

Both child and parent should `close` (or `fclose`) their file descriptors or file handle respectively.

# Week 3

[Lab: Utilities Unleashed](https://courses.engr.illinois.edu/cs241/sp2016/utilities_unleashed)

[MP: Vector](https://courses.engr.illinois.edu/cs241/sp2016/vector)

## 6.Forking Processes

[Lecture: Wikibook Fork, Exec, Wait](https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-2%3A-Fork%2C-Exec%2C-Wait)

**What does the following 'exec' example do?**

```c
#include <unistd.h>
#include <fcntl.h> // O_CREAT, O_APPEND etc. defined here

int main() {
    close(1); // close standard out
    open("log.txt", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    puts("Captain's log");
    chdir("/usr/include");
    // execl( executable,  arguments for executable including program name and NULL at the end)

    execl("/bin/ls", /* Remaining items sent to ls*/ "/bin/ls", ".", (char *) NULL); // "ls ."
    perror("exec failed");
    return 0; // Not expected
}
```

There's no error checking in the above code (we assume close,open,chdir etc works as expected).

- open: will use the lowest available file descriptor (i.e. 1) ; so standard out now goes to the log file.
- chdir : Change the current directory to /usr/include
- execl : Replace the program image with /bin/ls and call its main() method
- perror : We don't expect to get here - if we did then exec failed.

**Subtle forkbomb bug**

What's wrong with this code

```c
#include <unistd.h>
#define HELLO_NUMBER 10

int main(){
    pid_t children[HELLO_NUMBER];
    int i;
    for(i = 0; i < HELLO_NUMBER; ++i){
        pid_t child = fork();
        if(child == -1){
            break;
        }
        if(child == 0){ //I am the child
            execlp("ehco", "echo", "hello", NULL);
        }
        else{
            children[i] = child;
        }
    }

    int j;
    for(j = 0; j < i; j++){
        waitpid(children[j], NULL, 0);
    }
    return 0;
}
```

We misspelled `ehco`, so we can't `exec` it. What does this mean? Instead of creating 10 processes we just created 2**10 processes, fork bombing our machine. How could we prevent this? Put an exit right after exec so in case exec fails we won't end up fork bombing our machine.

**What does the child inherit from the parent?**

- Open file handles. If the parent later seeks, say, to the back to the beginning of the file then this will affect the child too (and vice versa).
- Signal handlers
- Current working directory
- Environment variables

**Good parents don't let their children become zombies!**

Note, the word 'zombie' in this instance sheds some light as to what they actually represent. When a child finishes (or terminates) it still takes up a slot in the kernel process table. Furthermore, they still contain information about the process that got terminated, such as process id, exit status, etc. (i.e. a skeleton of the original process still remains). Only when the child has been 'waited on' will the slot be available and the remaining information can be accessed by the parent.

A long running program could create many zombies by continually creating processes and never `wait`-ing for them.

**What would be effect of too many zombies?**

Eventually there would be insufficient space in the kernel process table to create a new processes. Thus `fork()` would fail and could make the system difficult / impossible to use - for example just logging in requires a new process!

**What does the system do to help prevent zombies?**

Once a process completes, any of its children will be assigned to "init" - the first process with pid of 1. Thus these children would see getppid() return a value of 1. These orphans will eventually finish and for a brief moment become a zombie. Fortunately, the init process automatically waits for all of its children, thus removing these zombies from the system.

## 7.Signals for Process Control

[Lecture: Wikibook process control](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals)

**Can I find out the exit value of my child?**

You can find the lowest 8 bits of the child's exit value (the return value of `main()` or value included in `exit()`): Use the "Wait macros" - typically you will use "WIFEXITED" and "WEXITSTATUS" . See `wait`/`waitpid` man page for more information).

```c
int status;
pid_t child = fork();
if (child == -1) return 1; // Failed
if (child > 0) { /* I am the parent - wait for the child to finish */
    pid_t pid = waitpid(child, &status, 0);
    if (pid != -1 && WIFEXITED(status)) {
        int low8bits = WEXITSTATUS(status);
        printf("Process %d returned %d" , pid, low8bits);
    }
} else { /* I am the child */
    // do something interesting
    execl("/bin/ls", "/bin/ls", ".", (char *) NULL); // "ls ."
}
```

A process can only have 256 return values, the rest of the bits are informational.

**What's a signal?**

A signal is a construct provided to us by the kernel. It allows one process to asynchronously send a signal (think a message) to another process. If that process wants to accept the signal, it can, and then, for most signals, can decide what to do with that signal. Here is a short list (non comprehensive) of signals.

| Name    | Default Action                       | Usual Use Case                    |
| ------- | ------------------------------------ | --------------------------------- |
| SIGINT  | Terminate Process (Can be caught)    | Tell the process to stop nicely   |
| SIGQUIT | Terminate Process (Can be caught)    | Tells the process to stop harshly |
| SIGSTOP | Stop Process (Cannot be caught)      | Stops the process to be continued |
| SIGCONT | Continues a Process                  | Continues to run the process      |
| SIGKILL | Terminate Process (Cannot be caught) | You want your process gone        |

**When are signals generated?**

- When the user sends a signal. For example, you are at the terminal, and you send `CTRL-C`
- When a system event happens. For example, you get a `SIGCHILD` after forking to notice when one of your children have exited.
- When another program sends it. For example, when you execute `kill -9 PID`, it sends `SIGKILL`
- When an appropriate hardware interrupt is triggered. For example, if you access a page that you aren't supposed to, the hardware generates a segfault interrupt which gets intercepted by the kernel. The kernel finds the process that caused this and sends a software interrupt signal `SIGSEGV`.

**Can I pause my child?**

Yes ! You can temporarily pause a running process by sending it a SIGSTOP signal. If it succeeds it will freeze a process; i.e. the process will not be allocated any more CPU time.

To allow a process to resume execution send it the SIGCONT signal.

For example, Here's program that slowly prints a dot every second, up to 59 dots.

```c
#include <unistd.h>
#include <stdio.h>
int main() {
    printf("My pid is %d\n", getpid());
    int i = 60;
    while (--i) { 
        write(1, ".", 1);
        sleep(1);
    }
    write(1, "Done!", 5);
    return 0;
}
```

We will first start the process in the background (notice the & at the end). Then send it a signal from the shell process by using the kill command.

```bash
>./program &
My pid is 403
...
>kill -SIGSTOP 403
>kill -SIGCONT 403
```

**How do I kill/stop/suspend my child from C?**

In C, send a signal to the child using `kill` POSIX call,

```c
kill(child, SIGUSR1); // Send a user-defined signal
kill(child, SIGSTOP); // Stop the child process (the child cannot prevent this)
kill(child, SIGTERM); // Terminate the child process (the child can prevent this)
kill(child, SIGINT); // Equivalent to CTRL-C (by default closes the process)
```

As we saw above there is also a kill command available in the shell e.g. get a list of running processes and then terminate process 45 and process 46

```bash
ps
kill -l 
kill -9 45
kill -s TERM 46
```

## 8.Thanks for the heap memory

[Lecture: Wikibook heap memory introduction](https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-1%3A-Heap-Memory-Introduction)

**What happens when I call malloc?**

The function `malloc` is a C library call and is used to reserve a contiguous block of memory. Unlike stack memory, the memory remains allocated until `free` is called with the same pointer. There is also `calloc` and `realloc` which are discussed below.

**Can malloc fail?**

If `malloc` fails to reserve any more memory then it returns `NULL`. Robust programs should check the return value. If your code assumes `malloc` succeeds and it does not, then your program will likely crash (segfault) when it tries to write to address 0.

**Where is the heap and how big is it?**

The heap is part of the process memory and it does not have a fixed size. Heap memory allocation is performed by the C library when you call `malloc` (`calloc`, `realloc`) and `free`.

First a quick review on process memory: A process is a running instance of your program. Each process has its own address space. For example on a 32 bit machine your process gets about 4 billion addresses to play with, however not all of these are valid or even mapped to actual physical memory (RAM). Inside the process's memory you will find the executable code, space for the stack, environment variables, global (static) variables and the heap.

By calling `sbrk` the C library can increase the size of the heap as your program demands more heap memory. As the heap and stack (one for each thread) need to grow, we put them at opposite ends of the address space. So for typical architectures the heap will grow upwards and the stack grows downwards.

Truthiness: Modern operating system memory allocators no longer need `sbrk` - instead they can request independent regions of virtual memory and maintain multiple memory regions. For example gigabyte requests may be placed in a different memory region than small allocation requests. However this detail is an unwanted complexity: The problems of fragmentation and allocating memory efficiently still apply, so we will ignore this implementation nicety here and will write as if the heap is a single region.

If we write a multi-threaded program (more about that later) we will need multiple stacks (one per thread) but there's only ever one heap.

On typical architectures, the heap is part of the `Data segment` and starts just above the code and global variables.

**Do programs need to call brk or sbrk?**

Not typically (though calling `sbrk(0)` can be interesting because it tells you where your heap currently ends). Instead programs use `malloc,calloc,realloc` and `free` which are part of the C library. The internal implementation of these functions will call `sbrk` when additional heap memory is required.

```c
void *top_of_heap = sbrk(0);
malloc(16384);
void *top_of_heap2 = sbrk(0);
printf("The top of heap went from %p to %p \n", top_of_heap, top_of_heap2);
```

Example output: `The top of heap went from 0x4000 to 0xa000`

**What is calloc?**

Unlike `malloc`, `calloc` initializes memory contents to zero and also takes two arguments (the number of items and the size in bytes of each item). A naive but readable implementation of `calloc` looks like this:

```c
void *calloc(size_t n, size_t size)
{
	size_t total = n * size; // Does not check for overflow!
	void *result = malloc(total);
	
	if (!result) return NULL;
	
// If we're using new memory pages 
// just allocated from the system by calling sbrk
// then they will be zero so zero-ing out is unnecessary,

	memset(result, 0, total);
	return result; 
}
```

An advanced discussion of these limitations is [here](http://locklessinc.com/articles/calloc/).

Programmers often use `calloc` rather than explicitly calling `memset` after `malloc`, to set the memory contents to zero. Note `calloc(x,y)` is identical to `calloc(y,x)`, but you should follow the conventions of the manual.

```c
// Ensure our memory is initialized to zero
link_t *link  = malloc(256);
memset(link, 0, 256); // Assumes malloc returned a valid address!

link_t *link = calloc(1, 256); // safer: calloc(1, sizeof(link_t));
```

**Why is the memory that is first returned by sbrk initialized to zero?**

If the operating system did not zero out contents of physical RAM it might be possible for one process to learn about the memory of another process that had previously used the memory. This would be a security leak.

Unfortunately this means that for `malloc` requests before any memory has been freed and simple programs (which end up using newly reserved memory from the system) the memory is *often* zero. Then programmers mistaken write C programs that assume malloc'd memory will *always* be zero.

```c
char* ptr = malloc(300);
// contents is probably zero because we get brand new memory
// so beginner programs appear to work!
// strcpy(ptr, "Some data"); // work with the data
free(ptr);
// later
char *ptr2 = malloc(308); // Contents might now contain existing data and is probably not zero
```

**Why doesn't malloc always initialize memory to zero?**

Performance! We want malloc to be as fast as possible. Zeroing out memory may be unnecessary.

**What is realloc and when would you use it?**

`realloc` allows you to resize an existing memory allocation that was previously allocated on the heap (via malloc,calloc or realloc). The most common use of realloc is to resize memory used to hold an array of values. A naive but readable version of realloc is suggested below

```c
void * realloc(void * ptr, size_t newsize) {
    // Simple implementation always reserves more memory
    // and has no error checking
    void *result = malloc(newsize); 
    size_t oldsize =  ... //(depends on allocator's internal data structure)
        if (ptr) memcpy(result, ptr, newsize < oldsize ? newsize : oldsize);
    free(ptr);
    return result;
}
```

An INCORRECT use of realloc is shown below:

```c
int *array = malloc(sizeof(int) * 2);
array[0] = 10; array[1] = 20;
// Ooops need a bigger array - so use realloc..
realloc (array, 3); // ERRORS!
array[2] = 30; 
```

The above code contains two mistakes. Firstly we needed 3*sizeof(int) bytes not 3 bytes. Secondly realloc may need to move the existing contents of the memory to a new location. For example, there may not be sufficient space because the neighboring bytes are already allocated. A correct use of realloc is shown below.

```c
array = realloc(array, 3 * sizeof(int));
// If array is copied to a new location then old allocation will be freed.
```

A robust version would also check for a `NULL` return value. Note `realloc` can be used to grow and shrink allocations.

**What is the silliest malloc and free implementation and what is wrong with it?**

```
void* malloc(size_t size)
// Ask the system for more bytes by extending the heap space. 
// sbrk Returns -1 on failure
   void *p = sbrk(size); 
   if(p == (void *) -1) return NULL; // No space left
   return p;
}
void free() {/* Do nothing */}
```

The above implementation suffers from two major drawbacks:

- System calls are slow (compared to library calls). We should reserve a large amount of memory and only occasionally ask for more from the system.
- No reuse of freed memory. Our program never re-uses heap memory - it just keeps asking for a bigger heap.

If this allocator was used in a typical program, the process would quickly exhaust all available memory. Instead we need an allocator that can efficiently use heap space and only ask for more memory when necessary.

**What are placement strategies?**

During program execution memory is allocated and de-allocated (freed), so there will be gaps (holes) in the heap memory that can be re-used for future memory requests. The memory allocator needs to keep track of which parts of the heap are currently allocated and which are parts are available.

Suppose our current heap size is 64K, though not all of it is in use because some earlier malloc'd memory has already been freed by the program:

| 16KB free | 10KB allocated | 1KB free | 1KB allocated | 30KB free | 4KB allocated | 2KB free |
| --------- | -------------- | -------- | ------------- | --------- | ------------- | -------- |
|           |                |          |               |           |               |          |

If a new malloc request for 2KB is executed (`malloc(2048)`), where should `malloc` reserve the memory? It could use the last 2KB hole (which happens to be the perfect size!) or it could split one of the other two free holes. These choices represent different placement strategies.

Whichever hole is chosen, the allocator will need to split the hole into two: The newly allocated space (which will be returned to the program) and a smaller hole (if there is spare space left over).

A perfect-fit strategy finds the smallest hole that is of sufficient size (at least 2KB):

| 16KB free | 10KB allocated | 1KB free | 1KB allocated | 30KB free | 4KB allocated | `2KB HERE!` |
| --------- | -------------- | -------- | ------------- | --------- | ------------- | ----------- |
|           |                |          |               |           |               |             |

A worst-fit strategy finds the largest hole that is of sufficient size (so break the 30KB hole into two):

| 16KB free | 10KB allocated | 1KB free | 1KB allocated | `2KB HERE!` | `28KB free` | 4KB allocated | 2KB free |
| --------- | -------------- | -------- | ------------- | ----------- | ----------- | ------------- | -------- |
|           |                |          |               |             |             |               |          |

A first-fit strategy finds the first available hole that is of sufficient size (break the 16KB hole into two):

| `2KB HERE!` | `14KB free` | 10KB allocated | 1KB free | 1KB allocated | 30KB free | 4KB allocated | 2KB free |
| ----------- | ----------- | -------------- | -------- | ------------- | --------- | ------------- | -------- |
|             |             |                |          |               |           |               |          |

**What is external fragmentation?**

In the example below, of the 64KB of heap memory, 17KB is allocated, and 47KB is free. However the largest available block is only 30KB because our available unallocated heap memory is fragmented into smaller pieces.

| `16KB free` | 10KB allocated | `1KB free` | 1KB allocated | `30KB free` | 4KB allocated | 2KB allocated |
| ----------- | -------------- | ---------- | ------------- | ----------- | ------------- | ------------- |
|             |                |            |               |             |               |               |

**What effect do placement strategies have on external fragmentation and performance?**

Different strategies affect the fragmentation of heap memory in non-obvious ways, which only are discovered by mathematical analysis or careful simulations under real-world conditions (for example simulating the memory allocation requests of a database or webserver). For example, best-fit at first glance appears to be an excellent strategy however, if we can not find a perfectly-sized hole then this placement creates many tiny unusable holes, leading to high fragmentation. It also requires a scan of all possible holes.

First fit has the advantage that it will not evaluate all possible placements and therefore be faster.

Since Worst-fit targets the largest unallocated space, it is a poor choice if large allocations are required.

In practice first-fit and next-fit (which is not discussed here) are often common placement strategy. Hybrid approaches and many other alternatives exist (see implementing a memory allocator page).

# Week 4

[Lab: Mini Valgrind](https://courses.engr.illinois.edu/cs241/sp2016/Mini%20Valgrind)

[MP: Shell](https://courses.engr.illinois.edu/cs241/sp2016/Shell)

## 9.Memory allocators I

[wikibook week 4](https://github.com/angrave/SystemProgramming/wiki#week-4)

## 10.Memory allocators II

[wikibook week 4](https://github.com/angrave/SystemProgramming/wiki#week-4)

## 11.Threads

[wikibook week 4](https://github.com/angrave/SystemProgramming/wiki#week-4)

# Week 5

[Lab: Mangled Mutexes](https://courses.engr.illinois.edu/cs241/sp2016/mangled_mutexes)

[MP: Malloc](https://courses.engr.illinois.edu/cs241/sp2016/malloc)

## 12.Threads and mutex locks

[wikibook week 4](https://github.com/angrave/SystemProgramming/wiki#week-4)

## 13.Mutexes and semaphores

[wikibook week 5](https://github.com/angrave/SystemProgramming/wiki#week-5)

## 14.Critical Section Problem

[wikibook week 5](https://github.com/angrave/SystemProgramming/wiki#week-6)

# Week 6

[Lab: Terrible Threads](https://courses.engr.illinois.edu/cs241/sp2016/terrible_threads)

## 15.Critical Section Problem II

[wikibook week 6](https://github.com/angrave/SystemProgramming/wiki#week-6)

## 16.Condition Variables

[wikibook week 6](https://github.com/angrave/SystemProgramming/wiki#week-6)

## 17.Reader Writer Problem

[wikibook week 7](https://github.com/angrave/SystemProgramming/wiki#week-7)

# Week 7

[Lab: Broken Barriers](https://courses.engr.illinois.edu/cs241/sp2016/broken_barriers)

## 18.Producer Consumer. RW II

[wikibook week 7](https://github.com/angrave/SystemProgramming/wiki#week-7)

## 19.Deadlock

[wikibook week 7](https://github.com/angrave/SystemProgramming/wiki#week-7)

## 20.Dining Philosophers

[wikibook week 8](https://github.com/angrave/SystemProgramming/wiki#week-8)

# Week 8

[Lab: Overworked Interns](https://courses.engr.illinois.edu/cs241/sp2016/overworked_interns)

[MP: Password Cracker](https://courses.engr.illinois.edu/cs241/sp2016/password_cracker)

## 21.Page tables

[wikibook week 9](https://github.com/angrave/SystemProgramming/wiki#week-9)

## 22.Pipes and Files

[wikibook week 9](https://github.com/angrave/SystemProgramming/wiki#week-9)

## 23.Pipes, Files, VM II

[wikibook week 9](https://github.com/angrave/SystemProgramming/wiki#week-9)

# Week 9

[Lab: Ideal Indirection](https://courses.engr.illinois.edu/cs241/sp2016/ideal_indirection)

## 24.Networking Intro

[wikibook week 10](https://github.com/angrave/SystemProgramming/wiki#week-10)

## 25.TCP

[wikibook week 10](https://github.com/angrave/SystemProgramming/wiki#week-10)









