COMPILER = g++
GENERAL_FLAGS = -g -std=c++17 -Wall -Wextra
GTEST_FLAGS = -lgtest -lgtest_main
BOOSTLIB_FLAGS = -I multiprecision-Boost_1_84_0/include -DBOOST_MP_STANDALONE
INCLUDE_PATH_FLAGS = -I ./

all: tests run

run: build
	./main.out

build: main.out

tests: test.out
	./$^

coverage:
	make tests
	lcov --directory . --capture --output-file cov.info \
		--ignore-errors mismatch,gcov,source
	lcov --remove cov.info '/usr/*' '*/test.cpp' -o cov.info \
	     --ignore-errors mismatch,unused
	genhtml cov.info -o cov_report

cov_clean:
	-rm -r cov.info cov_report *.gcda *.gcno

main.out: main.cpp k_map.h boostlib
	$(COMPILER) $(GENERAL_FLAGS) $< -o $@ $(INCLUDE_PATH_FLAGS) $(BOOSTLIB_FLAGS)

test.out: test.cpp k_map.h boostlib
	$(COMPILER) $(GENERAL_FLAGS) $< -o $@ $(INCLUDE_PATH_FLAGS) $(BOOSTLIB_FLAGS) $(GTEST_FLAGS)

boostlib:
	wget https://github.com/boostorg/multiprecision/archive/refs/tags/Boost_1_84_0.zip
	unzip Boost_1_84_0.zip
	rm Boost_1_84_0.zip
	touch boostlib

lib_clean:
	-rm -rf boostlib Boost_1_84_0.zip multiprecision-Boost_1_84_0

clean: cov_clean
	-rm main.out test.out
