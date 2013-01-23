NAME = compiler
RUNFLAGS = "info for runtime flags"

default: $(NAME)

$(NAME): compiler.py
	@echo 'Creating executable: $(NAME)'
	@cp $< $@
	@chmod a+x $@

clean:
	rm -f *.pyc
	rm -f $(NAME)

stutest.out: $(NAME)
	-$(NAME) $(RUNFLAGS) stutest1.in > stutest1.out
	-$(NAME) stutest2.in > stutest2.out

proftest.out: $(NAME)
	cat $(PROFTEST)
	compiler $(PROFTEST) > proftest.out
	cat proftest.out
