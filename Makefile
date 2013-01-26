NAME = compiler
SHELL = sh
CC = gcc
CPC = g++
REMOVE = rm -f
REMOVEDIR = rm -rf

SRC =	main.cpp \
		messages.cpp \

DEBUG = -g
OPTIMIZATION = -O3
CSTANDARD = 
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
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
ALL_CFLAGS = $(CFLAGS) $(GENDEPFLAGS)

OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)

all: $(NAME)
default: $(NAME)

# Link object files to executable
$(NAME): $(OBJ)
	@echo 'LD: $@'
	@$(CPC) -o $@ $(ALL_CFLAGS) $^ $(LDFLAGS)

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
	@$(CPC) -c $(ALL_CFLAGS) $< -o $@ 

doc:
	doxygen

clean:
	$(REMOVE) $(NAME)
	$(REMOVE) $(SRC:%.cpp=$(OBJDIR)/%.o)
	$(REMOVE) $(SRC:.cpp=.d)
	$(REMOVEDIR) .dep
	$(REMOVEDIR) $(OBJDIR)

# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# Listing of phony targets.
.PHONY : clean

