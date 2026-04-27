# S21 Decimal Library - Makefile

LIB = s21_decimal.a
TEST_LIB = $(addprefix test_, $(LIB))

# Compiler
CC = gcc
GCOV = gcov

# Apple Silicon tweaks
ifeq ($(shell uname -p), arm)
	CC = gcc
	GCOV = gcov-12
	WNO = -Wno-builtin-memcpy-chk-size
endif

# If gcc 13, use gcov-13
ifeq ($(shell gcc --version | head -1 | grep -o '13'), 13)
	GCOV = gcov-13
endif

# Standard flags
FLAGS = -Wall -Werror -Wextra -std=c11 -g -Isrc
GFLAGS = -fprofile-arcs -ftest-coverage

# Check flags (preferred)
CHFLAGS = $(shell pkg-config --cflags --libs check)

# Directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
BUILD_LIB_DIR = $(BUILD_DIR)/library
BUILD_TEST_DIR = $(BUILD_DIR)/tests
TEST_BUILD_DIR = $(BUILD_DIR)/gcov_library
GCOV_DIR = gcov_report

# Files
S21_SOURCES = $(wildcard $(SRC_DIR)/s21_*.c)
TESTS = $(wildcard $(TEST_DIR)/test_*.c)
S21_OBJECTS = $(addprefix $(BUILD_LIB_DIR)/, $(notdir $(S21_SOURCES:.c=.o)))
TEST_S21_OBJECTS = $(addprefix $(TEST_BUILD_DIR)/, $(notdir $(S21_SOURCES:.c=.o)))
TEST_OBJECTS = $(addprefix $(BUILD_TEST_DIR)/, $(notdir $(TESTS:.c=.o)))

# Linker libs for Check (Linux needs extras)
ifeq ($(shell uname -s), Linux)
	CHECK_LIBS = -lcheck -lsubunit -lm -lrt -lpthread
else
	CHECK_LIBS = $(CHFLAGS)
endif

.PHONY: all clean gcov_report \
	style style-fix check \
	rebuild gcov \
	valgrind leaks run test

all: $(LIB)

###############################################
# BUILD & RUN TESTS (WITH TEST LIB)
###############################################

test: $(TEST_OBJECTS) $(TEST_LIB)
	$(CC) $(FLAGS) $^ -o $@ $(CHECK_LIBS) $(GFLAGS) -lm

$(TEST_OBJECTS): $(BUILD_TEST_DIR)/%.o : $(TEST_DIR)/%.c $(TEST_DIR)/test_common.h $(SRC_DIR)/s21_decimal.h
	@mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(FLAGS) $(GFLAGS) -I$(TEST_DIR) $(shell pkg-config --cflags check) -c $< -o $@ $(WNO)

###############################################
# BUILD TEST VERSION OF LIBRARY (WITH COVERAGE)
###############################################

$(TEST_LIB): $(TEST_S21_OBJECTS)
	ar -rcs $@ $^

$(TEST_S21_OBJECTS): $(TEST_BUILD_DIR)/%.o : $(SRC_DIR)/%.c $(SRC_DIR)/s21_decimal.h $(SRC_DIR)/s21_internal.h
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(FLAGS) $(GFLAGS) -c $< -o $@

###############################################
# BUILD PROD VERSION OF LIBRARY (NO COVERAGE)
###############################################

s21_decimal.a: $(S21_OBJECTS)
	ar -rcs $@ $^

$(LIB): $(S21_OBJECTS)
	ar -rcs $@ $^

$(S21_OBJECTS): $(BUILD_LIB_DIR)/%.o : $(SRC_DIR)/%.c $(SRC_DIR)/s21_decimal.h
	@mkdir -p $(BUILD_LIB_DIR)
	$(CC) $(FLAGS) -c $< -o $@

###############################################
# GCOV_REPORT
###############################################

gcov: gcov_report
	@if [ "$$(uname -s)" = "Darwin" ]; then \
		open ./$(GCOV_DIR)/index.html; \
	else \
		xdg-open ./$(GCOV_DIR)/index.html 2>/dev/null || echo "Open ./$(GCOV_DIR)/index.html"; \
	fi

run: test
	-./test || true

gcov_report: $(LIB) test $(GCOV_DIR)/index.html

$(GCOV_DIR)/index.html: $(LIB) test
	@mkdir -p $(GCOV_DIR)
	@-./test || true
	lcov -t "s21_decimal" --capture --initial -d $(BUILD_DIR) --ignore-errors empty --ignore-errors unused -o s21_decimal.base.info
	lcov -t "s21_decimal" --capture -d $(BUILD_DIR) --ignore-errors empty --ignore-errors unused -o s21_decimal.run.info
	lcov -a s21_decimal.base.info -a s21_decimal.run.info -o s21_decimal.info --ignore-errors empty --ignore-errors unused
	lcov --remove s21_decimal.info '/usr/*' '*/tests/*' '*/test_*' --ignore-errors empty --ignore-errors unused -o s21_decimal_filtered.info
	genhtml -o $(GCOV_DIR) s21_decimal_filtered.info
	@rm -f *.info

###############################################
# LOCAL CHECKS
###############################################

style:
	@echo "Checking style..."
	@clang-format -style=Google -n $(SRC_DIR)/*.c $(SRC_DIR)/*.h
	@clang-format -style=Google -n $(TEST_DIR)/*.c $(TEST_DIR)/*.h
	@echo "Style check completed!"

style-fix:
	@echo "Fixing style..."
	@clang-format -style=Google -i $(SRC_DIR)/*.c $(SRC_DIR)/*.h
	@clang-format -style=Google -i $(TEST_DIR)/*.c $(TEST_DIR)/*.h
	@echo "Style fix completed!"

check:
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=checkersReport --suppress=normalCheckLevelMaxBranches --suppress=unmatchedSuppression ./; \
	else \
		echo "cppcheck is not installed. Skipping..."; \
	fi

valgrind: test
	@echo "Running valgrind memory leak check..."
	@if [ "$$(uname -s)" = "Linux" ]; then \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test || true; \
	else \
		echo "Valgrind is only available/configured for Linux in this Makefile. Skipping... Run 'make leaks' for macOS."; \
	fi

leaks: test
	@echo "Running macOS leaks memory leak check..."
	@if [ "$$(uname -s)" = "Darwin" ]; then \
		leaks -atExit -- ./test || true; \
	else \
		echo "Leaks is only available on macOS. Skipping... Run 'make valgrind' for Linux."; \
	fi

###############################################
# CLEAN / REBUILD
###############################################

clean:
	@echo "==Cleaning...=="
	@rm -rf $(GCOV_DIR) $(BUILD_DIR)
	@rm -rf *.html *.css
	@rm -f test
	@rm -f *.a
	@rm -f *.gch $(TEST_DIR)/*.gch
	@rm -f *.gcda *.gcno $(TEST_DIR)/*.gcda $(TEST_DIR)/*.gcno
	@echo "==Done!=="

rebuild:
	$(MAKE) clean
	$(MAKE) all
