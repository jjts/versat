PYTHON_DIR = ../../submodules/iob-soc/software/python/
CFLAGS=-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC 
INCLUDE_PC = -I../src/ -I.
VERSAT_INC=../../../hardware/include/



SRC_PC= ./testbench.c  ./test_versat.cpp

all:versat pc

versat:
	python ../../python/mkvhdr.py . .
	python ../../python/mkhdr.py . $(VERSAT_INC)  .


pc: ../src/versat.hpp versat.h 
	g++ -O3 -o firmware_PC.elf -pthread -lm $(CFLAGS) $(INCLUDE_PC) $(SRC_PC) ../src/*.cpp
	-mkdir iter
clean:
	@rm -rf *.elf *.h *.vh
	-rm -r iter
	-rm versat_info.txt


.PHONY: all clean
