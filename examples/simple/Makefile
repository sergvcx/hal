.PHONY: all clean

all: simple.abs

clean:
	rm -f simple.abs

OUTPUT_DIR ?= .

mc12101:
	nmc-g++ -o $(OUTPUT_DIR)/factorial_mc12101.abs factorial.cpp -Wl,--whole-archive -l mc12101load_nm -Wl,--no-whole-archive -L "$(MC12101)/lib" -L "../../lib" -I../../include -l hal_mc12101 -T mc12101brd.lds

