NAME = compiler
SHELL = sh
CC = gcc
CXX = g++
REMOVE = rm -f
REMOVEDIR = rm -rf

DEPDIR = .dep
DOCDIR = doc
TESTDIR = tests

SRC =	main.cpp \
		messages.cpp \
		lexer.cpp \
		token.cpp \

DEBUG = -g
OPTIMIZATION = -O3
CSTANDARD = -std=c++0x
INCLUDEDIRS = 
OBJDIR = obj
RUNFLAGS = 

CFLAGS =	$(INCLUDEDIRS) \
			-Wall \
			-Wextra \
			-Wmissing-declarations \
			$(CSTANDARD) \
			$(OPTIMIZATION) \

LDFLAGS = 

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF $(DEPDIR)/$(@F).d

# Combine all necessary flags and optional flags.
ALL_CFLAGS = $(CFLAGS) $(GENDEPFLAGS)

OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)

all: $(NAME)
default: $(NAME)

# Link object files to executable
$(NAME): $(OBJ)
	@echo 'LD: $@'
	@$(CXX) -o $@ $(ALL_CFLAGS) $^ $(LDFLAGS)

stutest.out: $(NAME)
	-$(NAME) $(RUNFLAGS) stutest1.in > stutest1.out
	-$(NAME) stutest2.in > stutest2.out

proftest.out: $(NAME)
	cat $(PROFTEST)
	$(NAME) $(PROFTEST) > proftest.out
	cat proftest.out

# Compile: create object files from C++ source files.
$(OBJDIR)/%.o : %.cpp
	@echo 'CC: $<'
	@$(CXX) -c $(ALL_CFLAGS) $< -o $@ 

doc:
	doxygen
	make -C $(DOCDIR)/latex

test: $(NAME) 
	make -C $(TESTDIR)
	python $(TESTDIR)/run-tests.py

clean:
	make -C $(TESTDIR) clean
	$(REMOVE) $(NAME)
	$(REMOVE) $(SRC:%.cpp=$(OBJDIR)/%.o)
	$(REMOVE) $(SRC:.cpp=.d)
	$(REMOVEDIR) $(DEPDIR)
	$(REMOVEDIR) $(OBJDIR)
	$(REMOVEDIR) $(DOCDIR)
	$(REMOVEDIR) test.txt

distclean: clean
	make -C $(TESTDIR) distclean

# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir $(DEPDIR) 2>/dev/null) $(wildcard $(DEPDIR)/*)

# Listing of phony targets.
.PHONY : clean doc

