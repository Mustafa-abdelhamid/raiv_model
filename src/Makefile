#=====================================================================
#
# Makefile                                              Date: 2022/10/29
#
# Author : Mustafa abdelhamid
#=====================================================================
all: raiv_core.o

raiv_core.o: raiv_core.c decoder.c
	gcc  $^  -o $@ -I $(PWD) 

clean:
	rm -rf raiv_core.o
