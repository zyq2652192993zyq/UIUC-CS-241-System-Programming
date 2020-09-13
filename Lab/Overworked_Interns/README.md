> # Overworked Interns

## Learning Objectives

- Synchronization
- Deadlock / Livelock / Starvation
- Coffman Conditions
- Dining Philosophers
- Messing with Interns

## Backstory

You are creating a start up that allows companies to rent code monkeys interns for a project. They are billed by the number of days it takes to complete the project. The reason why companies want to rent interns is because they realized they do not need dedicated interns, since they do not always have projects at hand (sometimes they are contemplating about life). Basically a company in 1 of 3 states: “trying to rent interns”, “working with interns” or “having a board meeting”. They came to the conclusion that a possible way of handling this dilemma is to simply overwork the interns by having them work for 2 companies, just not at the same time.

This is where you come along….

The scenario is as follows. Each of the companies practices pair programming, so you will need to find a way to schedule the interns so that we can continue to bill each company as often as possible. This means that work on a project can only be completed when the company has **TWO** interns assigned to it. There are `n` companies and `k` interns in this scenario. For some odd reason the `i`th company likes to work with intern `i%k` and `(i+1)%k`… A company might decide to give the interns a break from the project, in which case they are free to be assigned to another company that wants them and won’t be reassigned until said company gives them a break.

Your mentor has written a simulation of this scenario with a synchronization strategy, but it does not work. The mentor left some notes that reads

> Dear intern, I have written a simulation of the ‘intern for rent’ program, but sometimes a company grabs one intern, then waits for the second intern, which wont be released until the first intern is given a break…

## Your Job

- Read [Resource Allocation Graphs](https://github.com/angrave/SystemProgramming/wiki/Deadlock%2C-Part-1%3A-Resource-Allocation-Graph) and [Deadlock Conditions](https://github.com/angrave/SystemProgramming/wiki/Deadlock%2C-Part-2%3A-Deadlock-Conditions) to have all the knowledge you will need for this assignment.
- Fill out this [Google Form](http://goo.gl/forms/BdDaErdQjT) **BEFORE** you write any code (see the section on testing for details on gathering data).
- Read `simulator.c`, `bad_company.c`, and `company.h` until you have a good sense of what the code is doing (Nothing will make sense until you do).
- Overwrite `good_company.c` with your correct solution.

## Testing

We provided you with a Makefile.

Typing

```bash
make
```

will create the `good_company` and `bad_company` executable. For both the `good_company` and `bad_company` executable you can execute them with 2 required arguments and 3rd optional argument as follows:

```bash
./bad_company 5 6 100000
```

will run the bad solution with 5 companies and 6 interns and with a delta of 100000. Delta is approximately how long an operation takes in microseconds (read the source code for more details).

If you see something like this:

```
Company 0 used our services for 2083 billable days.
Company 1 used our services for 2164 billable days.
Company 2 used our services for 2456 billable days.
Company 3 used our services for 2826 billable days.
Company 4 used our services for 3733 billable days.

Total Billable days : 13262
```

then the simulation terminated successfully. This does not mean your solution is correct. There may still be race conditions, so it it up to you to test your code thoroughly with all sorts of parameters (and use [tsan](https://courses.engr.illinois.edu/cs241/sp2016/tsan)). If the simulator stops without billing the companies just hit `CTRL+C`.

------



















