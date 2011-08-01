


MYCFLAGS= -W -Wall -Wcast-align -Wtrigraphs \
	-Wpointer-arith -Wsign-compare -g\
	-Wformat=2 -Wno-format-y2k -Wmissing-braces -Wparentheses

all: pdpcm.out

pdpcm.out: compression.cc driver.cc compression.h
	mpiCC $(MYCFLAGS) -o pdpcm.out compression.cc driver.cc