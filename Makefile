LIBS_LINUX=
ADD_LIB=
LIBNAME=
DEPENDS=sudo apt install -y qtbase5-dev cmake doxygen

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)

	ADD_LIB= -l:s21_brick_game.a
	LIBNAME= s21_brick_game.a

	Linux_type := $(shell cat /etc/issue | sed -n '1p' | awk '{print $$1}')

	ifeq ($(Linux_type), Arch)
		LIBS_LINUX = -lm
	endif

	ifeq ($(Linux_type), Ubuntu)
		LIBS_LINUX = -lsubunit -lm -pthread -lrt
	endif

	ifeq ($(Linux_type), Debian)
		LIBS_LINUX = -lsubunit -lm -pthread -lrt
	endif

endif

ifeq ($(UNAME_S), Darwin)
    ADD_LIB= -ls21_brick_game
	LIBNAME= libs21_brick_game.a
endif

CC=g++
CFLAGS=-Wall -Wextra -Werror -g
TEST_LIBS= -lgtest_main -pthread -lgtest


OBJS=*.o
SRCS_OBJ=*.cpp
TESTS_OBJ=*.cpp

CONSOLE_TARGET= brick_game_console
DESKTOR_TARGET= brick_game_desktop
TEST_TARG= test_s21_brick_game

SRC_DIR=brick_game	
CONSOLE_DIR=gui/console
DESKTOP_DIR=gui/desktop
TESTS_DIR=tests

SRCS_H =*.h

LIBS = $(LIBS_LINUX)

ALL_SRC_OBJ = $(shell find $(SRC_DIR) -type f -name "$(SRCS_OBJ)")
ALL_TESTS_OBJ = $(shell find $(TESTS_DIR) -type f -name "$(TESTS_OBJ)")

ALL_SRC_H = $(shell find $(SRC_DIR) -type f -name "$(SRCS_H)")
ALL_TESTS_H = $(shell find $(TESTS_DIR) -type f -name "*.h")

ALL_CONSOLE_OBJ = $(shell find $(CONSOLE_DIR) -type f -name "*.cpp")

all: install

s21_brick_game.a:
	$(CC) $(CFLAGS) $(ALL_SRC_OBJ) -c
	ar rsc $(LIBNAME) $(OBJS)
	ranlib $(LIBNAME)
	rm -f $(OBJS)

desktop: s21_brick_game.a 
	cd gui/desktop && cmake -B../../build 
	cd build && cmake --build ./ && mv $(DESKTOR_TARGET) ../

console: s21_brick_game.a
	$(CC) $(CFLAGS) $(ALL_CONSOLE_OBJ) -L. $(ADD_LIB) $(LIBS) -lncurses -o $(CONSOLE_TARGET)

install: desktop console 
	sudo install -m 755 $(CONSOLE_TARGET) /usr/local/bin
	sudo install -m 755 $(DESKTOR_TARGET) /usr/local/bin

uninstall:
	sudo rm -f /usr/localn/bin/$(CONSOLE_TARGET)
	sudo rm -f /usr/local/bin/$(DESKTOR_TARGET)

dvi: 
	doxygen Doxyfile
	xdg-open docs/html/index.html

dist: clean_all
	tar -czvf s21_brick_game.tar.gz *

test: s21_brick_game.a
	$(CC) $(CFLAGS) $(ALL_TESTS_OBJ) $(LIBS) $(TEST_LIBS) -L. $(ADD_LIB) -o $(TEST_TARG) 
	./$(TEST_TARG)

test_val: s21_brick_game.a test
	valgrind --tool=memcheck --leak-check=yes -s ./$(TEST_TARG)

gcov_report:
	$(CC)  $(CFLAGS) $(LIBS) $(TEST_LIBS) --coverage  $(ALL_SRC_OBJ) $(ALL_TESTS_OBJ) -o $(TEST_TARG)
	./$(TEST_TARG)
	lcov -t "$(TEST_TARG)" -o $(TEST_TARG).info -c -d . --no-external --rc lcov_branch_coverage=1
	lcov --remove $(TEST_TARG).info '/usr/*' '*/gtest/*' '*/gmock/*' '*/cmake-build-debug/*' -o $(TEST_TARG)_filtered.info
	genhtml -o report $(TEST_TARG)_filtered.info
	rm -f *.gcda *.gcno *.info $(TEST_TARG)

check_linters:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -n $(ALL_SRC_OBJ) $(ALL_SRC_H) $(ALL_TESTS_OBJ) $(ALL_TESTS_H) $(ALL_CONSOLE_OBJ)
	rm -rf .clang-format

fix_linters:
	cp ../materials/linters/.clang-format ./.clang-format
	clang-format -i $(ALL_SRC_OBJ) $(ALL_SRC_H) $(ALL_TESTS_OBJ) $(ALL_TESTS_H) $(ALL_CONSOLE_OBJ)
	rm -rf .clang-format

clean_lib:
	rm -f *.[oa]

clean_cov:
	rm -f *.gcno *.gcda *.info *.gcov $(TEST_TARG)

clean_docs:
	rm -rf docs

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem $(ALL_SRC_OBJ)

clean: clean_lib clean_cov clean_docs
	rm -f $(TEST_TARG) $(CONSOLE_TARGET) $(DESKTOR_TARGET) SnakeScore.txt TetrisScore.txt
	rm -rf report *.dSYM
	rm -rf build

clean_all: uninstall clean

rebuild: clean all
