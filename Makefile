CC=g++ -std=c++17
CFLAGS=-Wall -Wextra -Werror -lstdc++
GCOV_LIBS=--coverage
BUILD_PATH=./
SOURCES=s21_matrix_oop.cpp
TEST_SOURSE = s21_matrix_test.cpp
H=s21_matrix_oop.h
LIBO=s21_matrix_oop.o
LIBA=s21_matrix_oop.a
EXE=test.out

OS = $(shell uname)

ifeq ($(OS), Darwin)
	LIBFLAGS = -lm -lgtest -lstdc++
else
	LIBFLAGS=-lstdc++ `pkg-config --cflags --libs gtest`
endif

all: s21_matrix_oop.a

s21_matrix_oop.a: clean
	$(CC) $(CFLAGS) -c $(SOURCES) -o $(BUILD_PATH)$(LIBO)
	ar rcs $(LIBA) $(LIBO)
	ranlib $(LIBA)

clean:
	rm -rf *.a && rm -rf *.o *.out
	rm -rf *.info && rm -rf *.gcda && rm -rf *.gcno &&  rm -rf *.gcov
	rm -rf report/ && rm -rf *.
	rm -f RESULT_VALGRIND.txt


test: s21_matrix_oop.a 
	@$(CC) $(CFLAGS) $(TEST_SOURSE) $(LIBA)  $(LIBFLAGS)  -o $(BUILD_PATH)$(EXE)
	@$(BUILD_PATH)$(EXE)

rebuild: clean all

gcov_report: s21_matrix_oop.a
	$(CC) $(CFLAGS) $(GCOV_LIBS) $(TEST_SOURSE) $(SOURCES) $(LIBA) -L. $(LIBA)  $(LIBFLAGS) -o $(BUILD_PATH)$(EXE)
	$(BUILD_PATH)$(EXE)
	lcov -t "test" -c -d  $(BUILD_PATH)  --no-external --output-file $(BUILD_PATH)coverage.info
	genhtml $(BUILD_PATH)coverage.info --output-directory $(BUILD_PATH)report/
ifeq ($(OS), Darwin)
	open ./report/index.html
else
	xdg-open ./report/index.html
endif
	rm -rf ./*.gcno ./*.gcda ./GcovReport.*


valgrind:
	valgrind --quiet --leak-check=full --leak-resolution=med --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test.out

check:
	cp ../materials/linters/.clang-format ./
	clang-format -i *.h *.cpp
	clang-format -n --verbose *.h *.cpp
	rm -rf ".clang-format"
	@ echo "Google style check is Done"

