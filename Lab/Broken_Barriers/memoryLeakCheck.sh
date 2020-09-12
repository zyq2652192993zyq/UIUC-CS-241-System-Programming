#!/bin/bash

valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./barrier_test
