CC = gcc
FLAGS = -std=c11 -Wall -Werror -Wextra 
NAME = s21_smartcalc
TEST_NAME = test_$(NAME)
COVERAGE_FILES = ./*.gcno
COVERAGE_RESULTS = ./*.gcda
GCOV_FILES = ./*.gcov
LCOV_FILES = ./*.info

GTK_SOURCE_FILES = s21_gtk_*.c
LOGIC_SOURCE_FILES = LOGIC/s21_sc*.c
LOGIC_OBJECT_FILES = s21_sc*.o

LIBS = `pkg-config --libs --cflags gtk+-3.0` -rdynamic -lm

# different libs for MAC and LINUX
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CHECK_LIBS = $(shell pkg-config --libs check)
else
CHECK_LIBS = -lcheck
endif

all: clean uninstall install run

clean:
	rm -rf *.o *.a $(COVERAGE_RESULTS) $(COVERAGE_FILES) $(GCOV_FILES) $(LCOV_FILES) $(TEST_NAME) gcov_report $(TEST_NAME).dSYM

$(NAME).a:
	@rm -rf *.o *.a
	$(CC) $(FLAGS) -c $(LOGIC_SOURCE_FILES)
	ar rc $(NAME).a $(LOGIC_OBJECT_FILES)

tests: compile_test
	@rm -f $(COVERAGE_RESULTS)
	./$(TEST_NAME)

install: $(NAME).a
	$(CC) $(GTK_SOURCE_FILES) $(NAME).a -o ../build/$(NAME) $(LIBS) #$(FLAGS)

run:
	../build/s21_smartcalc

uninstall:
	rm -rf ../build/*

dvi:
	open readme.html

dist:
	tar -cf SmartCalc_v1.0.tar *.c *.h Makefile GUI LOGIC

gcov_report: gcov
	@rm -f $(LCOV_FILES)
	lcov -t $(NAME) -o $(NAME).info -c -d .
	lcov -r $(NAME).info "$(PWD)/$(TEST_NAME).check" -o $(NAME).info
	@rm -rf gcov_report
	genhtml $(NAME).info --function-coverage -o gcov_report
	open gcov_report/index.html

compile_test:
	checkmk $(TEST_NAME).check | $(CC) $(FLAGS) $(LOGIC_SOURCE_FILES) -xc - $(CHECK_LIBS) -o $(TEST_NAME) --coverage
	
gcov: compile_test
	@rm -f $(COVERAGE_RESULTS)
	./$(TEST_NAME)
	@rm -f $(GCOV_FILES)
	gcov $(TEST_NAME).c -f

asan: compile_test_with_asan
	@rm -f $(COVERAGE_RESULTS)
	./$(TEST_NAME)

compile_test_with_asan:
	checkmk $(TEST_NAME).check | $(CC) $(FLAGS) $(LOGIC_SOURCE_FILES) -xc - $(CHECK_LIBS) -o $(TEST_NAME) --coverage -g -fsanitize=address

lint:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.c *.h

clang:
	clang-format -i *.c *.h */*.c

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

leaks: compile_test
	leaks -atExit -- ./$(TEST_NAME) | grep -i LEAK:

valgrind_test: compile_test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(TEST_NAME)

valgrind: install
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ../build/s21_smartcalc

gtk_mac:
	brew install gtk+3

gtk_linux:
	sudo apt-get install libgtk-3-dev

deps:
	@command -v $(CC) >/dev/null 2>&1 || { echo >&2 "gcc is not installed"; exit 1; }
	@command -v gcov >/dev/null 2>&1 || { echo >&2 "gcov is not installed"; exit 1; }
	@command -v $(lcov) >/dev/null 2>&1 || { echo >&2 "$(lcov) is not installed"; exit 1; }
	@command -v $(genthml) >/dev/null 2>&1 || { echo >&2 "$(genhtml) is not installed"; exit 1;  }
	@if [[ "$OSTYPE" == "linux-gnu"* ]]; then \
		command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config is not installed"; exit 1; }; \
		pkg-config --list-all | grep -c ^check >/dev/null 2>&1 || { echo >&2 "check is not installed"; exit 1; }; \
	fi 
	@echo "dependencies are installed"

# docs for gtk3
# https://docs.gtk.org/gtk3
# 
