FLAGS = -std=c11 -Wall -Werror -Wextra 
NAME = s21_smartcalc
TEST_NAME = test_$(NAME)
COVERAGE_FILES = ./*.gcno
COVERAGE_RESULTS = ./*.gcda
GCOV_FILES = ./*.gcov
LCOV_FILES = ./*.info

SOURCE_FILES = s21*.c
OBJECT_FILES = s21*.o

# different libs for MAC and LINUX
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
LIBS = $(shell pkg-config --libs check)
else
LIBS = -lcheck
endif

all: clean tests

clean:
	rm -rf *.o *.a $(COVERAGE_RESULTS) $(COVERAGE_FILES) $(GCOV_FILES) $(LCOV_FILES) $(TEST_NAME) $(TEST_NAME).c gcov_report $(TEST_NAME).dSYM

tests: compile_test
	@rm -f $(COVERAGE_RESULTS)
	./$(TEST_NAME)


install:

uninstall:

dvi:

dist:

tests:

gcov_report:


gcov_report: gcov
	@rm -f $(LCOV_FILES)
	lcov -t $(NAME) -o $(NAME).info -c -d .
	lcov -r $(NAME).info "$(shell pwd)/$(TEST_NAME).c" -o $(NAME).info
	lcov -r $(NAME).info "$(shell pwd)/$(TEST_NAME).check" -o $(NAME).info
	@rm -rf gcov_report
	genhtml $(NAME).info --function-coverage -o gcov_report
	open gcov_report/index.html

compile_test:
	checkmk $(TEST_NAME).check | gcc $(FLAGS) $(SOURCE_FILES) -xc - $(LIBS) -o $(TEST_NAME) --coverage

gcov: compile_test
	@rm -f $(COVERAGE_RESULTS)
	./$(TEST_NAME)
	@rm -f $(GCOV_FILES)
	gcov $(TEST_NAME).c -f

asan: compile_test_with_asan
	@rm -f $(COVERAGE_RESULTS)
	./$(TEST_NAME)

compile_test_with_asan:
	checkmk $(TEST_NAME).check | gcc $(FLAGS) $(SOURCE_FILES) -xc - $(LIBS) -o $(TEST_NAME) --coverage -g -fsanitize=address

lint:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.c *.h

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

leaks: compile_test
	leaks -atExit -- ./$(TEST_NAME) | grep -i LEAK:

valgrind: compile_test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(TEST_NAME)

docker: clean
	cd ../materials/build && sh run.sh

deps:
	@command -v gcc >/dev/null 2>&1 || { echo >&2 "gcc is not installed"; exit 1; }
	@command -v gcov >/dev/null 2>&1 || { echo >&2 "gcov is not installed"; exit 1; }
	@command -v $(lcov) >/dev/null 2>&1 || { echo >&2 "$(lcov) is not installed"; exit 1; }
	@command -v $(genthml) >/dev/null 2>&1 || { echo >&2 "$(genhtml) is not installed"; exit 1;  }
	@if [[ "$OSTYPE" == "linux-gnu"* ]]; then \
		command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config is not installed"; exit 1; }; \
		pkg-config --list-all | grep -c ^check >/dev/null 2>&1 || { echo >&2 "check is not installed"; exit 1; }; \
	fi 
	@echo "dependencies are installed"