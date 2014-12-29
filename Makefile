# Directories containing source files, separated by ":".
VPATH = src

# Source files.
GOL_SRC = \
	Main.cpp \
	Board.cpp \
	BasicBoard.cpp

# Dependencies and object files.
GOL_DEP = $(patsubst %.cpp, dep/%.d, ${GOL_SRC})
GOL_OBJ = $(patsubst %.cpp, obj/%.o, ${GOL_SRC})

# Name of application binary.
TARGET = gol

# Compiler to use.
CXXDEP = g++

# Include flags.
INCLUDEFLAGS = -I inc

# Compile options.
CXXFLAGS = -g -Wall -std=c++11

# Rules.
all: bin/${TARGET}

bin/$(TARGET): ${GOL_OBJ}
	@mkdir -p bin
	$(CXX) -g -o $@ $^

obj/%.o: %.cpp
	@mkdir -p obj
	$(CXX) -c $(CXXFLAGS) $(INCLUDEFLAGS) -o $@ $<

# Thanks to: http://users.softlab.ece.ntua.gr/~ttsiod/makefile.html
dep/%.d: %.cpp
	@mkdir -p dep
	@echo Generating dependencies for $^
	@set -e; \
	$(CXXDEP) -MM -MP $(INCLUDEFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj\/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	@rm -rf dep obj bin

ifneq ($(MAKECMDGOALS),clean)
-include ${GOL_DEP}
endif
