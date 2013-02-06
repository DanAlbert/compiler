default: gtest/Makefile
	make -C gtest/
	make -C unit/

gtest/Makefile:
	cd gtest/ && ./configure && cd ..

clean:
	make -C gtest/ clean
	make -C unit/ clean

distclean: clean
	-make -C gtest/ distclean

.PHONY : clean gtest
