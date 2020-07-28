.PNONY : all
# this works correctly on systems servers. To run in other systems, you may need to remove "env"
all : lab10
#	env time -p ./lab10

lab10 : lab10.c Makefile
	gcc -pthread -std=gnu11 -O2 -g -o $@ $<
