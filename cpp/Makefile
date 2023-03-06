#
# **************************************************************
# *         C++ Mathematical Expression Toolkit Library        *
# *                                                            *
# * Author: Arash Partow (1999-2023)                           *
# * URL: https://www.partow.net/programming/exprtk/index.html  *
# *                                                            *
# * Copyright notice:                                          *
# * Free use of the Mathematical Expression Toolkit Library is *
# * permitted under the guidelines and in accordance with the  *
# * most current version of the MIT License.                   *
# * http://www.opensource.org/licenses/MIT                     *
# *                                                            *
# **************************************************************
#


COMPILER         := -c++
#COMPILER        := -clang++
OPTIMIZATION_OPT := -O1
BASE_OPTIONS     := -pedantic-errors -Wall -Wextra -Werror -Wno-long-long
OPTIONS          := $(BASE_OPTIONS) $(OPTIMIZATION_OPT)
LINKER_OPT       := -L/usr/lib -lstdc++ -lm
ASAN_OPT         := -g -fsanitize=address   -fno-omit-frame-pointer
MSAN_OPT         := -g -fsanitize=memory    -fno-omit-frame-pointer
LSAN_OPT         := -g -fsanitize=leak      -fno-omit-frame-pointer
USAN_OPT         := -g -fsanitize=undefined -fno-omit-frame-pointer
BUILD_SRC        := $(sort $(wildcard exprtk_*.cpp))
BUILD_LIST       := $(BUILD_SRC:%.cpp=%)


all: judge

judge : judge.cpp exprtk.hpp
	$(COMPILER) $(OPTIONS) -o judge judge.cpp $(LINKER_OPT)

test : test.cpp exprtk.hpp
	$(COMPILER) $(OPTIONS) -o test test.cpp $(LINKER_OPT)

clean:
	rm -f core.* *~ *.o *.bak *stackdump gmon.out *.gcda *.gcno *.gcnor *.gch
