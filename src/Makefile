PROJECT_NAME=BrickGame_v2
CC=gcc
CPP=g++
CFLAGS=-Wall -Werror -Wextra
STANDART=-std=c++17

CLI=gui/cli
DESKTOP=gui/desktop
TETRIS=brick_game/tetris
SNAKE=brick_game/snake

COVFLAGS = -fprofile-arcs -ftest-coverage
GTEST_LIB := $(shell pkg-config --libs gtest)
INCLUDE := $(shell pkg-config --cflags gtest)

OPENOS = vi 
ifeq ($(shell uname -s), Linux) 
	OPENOS = xdg-open 
endif 
ifeq ($(shell uname -s), Darwin) 
	OPENOS = open 
endif 

all: clean cli desktop

cli: libtetris.a libsnake.a
	[ -d build ] || mkdir build
	$(CC) $(CFLAGS) -c $(CLI)/*.c
	$(CPP) $(STANDART) -c $(CLI)/*.cpp
	cd build && $(CPP) $(STANDART) -o $(PROJECT_NAME)_cli ../*.o -lncurses -L./.. -lsnake -ltetris
	rm -rf *.o *.a

libsnake.a:
	$(CPP) $(STANDART) -c $(SNAKE)/*.cpp
	ar rc libsnake.a *.o
	rm -rf *.o

libtetris.a:
	$(CC) $(CFLAGS) -c $(TETRIS)/*.c
	ar rc libtetris.a *.o
	rm -rf *.o

desktop:
	rm -rf desk
	mkdir desk
	cd desk && qmake ../gui/desktop
	cd desk && make
	[ -d build ] || mkdir build
	mv desk/BrickGame_desktop build/$(PROJECT_NAME)_desktop
	rm -rf desk

dvi: clean
	@$(OPENOS) doc/documentation.md

dist: install
	tar -cvzf $(PROJECT_NAME).tgz build
	rm -rf build

tests: clean
	$(CPP) $(STANDART) $(COVFLAGS) $(INCLUDE) -c $(SNAKE)/*.cpp
	$(CPP) $(STANDART) $(COVFLAGS) $(INCLUDE) -c tests/*.cpp
	$(CPP) $(STANDART) $(COVFLAGS) *.o -o snake_tests -lm $(GTEST_LIB)
	./snake_tests
	rm -rf *.o 

gcov_report: tests
	lcov -c -d . --no-external -o snake_gcov.info
	genhtml -o report snake_gcov.info
	chmod 777 report/*
	rm *.gcda *.gcno *.info snake_tests
	@$(OPENOS) ./report/index.html

install: all

uninstall:
	rm -rf build

start_cli:
	./build/BrickGame_v2_cli

start_desktop:
	./build/BrickGame_v2_desktop

clean: 
	@rm -rf build report $(PROJECT_NAME).tgz *.txt *.o *.a *.info snake_tests desk *.gcda *.gcno