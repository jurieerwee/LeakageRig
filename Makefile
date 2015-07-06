#Comment

COMPILER ?= $(GCC_PATH)g++-4.9
# -std=c++14 is required to allow extended initializer list

FLAGS ?= -o -g -Wall -std=c++14 -lstdc++ -DBOOST_LOG_DYN_LINK   $(GCC_SUPPFLAGS)
# 
LDFLAGS ?= -g  -lboost_log -lboost_thread -lpthread -lboost_system  -lboost_log_setup
LDLIBS = -lm

EXECUTABLE = my_testRig

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)

all:  release

release: $(OBJS)
	$(COMPILER) $(OBJS)  $(LDFLAGS)  $(LDLIBS) -o $(EXECUTABLE) 
	
obj/%.o: src/%.cpp
	$(COMPILER) $(FLAGS) -o $@ -c $<
	
clean:
	rm -f obj/*
	rm ${EXECUTABLE}
	
cleandata:
	rm cachegrind.out.*

dist-clean: clean
	rm -f $(EXECUTABLE) *~ .depend *.zip

