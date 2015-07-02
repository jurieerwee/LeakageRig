#Comment

COMPILER ?= $(GCC_PATH)g++
# -std=c++0x is required to allow extended initializer list

FLAGS ?= -o -g -Wall -std=c++0x -lstdc++ -lboost_regex  $(GCC_SUPPFLAGS)

LDFLAGS ?= -g
LDLIBS = -lm

EXECUTABLE = my_testRig

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)

all:  release

release: $(OBJS)
	$(COMPILER) $(LDFLAGS) -o $(EXECUTABLE) $(OBJS) $(LDLIBS)
	
obj/%.o: src/%.cpp
	$(COMPILER) $(FLAGS) -o $@ -c $<
	
clean:
	rm -f obj/*
	rm ${EXECUTABLE}
	
cleandata:
	rm cachegrind.out.*

dist-clean: clean
	rm -f $(EXECUTABLE) *~ .depend *.zip

