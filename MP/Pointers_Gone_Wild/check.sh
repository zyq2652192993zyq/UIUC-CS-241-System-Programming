#!/bin/bash
make

./part1-main > part1-output.txt
diff part1-expect-output.txt part1-output.txt

# valgrind --tool=memcheck --leak-check=full ./part1-main

./part2-main > part2-output.txt
diff part2-expect-output.txt part2-output.txt

# valgrind --tool=memcheck --leak-check=full ./part2-main

make clean
