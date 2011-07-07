


MYCFLAGS= -W -Wall -Wcast-align -Wtrigraphs \
	-Wpointer-arith -Wsign-compare -g\
	-Wformat=2 -Wno-format-y2k -Wmissing-braces -Wparentheses

all: dpcm.out

dpcm.out: compression.cc driver.cc compression.h
	g++ -o dpcm.out compression.cc driver.cc