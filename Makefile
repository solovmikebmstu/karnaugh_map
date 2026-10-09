COMPILER = g++
GENERAL_FLAGS = -g -std=c++17
GTEST_FLAGS = -lgtest -lgtest_main
BOOSTLIB_FLAGS = -I multiprecision-Boost_1_84_0/include -DBOOST_MP_STANDALONE
INCLUDE_PATH_FLAGS = -I ./

all: tests run

run: build
	./main.out

build: main.out

tests: test.out
	./$^

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
	rm -rf boostlib Boost_1_84_0.zip multiprecision-Boost_1_84_0

clean:
	rm main.out test.out
