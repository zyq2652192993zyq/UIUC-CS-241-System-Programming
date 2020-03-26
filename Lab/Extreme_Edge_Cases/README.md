># Lab-2 Extreme Edge Cases

# Backstory

What makes code good? Is it camelCase? Lots of comments? Descriptive variable names, perhaps?

One thing we know is that good code is generally modular–it consists of discrete “units” of functionality that are only responsible for certain behavior. In our case, where we’re working with C, these units are primarily functions.

For example, the C string function strlen is responsible solely for determining the length of a string; it doesn’t do any I/O or networking. A function that knows all and tries to do all would be bad design, and testing whether that kind of function adheres to expectations would be nontrivial.

A programmer might ask, “Do my units of work behave the way I expect? If my function expects a string, how does it behave when given NULL?”. These are crucial questions, since ensuring that units of code work exactly the way one would expect makes it easy to build reliable and robust software. An unreliable unit in a large system can affect the entire system significantly. Imagine if strcpy, for example, did not behave properly on all inputs; all of the higher-level units that use strcpy, and all of the units that interact with those units, would in-turn have unpredictable behavior, and so the unreliablity would propagate through the whole system.

Enter unit testing.

# Unit Testing

Unit testing is a ubiquitous and crucial software development method used heavily in industry. According to artofunittesting.com, a unit test is an automated piece of code that invokes a unit of work in the system and then checks a single assumption about the behavior of that unit of work. This sounds like testing–leave it to the QAs, right? Actually, developers, much to their chagrin, are expected to write their own unit tests.

In order to write effective unit tests, all possible cases of input to a unit (mainly functions, in C), including edge cases, should be tested. Good unit tests test (extreme) edge cases, making sure that the discrete unit of functionality performs as specified with unexpected inputs.

In this Lab, your goal is to create and test the behavior of an arbitrary string manipulation function to determine if it is reliable and predictable. While writing your functions, try and write modular code–this will make your life easier when you test it. You’ll learn how to write effective test cases–an incredibly helpful skill for the rest of the course. Finally, you’ll be able to take these skills to Facenovel for your next internship and impress your coworkers.

# Camel Caser

We have chosen

```c
char ** camel_caser(const char* input)
```

as your arbitrary string manipulation function.

Your manager, to celebrate Hump Day, has asked all of the interns to implement a brand new camelCaser to convert sentences into camelCase. To give you a chance to earn your return offer, he also assigned you to write test cases for all the other interns’ implementations of camelCaser, with the implementation hidden from you.

Let’s say I want to get a sequence of sentences in camelCase. This is the string passed into your method:

```
"The Heisenbug is an incredible creature. Facenovel servers get their power from its indeterminism. Code smell can be ignored with INCREDIBLE use of air freshener. God objects are the new religion."
```

Your method should return the following:

```
["theHeisenbugIsAnIncredibleCreature",
"facenovelServersGetTheirPowerFromItsIndeterminism",
"codeSmellCanBeIgnoredWithIncredibleUseOfAirFreshener",
"godObjectsAreTheNewReligion",
NULL]
```

The brackets denote that the above is an array of those strings. (More details in this [section](https://courses.engr.illinois.edu/cs241/sp2016/extreme_edge_cases.html#memory))

- A NULL pointer is undefined for camelCaser, so you should just return a NULL pointer.
- A input sentence, `input_s`, is defined as any MAXIMAL substring of the input string that ends with a punctuation mark and does not contain a punctuation mark.
  - This means that “Hello.World.” gets split into 2 sentences “Hello.” and “World.” and NOT “Hello.World.”.
- Let the camelCasing of `input_s` be called `output_s`
- `output_s` is the the concatenation of all words `w` in `input_s` after `w` has been camelcased
  - The puncation from `input_s` is not added to `output_s`
- `w` is camelCased iff
  - it is first lowercased and if it is not the first word in the sentence, then it has its first letter uppercased.
- words are
  - delimited by the MAXIMAL amount of whitespace
    - This means that “hello world” is split into “hello” and world” and NOT “hello “, “ “, “ world” or any other combination of whitespaces
  - considered uppercased words if all of its letters are uppercased.
  - considered lowercased words if all of its letters are lowercased.
- punctuation marks, whitespace, and letters are defined by `ispunct()`, `isspace()`, and `isalpha()` respectively.
  - These are parts of the C standard, so you can `man ispunct` for more information.
- Finally, you return an array of `output_s` for every `input_s` in the input string terminated by a NULL pointer.

We have also included a reference implementation in the folder named “reference”. In the reference folder you can find the main file, which will let you interact with the reference implementation. This means that if you have a question like “what should be the result of inputting into camel_caser()”, then you should try it out in the reference main file.

Your implementation goes in camelCaser.c and you may not leak any memory.

# Camel Caser Result In Memory

camelCaser takes in a C string, which represents an arbitrary amount of sentences and returns a pointer to a NULL terminated array of C strings where each sentence has been camelCased. Note that all the C strings must be in their own buffers (1 malloc per camelCased sentence).

For those who like pictures I will explain what the return value of camelCaser looks like in memory.

![](F:\Project\UIUC-CS-241-System-Programming\assets\char_double_pointer.jpg)

In the above picture you can see that we have a char double pointer called ‘array’. Now in this scenario the char double pointer is a character pointer that points to the beginning of a NULL terminated array of character pointers. Each of the character pointers in the array point to the beginning of a NULL terminated char array that can be anywhere in memory in a separate buffer. The reason why these arrays are NULL terminated is because your user will need to know when these arrays end so they do not start reading garbage values. This means that

```c
array[0]
```

will return a character pointer. Dereferencing that character pointer gets me an actual character. For demonstration purposes I am going to show you how to grab the character “s” in “as”.

```c
// Take array and move it over by 3 times the size of a char pointer.
char **ptr = array + 3;
// Deference ptr to get back a character pointer pointing to the beginning of "as".
char *as = *ptr;
// Take that pointer and move it over by 1 times the size of a char.
char *ptr2 = as + 1;
// Now dereference that to get an actual char.
char s = *ptr2;
```

# Writing Unit Tests

Your goal is to show that the other interns’ implementations of camelCaser–which, of course, you can’t see directly–fail on some extreme test cases, and, in the meantime, demonstrate to the head honcho at Facenovel exactly how robust your own function is.

Facenovel promises to pass in C-strings. Likewise, you promise to return a dynamically allocated NULL terminated array of strings (the array and the strings both on the heap and each string in separate buffers).

What kinds of edge cases might come up?

Run make camelCaser to test. You will have to fill in tests in camelCaser_tests.c.

Because Facenovel values their testing server time you may not try more than 16 different inputs and each input must be less than 128 characters (only characters). This does NOT mean your implementation can assume input of 128 characters or less.

Also it is not in the spirit of unit testing to diff your implementation with the one you are testing. So for this lab you may not call on your own camel_caser when implementing the tester.

# Files

- `camelCaser_main.c` - Calls your unit tester
- `reference` - Contains the reference implementation of camelCaser
- `camelCaser.c` - Contains your implementation of camelCaser
- `camelCaser_tests.c` - Contains your unit tester for a camelCaser implementation

----

这个Lab单纯从实现难度来讲并不是很难，类似一个算法题。

题目的意思是输入一个长字符串，然后将此字符串切割成子串，每个子串的第一个单词小写，去掉空格，保持驼峰的命名规则，然后所有的子串组成一个数组输出，但是标点符号不用包含在子串里。如果输入的字符串为空，则直接返回`null`。

输入语句`input_s`被定义为输入字符串中以标点符号结尾且不包含标点符号的任何最大子字符串。这里标点符号的判定题目里引导我们使用函数`ispunct()`，除了空格，字母和数字外，其他都是标点符号。

**程序设计思路（先用C++实现）**

首先建立一个存储字符串的数组，第一次遍历先去掉标点符号，取出带有中间空格的子串。这里只需要注意每个标点符号后面带有一个空格。

然后去处理每个子串，首先把每个子串的第一个字母无论大写还是小写，一律变成小写，然后每个空格触发修改条件，将空格后面的字符变成大写。

最后重载输出运算符即可。

```c++
#include <iostream>
#include <vector>
#include <cmath>
#include <cctype>
#include <string>
#include <climits>
#include <cstdio>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>

using namespace std;

vector<string> camel_parser(const string & line)
{
    int n = line.size();
    vector<string> store;
    int start = 0, end = 0;
    while (end < n) {
        while (!ispunct(line[end])) { ++end; }
        store.push_back(line.substr(start, end - start));
        end += 2; start = end; // +=2是因为标点符号后面都会带着一个空格
    }

    for (string & str : store) {
        str[0] = tolower(str[0]);
        int n = str.size();
        string tmp;
        for (int i = 0; i < n; ++i) {
            if (isspace(str[i])) {
                str[i + 1] = toupper(str[i + 1]);
            }
            else {
                tmp.push_back(str[i]);
            }
        }
        str = tmp;
    }

    return store;
}

ostream & operator<<(ostream & os, const vector<string> & v)
{
    for (const auto & e : v) os << e << endl;
    return os;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    string line;
    while (getline(cin, line)) {
        auto v = camel_parser(line);
        cout << v << endl;
    }

    return 0;
}
```

```
# 测试用例
The Heisenbug is an incredible creature. Facenovel servers get their power from its indeterminism. Code smell can be ignored with INCREDIBLE use of air freshener. God objects are the new religion.
```

```
# 输出结果
theHeisenbugIsAnIncredibleCreature
facenovelServersGetTheirPowerFromItsIndeterminism
codeSmellCanBeIgnoredWithINCREDIBLEUseOfAirFreshener
godObjectsAreTheNewReligion
```

已经可以用C++语言实现了，那么用C语言实现基本上思路也是一致的。

**C语言单文件的实现**

既然已经用C++可以实现了，那么就考虑一下用C语言实现，顺便熟悉下C语言。另外用C语言必须做的一件事就是内存泄漏的检测。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// #include "camelCaser.h"

void print_output(char **output){
    if(output){
        char ** line = output;
        while(*line){
            printf("%s\n", *line);
            line++;
        }
    } else{
        printf("NULL POINTER!\n");
    }
    printf("-----------------------------------------------------------------------------------------\n");
}

char **camel_caser(const char *input)
{
	int cnt = 0;
	int len = strlen(input);
	//统计标点符号个数，也是子串的个数
	for (int i = 0; i < len; ++i) {
		if (ispunct(input[i])) ++(cnt);
	}
	//切割出每个子串
	int num = cnt;
	char **output = malloc(num * sizeof(char *));
	int start = 0, end = 0;
	int pos = 0;
	while (end < len) {
		while (!ispunct(input[end])) { ++end; }
		int n = end - start;
		output[pos] = malloc(n * sizeof(char));
		for (int i = 0; i < n; ++i) {
			output[pos][i] = input[i + start];
		}
		end += 2; start = end; ++pos;
	}

	char **res = malloc(num * sizeof(char *)); //最终的结果
	for (int i = 0; i < num; ++i) {
		char *tmp = output[i];
		int size = strlen(tmp);
		int k = 0;
		for (int j = 0; j < size; ++j) {
			if (isspace(tmp[j])) ++k;
		}
		res[i] = malloc((size - k) * sizeof(char));
		res[i][0] = tolower(tmp[0]);
		int pos = 1;
		for (int j = 1; j < size; ++j) {
			if (isspace(tmp[j])) {
				tmp[j + 1] = toupper(tmp[j + 1]);
			}
			else {
				res[i][pos++] = tmp[j];
			}
		}
	}

	for (int i = 0; i < num; ++i) {
		free(output[i]); output[i] = NULL;
	}
	free(output); output = NULL;

	return res;
}

void makeEmpty(char **output)
{
	while (*output) {
		char *tmp = *output;
		++output;
		free(tmp); tmp = NULL;
	}
}

int main() {
    char * inputs[] = {
        "Hello World.",
        "The Heisenbug is an incredible creature. Facenovel servers get their power from its indeterminism. Code smell can be ignored with INCREDIBLE use of air freshener. God objects are the new religion.",
        NULL
    };

    char ** input = inputs;
    while(*input){
        char **output = camel_caser(*input);
        print_output(output);
        makeEmpty(output);
        input++;
    }

    return 0;
}
```

最后用valgrind检验无内存泄漏。然后把上面的思路按照多文件编程规范编写即可。

----

需要完善的时`camlCaser_main.c`文件，通过此文件来调用我们的测试用例。

需要自主写出`camlCaser.c`的实现，接口被确定为`char ** camel_caser(const char* input)`。

在`camelCaser_test.c`里面补充测试用例。

