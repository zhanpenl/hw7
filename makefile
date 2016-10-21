CC = g++
CFLAG = -g -Wall -std=c++11
GTEST_LL = -I /usr/local/opt/gtest/include/ -l gtest -l gtest_main -pthread

all: 

bst_test: bst_gtest.cpp bst.h
	$(CC) $(CFLAG) $^ $(GTEST_LL) -o $@

avl_test: avl_gtest.cpp bst.h
	$(CC) $(CFLAG) $^ $(GTEST_LL) -o $@

main: main.cpp bst.h
	$(CC) $(CFLAG) $^ $(GTEST_LL) -o $@ -std=c++11

.PHONY:

clean:
	rm -rf bst_test avl_test main