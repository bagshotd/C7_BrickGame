CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11 -lncurses
SANITIZE=-fsanitize=address -g 
LFLAGS=$(FLAGS) -lncurses -l
TST_LIBS = -lcheck
LCOVFLAGS = --ignore-errors inconsistent --ignore-errors mismatch

BACK=$(wildcard back/*.c)
FRONT=$(wildcard front/*.c)
OBJ_BACK=$(BACK:.c=.o)
TEST=$(wildcard tests/check.c)

all: install

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

mkbuild:
	rm -rf build/
	mkdir build

uninstall :
	rm -rf build/
	echo "\033[31mGame uninstall \033[0m"

install : mkbuild main
	echo "\033[32mGame install \033[0m"

back_lib : $(OBJ_BACK)
	ar rc build/libback.a back/*.o
	ranlib build/libback.a
	cd ..
	rm back/*.o

front_lib:
	$(CC) $(FLAGS) -c front/frontend.c -o front/frontend.o
	ar rc build/libfront.a front/frontend.o
	ranlib build/libfront.a 
	rm front/*.o

main: back_lib front_lib
	$(CC)   tetris.c  -o build/TETRIS  $(FLAGS) -lncurses -L./build -lback -lfront

dvi:
	doxygen .Doxyfile
	open doxygen/html/index.html

dist: install
	rm -rf dist.tar.gz
	tar -cvzf dist.tar.gz build/Tetris
	

clang-format:
	clang-format -i back/*.c back/*.h front/*.c front/*.h tetris.c tests/*.c

clean :
	rm -rf *.o back/*.o front/*.o front/*.gcda front/*.gcno build/ *.dSYM doxygen/ back/*.gcda  back/*.gcno *.gcda *gcno *.info .saveScore.txt report/ test dist.tar.gz

sanitize : mkbuild back_lib front_lib
	$(CC) $(FLAGS) $(SANITIZE) -lncurses build/back.a build/front.a tetris.c -o build/TETRIS
	echo "\033[32mSanitize done \033[0m"

add_coverage_flag:
	$(eval FLAGS += --coverage)

test : clean mkbuild back_lib
	$(CC) $(FLAGS) $(TEST) -lncurses build/back.a $(TST_LIBS) -o test

gcov_report: add_coverage_flag test
	./test
	lcov -t "check" -o check.info -c -d ./  --no-external $(LCOVFLAGS)
	genhtml -o report check.info $(LCOVFLAGS)
	open report/index.html

leaks: install
	leaks -atExit -- ./Tetris

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .