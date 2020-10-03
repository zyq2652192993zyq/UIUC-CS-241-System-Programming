#!/bin/bash

valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./brutus journal.txt out.txt
